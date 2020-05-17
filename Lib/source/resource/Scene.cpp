#include "nelf/resource/Scene.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cstdlib>
#include <cstring>

#include "nelf/Directory.h"
#include "nelf/Engine.h"
#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/Log.h"
#include "nelf/Math.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/actor/Actor.h"
#include "nelf/actor/Camera.h"
#include "nelf/actor/Entity.h"
#include "nelf/actor/Light.h"
#include "nelf/actor/Particles.h"
#include "nelf/actor/Sprite.h"
#include "nelf/actor/lightType.h"
#include "nelf/audio/AudioDevice.h"
#include "nelf/drawMode.h"
#include "nelf/errorCode.h"
#include "nelf/gfx/gfxBlendMode.h"
#include "nelf/gfx/gfxDepthFunc.h"
#include "nelf/gfx/gfxDriver.h"
#include "nelf/gfx/gfxFogMode.h"
#include "nelf/gfx/gfxFormatType.h"
#include "nelf/gfx/gfxGBuffer.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/gfx/gfxObject.h"
#include "nelf/gfx/gfxProjectionMode.h"
#include "nelf/gfx/gfxQuery.h"
#include "nelf/gfx/gfxRenderTarget.h"
#include "nelf/gfx/gfxShaderProgram.h"
#include "nelf/gfx/gfxTextureMapType.h"
#include "nelf/gfx/gfxVertexArray.h"
#include "nelf/gfx/gfxVertexData.h"
#include "nelf/gfx/gfxVertexDataType.h"
#include "nelf/gfx/gfxVertexIndex.h"
#include "nelf/objectType.h"
#include "nelf/pak/Pak.h"
#include "nelf/pak/PakIndex.h"
#include "nelf/physics/Joint.h"
#include "nelf/physics/PhysicsObject.h"
#include "nelf/physics/PhysicsWorld.h"
#include "nelf/resource/Armature.h"
#include "nelf/resource/Material.h"
#include "nelf/resource/Model.h"
#include "nelf/resource/Resource.h"
#include "nelf/resource/Resources.h"
#include "nelf/resource/Script.h"
#include "nelf/resource/Texture.h"

// Fog shader
static const char* composeFogVert = R"##(
attribute vec3 elf_VertexAttr;
attribute vec2 elf_TexCoordAttr;
uniform mat4 elf_ModelviewMatrix;
uniform mat4 elf_ProjectionMatrix;

void main()
{
    gl_Position = elf_ProjectionMatrix * (elf_ModelviewMatrix * vec4(elf_VertexAttr, 1.0));
}
)##";

static const char* composeFogFrag = R"##(
uniform float elf_FogStart;
uniform float elf_FogEnd;
uniform vec3 elf_FogColor;

void main()
{
    float distance = (elf_FogEnd - gl_FragCoord.z / gl_FragCoord.w);
    float rangeDiff = (elf_FogEnd - elf_FogStart);
    float density = 1.0 - clamp(distance / rangeDiff, 0.0, 1.0);

    gl_FragColor = vec4(elf_FogColor, density);
}
)##";

elfScene* elfCreateScene(const char* name)
{
    elfScene* scene;

    scene = (elfScene*)malloc(sizeof(elfScene));
    memset(scene, 0x0, sizeof(elfScene));
    scene->objType = ELF_SCENE;
    scene->objDestr = elfDestroyScene;

    scene->models = elfCreateList();
    scene->scripts = elfCreateList();
    scene->textures = elfCreateList();
    scene->materials = elfCreateList();
    scene->cameras = elfCreateList();
    scene->entities = elfCreateList();
    scene->lights = elfCreateList();
    scene->armatures = elfCreateList();
    scene->particles = elfCreateList();
    scene->sprites = elfCreateList();
    scene->entityQueue = elfCreateList();
    scene->spriteQueue = elfCreateList();

    // TODO Why is this actually not done in the create* method?
    elfIncRef((elfObject*)scene->models);
    elfIncRef((elfObject*)scene->scripts);
    elfIncRef((elfObject*)scene->materials);
    elfIncRef((elfObject*)scene->textures);
    elfIncRef((elfObject*)scene->cameras);
    elfIncRef((elfObject*)scene->entities);
    elfIncRef((elfObject*)scene->lights);
    elfIncRef((elfObject*)scene->armatures);
    elfIncRef((elfObject*)scene->particles);
    elfIncRef((elfObject*)scene->sprites);
    elfIncRef((elfObject*)scene->entityQueue);
    elfIncRef((elfObject*)scene->spriteQueue);

    gfxSetShaderParamsDefault(&scene->shaderParams);

    scene->world = elfCreatePhysicsWorld();
    scene->dworld = elfCreatePhysicsWorld();

    scene->physics = true;
    scene->runScripts = true;

    if (name)
        scene->name = elfCreateString(name);

    scene->composeFogShdr = gfxCreateShaderProgram(composeFogVert, composeFogFrag);

    scene->id = ++res->sceneIdCounter;

    elfIncObj(ELF_SCENE);

    return scene;
}

#ifndef NO_ASSIMP
void elfRecursivelyImportAssets(elfScene* scene, const struct aiScene* aiscn, struct aiNode* aind)
{
    int i, j, k;
    elfEntity* entity;
    elfModel* model;
    elfMaterial* material;
    elfTexture* texture;
    int areaIndex;
    int vertexOffset;
    int indexOffset;
    float* vertexBuffer;
    float* normalBuffer;
    float* texcoordBuffer = NULL;
    unsigned int* indexBuffer;
    aiColor4D col;
    const aiMaterial* aimat;
    float shininess;
    float strength;
    unsigned int max;
    aiString path;
    aiString name;
    char* parentFolder;
    char* realPath;
    char* texName;
    bool isTexCoords = false;

    entity = elfCreateEntity(aind->mName.data);
    model = elfCreateModel(aind->mName.data);

    model->frameCount = 1;

    for (i = 0; i < (int)aind->mNumMeshes; i++)
    {
        const struct aiMesh* mesh = aiscn->mMeshes[aind->mMeshes[i]];

        if (mesh->mNumVertices < 3 || mesh->mNumFaces < 1)
            continue;

        model->verticeCount += mesh->mNumVertices;
        model->areaCount += 1;
        model->indiceCount += mesh->mNumFaces * 3;

        if (mesh->mTextureCoords[0] != NULL)
            isTexCoords = true;
    }

    if (model->verticeCount > 2 && model->indiceCount > 2)
    {
        model->vertices = gfxCreateVertexData(3 * model->verticeCount, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
        gfxIncRef((gfxObject*)model->vertices);
        vertexBuffer = (float*)gfxGetVertexDataBuffer(model->vertices);

        model->normals = gfxCreateVertexData(3 * model->verticeCount, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
        gfxIncRef((gfxObject*)model->normals);
        normalBuffer = (float*)gfxGetVertexDataBuffer(model->normals);

        if (isTexCoords)
        {
            model->texCoords = gfxCreateVertexData(2 * model->verticeCount, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
            gfxIncRef((gfxObject*)model->texCoords);
            texcoordBuffer = (float*)gfxGetVertexDataBuffer(model->texCoords);
        }

        model->areas = (elfModelArea*)malloc(sizeof(elfModelArea) * model->areaCount);
        memset(model->areas, 0x0, sizeof(elfModelArea) * model->areaCount);

        areaIndex = 0;
        vertexOffset = 0;

        for (i = 0; i < (int)aind->mNumMeshes; i++)
        {
            const struct aiMesh* mesh = aiscn->mMeshes[aind->mMeshes[i]];

            if (mesh->mNumVertices < 3 || mesh->mNumFaces < 1)
                continue;

            for (j = 0; j < (int)mesh->mNumVertices; j++)
            {
                memcpy(&vertexBuffer[(vertexOffset + j) * 3], &mesh->mVertices[j].x, sizeof(float) * 3);
                if (mesh->mNormals != NULL)
                    memcpy(&normalBuffer[(vertexOffset + j) * 3], &mesh->mNormals[j].x, sizeof(float) * 3);
                if (mesh->mTextureCoords[0] != NULL)
                    memcpy(&texcoordBuffer[(vertexOffset + j) * 2], &mesh->mTextureCoords[0][j].x, sizeof(float) * 2);
            }

            model->areas[areaIndex].indiceCount = mesh->mNumFaces * 3;
            model->areas[areaIndex].index =
                gfxCreateVertexData(model->areas[areaIndex].indiceCount, GFX_UINT, GFX_VERTEX_DATA_STATIC);
            gfxIncRef((gfxObject*)model->areas[areaIndex].index);
            indexBuffer = (unsigned int*)gfxGetVertexDataBuffer(model->areas[areaIndex].index);

            for (j = 0; j < (int)mesh->mNumFaces; j++)
            {
                const struct aiFace* face = &mesh->mFaces[j];

                if (face->mNumIndices != 3)
                {
                    elfSetError(ELF_INVALID_MESH, "error: unexpected non triangular face when loading mesh\n");
                    continue;
                }

                for (k = 0; k < (int)face->mNumIndices; k++)
                {
                    indexBuffer[j * 3 + k] = vertexOffset + face->mIndices[k];
                }
            }

            vertexOffset += mesh->mNumVertices;

            model->areas[areaIndex].vertexIndex = gfxCreateVertexIndex(true, model->areas[areaIndex].index);
            gfxIncRef((gfxObject*)model->areas[areaIndex].vertexIndex);

            areaIndex += 1;

            aimat = aiscn->mMaterials[mesh->mMaterialIndex];

            aiGetMaterialString(aimat, AI_MATKEY_NAME, &name);

            material = elfCreateMaterial(name.data);

            if (AI_SUCCESS == aiGetMaterialColor(aimat, AI_MATKEY_COLOR_DIFFUSE, &col))
                elfSetMaterialDiffuseColor(material, col.r, col.g, col.b, col.a);
            if (AI_SUCCESS == aiGetMaterialColor(aimat, AI_MATKEY_COLOR_SPECULAR, &col))
                elfSetMaterialSpecularColor(material, col.r, col.g, col.b, col.a);
            if (AI_SUCCESS == aiGetMaterialColor(aimat, AI_MATKEY_COLOR_AMBIENT, &col))
                elfSetMaterialAmbientColor(material, col.r, col.g, col.b, col.a);

            max = 1;
            if (AI_SUCCESS == aiGetMaterialFloatArray(aimat, AI_MATKEY_SHININESS, &shininess, &max))
            {
                max = 1;
                if (AI_SUCCESS == aiGetMaterialFloatArray(aimat, AI_MATKEY_SHININESS_STRENGTH, &strength, &max))
                {
                    elfSetMaterialSpecularPower(material, shininess * strength);
                }
                else
                {
                    elfSetMaterialSpecularPower(material, shininess);
                }
            }

            parentFolder = elfGetDirectoryFromPath(scene->filePath);
            realPath = NULL;

            if (AI_SUCCESS == aiGetMaterialString(aimat, AI_MATKEY_TEXTURE_DIFFUSE(0), &path))
            {
                if (realPath)
                    elfDestroyString(realPath);
                if (strlen(path.data) > 1 && path.data[0] != '/' && path.data[1] != ':')
                    realPath = elfMergeStrings(parentFolder, path.data);
                else
                    realPath = elfCreateString(path.data);
                texName = elfGetFileFromPath(realPath);
                texture = elfCreateTextureFromFile(texName, realPath);
                elfDestroyString(texName);
                if (texture)
                    elfSetMaterialDiffuseMap(material, texture);
            }

            if (AI_SUCCESS == aiGetMaterialString(aimat, AI_MATKEY_TEXTURE_SPECULAR(0), &path))
            {
                if (realPath)
                    elfDestroyString(realPath);
                if (strlen(path.data) > 1 && path.data[0] != '/' && path.data[1] != ':')
                    realPath = elfMergeStrings(parentFolder, path.data);
                else
                    realPath = elfCreateString(path.data);
                texName = elfGetFileFromPath(realPath);
                texture = elfCreateTextureFromFile(texName, realPath);
                elfDestroyString(texName);
                if (texture)
                    elfSetMaterialSpecularMap(material, texture);
            }

            if (AI_SUCCESS == aiGetMaterialString(aimat, AI_MATKEY_TEXTURE_NORMALS(0), &path))
            {
                if (realPath)
                    elfDestroyString(realPath);
                if (strlen(path.data) > 1 && path.data[0] != '/' && path.data[1] != ':')
                    realPath = elfMergeStrings(parentFolder, path.data);
                else
                    realPath = elfCreateString(path.data);
                texName = elfGetFileFromPath(realPath);
                texture = elfCreateTextureFromFile(texName, realPath);
                elfDestroyString(texName);
                if (texture)
                    elfSetMaterialNormalMap(material, texture);
            }

            if (AI_SUCCESS == aiGetMaterialString(aimat, AI_MATKEY_TEXTURE_LIGHTMAP(0), &path))
            {
                if (realPath)
                    elfDestroyString(realPath);
                if (strlen(path.data) > 1 && path.data[0] != '/' && path.data[1] != ':')
                    realPath = elfMergeStrings(parentFolder, path.data);
                else
                    realPath = elfCreateString(path.data);
                texName = elfGetFileFromPath(realPath);
                texture = elfCreateTextureFromFile(texName, realPath);
                elfDestroyString(texName);
                if (texture)
                    elfSetMaterialLightMap(material, texture);
            }

            if (AI_SUCCESS == aiGetMaterialString(aimat, AI_MATKEY_TEXTURE_HEIGHT(0), &path))
            {
                if (realPath)
                    elfDestroyString(realPath);
                if (strlen(path.data) > 1 && path.data[0] != '/' && path.data[1] != ':')
                    realPath = elfMergeStrings(parentFolder, path.data);
                else
                    realPath = elfCreateString(path.data);
                texName = elfGetFileFromPath(realPath);
                texture = elfCreateTextureFromFile(texName, realPath);
                elfDestroyString(texName);
                if (texture)
                    elfSetMaterialHeightMap(material, texture);
            }

            elfDestroyString(parentFolder);
            if (realPath)
                elfDestroyString(realPath);

            elfAddEntityMaterial(entity, material);
        }

        model->vertexArray = gfxCreateVertexArray(true);
        gfxIncRef((gfxObject*)model->vertexArray);

        gfxSetVertexArrayData(model->vertexArray, GFX_VERTEX, model->vertices);
        gfxSetVertexArrayData(model->vertexArray, GFX_NORMAL, model->normals);
        if (isTexCoords)
            gfxSetVertexArrayData(model->vertexArray, GFX_TEX_COORD, model->texCoords);

        model->index = (unsigned int*)malloc(sizeof(unsigned int) * model->indiceCount);

        indexOffset = 0;

        for (i = 0; i < model->areaCount; i++)
        {
            indexBuffer = (unsigned int*)gfxGetVertexDataBuffer(model->areas[i].index);
            memcpy(&model->index[indexOffset], indexBuffer, sizeof(unsigned int) * model->areas[i].indiceCount);
            indexOffset += model->areas[i].indiceCount;
        }

        // get bounding box values
        memcpy(&model->bbMin.x, vertexBuffer, sizeof(float) * 3);
        memcpy(&model->bbMax.x, vertexBuffer, sizeof(float) * 3);

        for (i = 3; i < model->verticeCount * 3; i += 3)
        {
            if (vertexBuffer[i] < model->bbMin.x)
                model->bbMin.x = vertexBuffer[i];
            if (vertexBuffer[i + 1] < model->bbMin.y)
                model->bbMin.y = vertexBuffer[i + 1];
            if (vertexBuffer[i + 2] < model->bbMin.z)
                model->bbMin.z = vertexBuffer[i + 2];

            if (vertexBuffer[i] > model->bbMax.x)
                model->bbMax.x = vertexBuffer[i];
            if (vertexBuffer[i + 1] > model->bbMax.y)
                model->bbMax.y = vertexBuffer[i + 1];
            if (vertexBuffer[i + 2] > model->bbMax.z)
                model->bbMax.z = vertexBuffer[i + 2];
        }

        // set entity model
        elfSetEntityModel(entity, model);

        elfGenerateEntityTangents(entity);
        elfAddSceneEntity(scene, entity);
    }
    else
    {
        elfDestroyModel(model);
        elfDestroyEntity(entity);
    }

    for (i = 0; i < (int)aind->mNumChildren; i++)
    {
        elfRecursivelyImportAssets(scene, aiscn, aind->mChildren[i]);
    }
}
#endif

elfScene* elfCreateSceneFromFile(const char* name, const char* filePath)
{
    elfPak* pak;
    elfScene* scene;
    const char* type;

    type = strrchr(filePath, '.');
    if (!type)
    {
        elfSetError(ELF_INVALID_FILE, "error: can't open file \"%s\", no identifying file extension\n", filePath);
        return NULL;
    }

    if (!strcmp(type, ".pak"))
    {
        pak = elfCreatePakFromFile(filePath);
        if (!pak)
            return NULL;

        scene = elfCreateSceneFromPak(name, pak);

        return scene;
    }
#ifndef NO_ASSIMP
    else if (aiIsExtensionSupported(type))
    {
        const struct aiScene* aiscn;
        struct aiLogStream stream;

        stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);

        aiscn = aiImportFile(filePath, aiProcessPreset_TargetRealtime_Quality);
        if (!aiscn)
        {
            elfSetError(ELF_INVALID_FILE, "error: assimp failed to load file \"%s\"\n", filePath);
            return NULL;
        }

        scene = elfCreateScene(name);
        scene->filePath = elfCreateString(filePath);

        elfRecursivelyImportAssets(scene, aiscn, aiscn->mRootNode);

        aiReleaseImport(aiscn);

        return scene;
    }
#endif
    else
    {
        elfSetError(ELF_INVALID_FILE, "error: can't open \"%s\", unsupported format\n", filePath);
        return NULL;
    }
}

unsigned char elfSaveScene(elfScene* scene, const char* filePath) { return elfSaveSceneToPak(scene, filePath); }

void elfUpdateScene(elfScene* scene, float sync)
{
    elfCamera* cam;
    elfEntity* ent;
    elfLight* light;
    elfParticles* par;
    elfSprite* spr;
    float position[3];
    float orient[4];
    float vecZ[3] = {0.0f, 0.0f, -1.0f};
    float vecY[3] = {0.0f, 1.0f, -1.0f};
    float frontUpVec[6];

    if (sync > 0.0f)
    {
        if (scene->physics)
            elfUpdatePhysicsWorld(scene->world, sync);
        elfUpdatePhysicsWorld(scene->dworld, sync);
    }

    if (scene->curCamera)
    {
        elfGetActorPosition_((elfActor*)scene->curCamera, position);
        elfGetActorOrientation_((elfActor*)scene->curCamera, orient);
        gfxMulQuaVec(orient, vecZ, &frontUpVec[0]);
        gfxMulQuaVec(orient, vecY, &frontUpVec[3]);
        elfSetAudioListenerPosition(position[0], position[1], position[2]);
        elfSetAudioListenerOrientation(frontUpVec);
    }

    // logics update pass
    for (cam = (elfCamera*)elfBeginList(scene->cameras); cam != NULL; cam = (elfCamera*)elfGetListNext(scene->cameras))
    {
        elfUpdateCamera(cam);
    }

    for (ent = (elfEntity*)elfBeginList(scene->entities); ent != NULL;
         ent = (elfEntity*)elfGetListNext(scene->entities))
    {
        elfUpdateEntity(ent);
    }

    for (light = (elfLight*)elfBeginList(scene->lights); light != NULL;
         light = (elfLight*)elfGetListNext(scene->lights))
    {
        elfUpdateLight(light);
    }

    for (par = (elfParticles*)elfBeginList(scene->particles); par != NULL;
         par = (elfParticles*)elfGetListNext(scene->particles))
    {
        elfUpdateParticles(par, sync);
    }

    for (spr = (elfSprite*)elfBeginList(scene->sprites); spr != NULL; spr = (elfSprite*)elfGetListNext(scene->sprites))
    {
        elfUpdateSprite(spr);
    }
}

void elfScenePreDraw(elfScene* scene)
{
    elfCamera* cam;
    elfEntity* ent;
    elfLight* light;
    elfSprite* spr;
    elfParticles* par;

    for (cam = (elfCamera*)elfBeginList(scene->cameras); cam != NULL; cam = (elfCamera*)elfGetListNext(scene->cameras))
    {
        elfCameraPreDraw(cam);
    }

    for (ent = (elfEntity*)elfBeginList(scene->entities); ent != NULL;
         ent = (elfEntity*)elfGetListNext(scene->entities))
    {
        elfEntityPreDraw(ent);
    }

    for (light = (elfLight*)elfBeginList(scene->lights); light != NULL;
         light = (elfLight*)elfGetListNext(scene->lights))
    {
        elfLightPreDraw(light);
    }

    for (spr = (elfSprite*)elfBeginList(scene->sprites); spr != NULL; spr = (elfSprite*)elfGetListNext(scene->sprites))
    {
        elfSpritePreDraw(spr, scene->curCamera);
    }

    for (par = (elfParticles*)elfBeginList(scene->particles); par != NULL;
         par = (elfParticles*)elfGetListNext(scene->particles))
    {
        elfParticlesPreDraw(par);
    }
}

void elfScenePostDraw(elfScene* scene)
{
    elfCamera* cam;
    elfEntity* ent;
    elfLight* light;
    elfSprite* spr;
    elfParticles* par;

    for (cam = (elfCamera*)elfBeginList(scene->cameras); cam != NULL; cam = (elfCamera*)elfGetListNext(scene->cameras))
    {
        elfCameraPostDraw(cam);
    }

    for (ent = (elfEntity*)elfBeginList(scene->entities); ent != NULL;
         ent = (elfEntity*)elfGetListNext(scene->entities))
    {
        elfEntityPostDraw(ent);
    }

    for (light = (elfLight*)elfBeginList(scene->lights); light != NULL;
         light = (elfLight*)elfGetListNext(scene->lights))
    {
        elfLightPostDraw(light);
    }

    for (spr = (elfSprite*)elfBeginList(scene->sprites); spr != NULL; spr = (elfSprite*)elfGetListNext(scene->sprites))
    {
        elfSpritePostDraw(spr);
    }

    for (par = (elfParticles*)elfBeginList(scene->particles); par != NULL;
         par = (elfParticles*)elfGetListNext(scene->particles))
    {
        elfParticlesPostDraw(par);
    }
}

void elfDestroyScene(void* data)
{
    elfScene* scene = (elfScene*)data;

    elfActor* actor;

    if (scene->name)
        elfDestroyString(scene->name);
    if (scene->filePath)
        elfDestroyString(scene->filePath);

    if (scene->entityQueue)
        elfDecRef((elfObject*)scene->entityQueue);
    if (scene->spriteQueue)
        elfDecRef((elfObject*)scene->spriteQueue);

    for (actor = (elfActor*)elfBeginList(scene->cameras); actor; actor = (elfActor*)elfGetListNext(scene->cameras))
        elfRemoveActor(actor);
    for (actor = (elfActor*)elfBeginList(scene->entities); actor; actor = (elfActor*)elfGetListNext(scene->entities))
        elfRemoveActor(actor);
    for (actor = (elfActor*)elfBeginList(scene->lights); actor; actor = (elfActor*)elfGetListNext(scene->lights))
        elfRemoveActor(actor);
    for (actor = (elfActor*)elfBeginList(scene->particles); actor; actor = (elfActor*)elfGetListNext(scene->particles))
        elfRemoveActor(actor);
    for (actor = (elfActor*)elfBeginList(scene->sprites); actor; actor = (elfActor*)elfGetListNext(scene->sprites))
        elfRemoveActor(actor);

    if (scene->models)
        elfDecRef((elfObject*)scene->models);
    if (scene->scripts)
        elfDecRef((elfObject*)scene->scripts);
    if (scene->materials)
        elfDecRef((elfObject*)scene->materials);
    if (scene->textures)
        elfDecRef((elfObject*)scene->textures);
    if (scene->cameras)
        elfDecRef((elfObject*)scene->cameras);
    if (scene->entities)
        elfDecRef((elfObject*)scene->entities);
    if (scene->lights)
        elfDecRef((elfObject*)scene->lights);
    if (scene->armatures)
        elfDecRef((elfObject*)scene->armatures);
    if (scene->particles)
        elfDecRef((elfObject*)scene->particles);
    if (scene->sprites)
        elfDecRef((elfObject*)scene->sprites);

    elfDestroyPhysicsWorld(scene->world);
    elfDestroyPhysicsWorld(scene->dworld);

    if (scene->pak)
        elfDecRef((elfObject*)scene->pak);

    if (scene->composeFogShdr)
        gfxDestroyShaderProgram(scene->composeFogShdr);

    elfDecObj(ELF_SCENE);

    free(scene);
}

const char* elfGetSceneName(elfScene* scene) { return scene->name; }

const char* elfGetSceneFilePath(elfScene* scene) { return scene->filePath; }

void elfSetScenePhysics(elfScene* scene, bool physics) { scene->physics = physics; }

bool elfGetScenePhysics(elfScene* scene) { return scene->physics; }

void elfSetSceneRunScripts(elfScene* scene, bool runScripts) { scene->runScripts = runScripts; }

bool elfGetSceneRunScripts(elfScene* scene) { return scene->runScripts; }

void elfSetSceneDebugDraw(elfScene* scene, bool debugDraw) { scene->debugDraw = debugDraw; }

bool elfGetSceneDebugDraw(elfScene* scene) { return scene->debugDraw; }

void elfSetSceneOcclusionCulling(elfScene* scene, bool occlusionCulling)
{
    // TODO Why?
    if (gfxGetVersion() < 150)
        return;

    scene->occlusionCulling = occlusionCulling;
}

bool elfGetSceneOcclusionCulling(elfScene* scene) { return scene->occlusionCulling; }

void elfSetSceneGravity(elfScene* scene, float x, float y, float z)
{
    elfSetPhysicsWorldGravity(scene->world, x, y, z);
}

elfVec3f elfGetSceneGravity(elfScene* scene) { return elfGetPhysicsWorldGravity(scene->world); }

void elfSetSceneAmbientColor(elfScene* scene, float r, float g, float b, float a)
{
    scene->ambientColor.r = r;
    scene->ambientColor.g = g;
    scene->ambientColor.b = b;
    scene->ambientColor.a = a;
}

elfColor elfGetSceneAmbientColor(elfScene* scene) { return scene->ambientColor; }

void elfSetSceneFog(elfScene* scene, bool fog) { scene->fog = fog; }

void elfSetSceneFogStart(elfScene* scene, float start)
{
    scene->fogStart = start;
    if (scene->fogStart < 0.0f)
        scene->fogStart = 0.0f;
}

void elfSetSceneFogEnd(elfScene* scene, float end)
{
    scene->fogEnd = end;
    if (scene->fogEnd < 0.0f)
        scene->fogEnd = 0.0f;
}

void elfSetSceneFogColor(elfScene* scene, float r, float g, float b, float a)
{
    scene->fogColor.r = r;
    scene->fogColor.g = g;
    scene->fogColor.b = b;
    scene->fogColor.a = a;
}

bool elfGetSceneFog(elfScene* scene) { return scene->fog; }

float elfGetSceneFogStart(elfScene* scene) { return scene->fogStart; }

float elfGetFogEnd(elfScene* scene) { return scene->fogEnd; }

elfColor elfGetFogColor(elfScene* scene) { return scene->fogColor; }

int elfGetSceneCameraCount(elfScene* scene) { return elfGetListLength(scene->cameras); }

int elfGetSceneEntityCount(elfScene* scene) { return elfGetListLength(scene->entities); }

int elfGetSceneLightCount(elfScene* scene) { return elfGetListLength(scene->lights); }

int elfGetSceneArmatureCount(elfScene* scene) { return elfGetListLength(scene->armatures); }

int elfGetSceneParticlesCount(elfScene* scene) { return elfGetListLength(scene->particles); }

int elfGetSceneSpriteCount(elfScene* scene) { return elfGetListLength(scene->sprites); }

void elfSetActorScene(elfScene* scene, elfActor* actor)
{
    elfJoint* joint;

    if (actor->scene)
        elfRemoveSceneActorByObject(actor->scene, actor);

    actor->scene = scene;

    if (actor->object)
        elfSetPhysicsObjectWorld(actor->object, scene->world);
    if (actor->dobject)
        elfSetPhysicsObjectWorld(actor->dobject, scene->dworld);

    for (joint = (elfJoint*)elfBeginList(actor->joints); joint; joint = (elfJoint*)elfGetListNext(actor->joints))
    {
        elfSetJointWorld(joint, scene->world);
    }
}

void elfAddSceneCamera(elfScene* scene, elfCamera* camera)
{
    if (!camera)
        return;
    elfSetActorScene(scene, (elfActor*)camera);
    elfAppendListObject(scene->cameras, (elfObject*)camera);
    if (elfGetListLength(scene->cameras) == 1)
        scene->curCamera = camera;
}

void elfAddSceneEntity(elfScene* scene, elfEntity* entity)
{
    if (!entity)
        return;
    elfSetActorScene(scene, (elfActor*)entity);
    elfAppendListObject(scene->entities, (elfObject*)entity);
}

void elfAddSceneLight(elfScene* scene, elfLight* light)
{
    if (!light)
        return;
    elfSetActorScene(scene, (elfActor*)light);
    elfAppendListObject(scene->lights, (elfObject*)light);
}

void elfAddSceneParticles(elfScene* scene, elfParticles* particles)
{
    if (!particles)
        return;
    elfSetActorScene(scene, (elfActor*)particles);
    elfAppendListObject(scene->particles, (elfObject*)particles);
}

void elfAddSceneSprite(elfScene* scene, elfSprite* sprite)
{
    if (!sprite)
        return;
    elfSetActorScene(scene, (elfActor*)sprite);
    elfAppendListObject(scene->sprites, (elfObject*)sprite);
}

void elfSetSceneActiveCamera(elfScene* scene, elfCamera* camera) { scene->curCamera = camera; }

elfCamera* elfGetSceneActiveCamera(elfScene* scene) { return scene->curCamera; }

elfCollision* elfGetSceneRayCastResult(elfScene* scene, float x, float y, float z, float dx, float dy, float dz)
{
    return elfGetRayCastResult(scene->world, x, y, z, dx, dy, dz);
}

elfList* elfGetSceneRayCastResults(elfScene* scene, float x, float y, float z, float dx, float dy, float dz)
{
    return elfGetRayCastResults(scene->world, x, y, z, dx, dy, dz);
}

elfCollision* elfGetDebugSceneRayCastResult(elfScene* scene, float x, float y, float z, float dx, float dy, float dz)
{
    return elfGetRayCastResult(scene->dworld, x, y, z, dx, dy, dz);
}

elfList* elfGetDebugSceneRayCastResults(elfScene* scene, float x, float y, float z, float dx, float dy, float dz)
{
    return elfGetRayCastResults(scene->dworld, x, y, z, dx, dy, dz);
}

elfCamera* elfGetSceneCameraByIndex(elfScene* scene, int idx)
{
    return (elfCamera*)elfGetListObject(scene->cameras, idx);
}

elfEntity* elfGetSceneEntityByIndex(elfScene* scene, int idx)
{
    return (elfEntity*)elfGetListObject(scene->entities, idx);
}

elfLight* elfGetSceneLightByIndex(elfScene* scene, int idx) { return (elfLight*)elfGetListObject(scene->lights, idx); }

elfArmature* elfGetSceneArmatureByIndex(elfScene* scene, int idx)
{
    return (elfArmature*)elfGetListObject(scene->armatures, idx);
}

elfParticles* elfGetSceneParticlesByIndex(elfScene* scene, int idx)
{
    return (elfParticles*)elfGetListObject(scene->particles, idx);
}

elfSprite* elfGetSceneSpriteByIndex(elfScene* scene, int idx)
{
    return (elfSprite*)elfGetListObject(scene->sprites, idx);
}

elfTexture* elfGetSceneTexture(elfScene* scene, const char* name)
{
    elfTexture* texture;

    for (texture = (elfTexture*)elfBeginList(scene->textures); texture;
         texture = (elfTexture*)elfGetListNext(scene->textures))
    {
        if (!strcmp(texture->name, name))
            return texture;
    }

    return NULL;
}

elfModel* elfGetSceneModel(elfScene* scene, const char* name)
{
    elfModel* model;

    for (model = (elfModel*)elfBeginList(scene->models); model; model = (elfModel*)elfGetListNext(scene->models))
    {
        if (!strcmp(model->name, name))
            return model;
    }

    return NULL;
}

elfScript* elfGetSceneScript(elfScene* scene, const char* name)
{
    elfScript* script;

    for (script = (elfScript*)elfBeginList(scene->scripts); script; script = (elfScript*)elfGetListNext(scene->scripts))
    {
        if (!strcmp(script->name, name))
            return script;
    }

    return NULL;
}

elfMaterial* elfGetSceneMaterial(elfScene* scene, const char* name)
{
    elfMaterial* material;

    for (material = (elfMaterial*)elfBeginList(scene->materials); material;
         material = (elfMaterial*)elfGetListNext(scene->materials))
    {
        if (!strcmp(material->name, name))
            return material;
    }

    return NULL;
}

elfActor* elfGetSceneActor(elfScene* scene, const char* name)
{
    elfActor* actor;

    actor = (elfActor*)elfGetSceneCamera(scene, name);
    if (actor)
        return actor;
    actor = (elfActor*)elfGetSceneEntity(scene, name);
    if (actor)
        return actor;
    actor = (elfActor*)elfGetSceneLight(scene, name);
    if (actor)
        return actor;
    actor = (elfActor*)elfGetSceneArmature(scene, name);
    if (actor)
        return actor;
    actor = (elfActor*)elfGetSceneParticles(scene, name);
    if (actor)
        return actor;

    return NULL;
}

elfCamera* elfGetSceneCamera(elfScene* scene, const char* name)
{
    elfCamera* camera;

    for (camera = (elfCamera*)elfBeginList(scene->cameras); camera; camera = (elfCamera*)elfGetListNext(scene->cameras))
    {
        if (!strcmp(camera->name, name))
            return camera;
    }

    return NULL;
}

elfEntity* elfGetSceneEntity(elfScene* scene, const char* name)
{
    elfEntity* entity;

    for (entity = (elfEntity*)elfBeginList(scene->entities); entity;
         entity = (elfEntity*)elfGetListNext(scene->entities))
    {
        if (!strcmp(entity->name, name))
            return entity;
    }

    return NULL;
}

elfLight* elfGetSceneLight(elfScene* scene, const char* name)
{
    elfLight* light;

    for (light = (elfLight*)elfBeginList(scene->lights); light; light = (elfLight*)elfGetListNext(scene->lights))
    {
        if (!strcmp(light->name, name))
            return light;
    }

    return NULL;
}

elfArmature* elfGetSceneArmature(elfScene* scene, const char* name)
{
    elfArmature* armature;

    for (armature = (elfArmature*)elfBeginList(scene->armatures); armature;
         armature = (elfArmature*)elfGetListNext(scene->armatures))
    {
        if (!strcmp(armature->name, name))
            return armature;
    }

    return NULL;
}

elfParticles* elfGetSceneParticles(elfScene* scene, const char* name)
{
    elfParticles* particles;

    for (particles = (elfParticles*)elfBeginList(scene->particles); particles;
         particles = (elfParticles*)elfGetListNext(scene->particles))
    {
        if (!strcmp(particles->name, name))
            return particles;
    }

    return NULL;
}

elfSprite* elfGetSceneSprite(elfScene* scene, const char* name)
{
    elfSprite* sprite;

    for (sprite = (elfSprite*)elfBeginList(scene->sprites); sprite; sprite = (elfSprite*)elfGetListNext(scene->sprites))
    {
        if (!strcmp(sprite->name, name))
            return sprite;
    }

    return NULL;
}

elfTexture* elfGetOrLoadTextureByName(elfScene* scene, const char* name)
{
    elfTexture* texture;
    elfPakIndex* index;
    FILE* file;

    for (texture = (elfTexture*)elfBeginList(scene->textures); texture;
         texture = (elfTexture*)elfGetListNext(scene->textures))
    {
        if (!strcmp(texture->name, name))
            return texture;
    }

    if (scene->pak)
    {
        index = elfGetPakIndexByName(scene->pak, name, ELF_TEXTURE);
        if (index)
        {
            file = fopen(scene->pak->filePath, "rb");
            fseek(file, elfGetPakIndexOffset(index), SEEK_SET);
            if (feof(file))
                return NULL;

            texture = elfCreateTextureFromPak(file, name, scene);
            if (texture)
                elfAppendListObject(scene->textures, (elfObject*)texture);
            fclose(file);
            return texture;
        }
    }

    return NULL;
}

elfModel* elfGetOrLoadModelByName(elfScene* scene, const char* name)
{
    elfModel* model;
    elfPakIndex* index;
    FILE* file;

    for (model = (elfModel*)elfBeginList(scene->models); model; model = (elfModel*)elfGetListNext(scene->models))
    {
        if (!strcmp(model->name, name))
            return model;
    }

    if (scene->pak)
    {
        index = elfGetPakIndexByName(scene->pak, name, ELF_MODEL);
        if (index)
        {
            file = fopen(scene->pak->filePath, "rb");
            fseek(file, elfGetPakIndexOffset(index), SEEK_SET);
            if (feof(file))
                return NULL;

            model = elfCreateModelFromPak(file, name, scene);
            if (model)
                elfAppendListObject(scene->models, (elfObject*)model);
            fclose(file);
            return model;
        }
    }

    return NULL;
}

elfScript* elfGetOrLoadScriptByName(elfScene* scene, const char* name)
{
    elfScript* script;
    elfPakIndex* index;
    FILE* file;

    for (script = (elfScript*)elfBeginList(scene->scripts); script; script = (elfScript*)elfGetListNext(scene->scripts))
    {
        if (!strcmp(script->name, name))
            return script;
    }

    if (scene->pak)
    {
        index = elfGetPakIndexByName(scene->pak, name, ELF_SCRIPT);
        if (index)
        {
            file = fopen(scene->pak->filePath, "rb");
            fseek(file, elfGetPakIndexOffset(index), SEEK_SET);
            if (feof(file))
                return NULL;

            script = elfCreateScriptFromPak(file, name, scene);
            if (script)
                elfAppendListObject(scene->scripts, (elfObject*)script);
            fclose(file);
            return script;
        }
    }

    return NULL;
}

elfMaterial* elfGetOrLoadMaterialByName(elfScene* scene, const char* name)
{
    elfMaterial* material;
    elfPakIndex* index;
    FILE* file;

    for (material = (elfMaterial*)elfBeginList(scene->materials); material;
         material = (elfMaterial*)elfGetListNext(scene->materials))
    {
        if (!strcmp(material->name, name))
            return material;
    }

    if (scene->pak)
    {
        index = elfGetPakIndexByName(scene->pak, name, ELF_MATERIAL);
        if (index)
        {
            file = fopen(scene->pak->filePath, "rb");
            fseek(file, elfGetPakIndexOffset(index), SEEK_SET);
            if (feof(file))
                return NULL;

            material = elfCreateMaterialFromPak(file, name, scene);
            if (material)
                elfAppendListObject(scene->materials, (elfObject*)material);
            fclose(file);
            return material;
        }
    }

    return NULL;
}

elfCamera* elfGetOrLoadCameraByName(elfScene* scene, const char* name)
{
    elfCamera* camera;
    elfPakIndex* index;
    FILE* file;

    for (camera = (elfCamera*)elfBeginList(scene->cameras); camera; camera = (elfCamera*)elfGetListNext(scene->cameras))
    {
        if (!strcmp(camera->name, name))
            return camera;
    }

    if (scene->pak)
    {
        index = elfGetPakIndexByName(scene->pak, name, ELF_CAMERA);
        if (index)
        {
            file = fopen(scene->pak->filePath, "rb");
            fseek(file, elfGetPakIndexOffset(index), SEEK_SET);
            if (feof(file))
                return NULL;
            camera = elfCreateCameraFromPak(file, name, scene);
            if (camera)
                elfAddSceneCamera(scene, camera);
            fclose(file);
            return camera;
        }
    }

    return NULL;
}

elfEntity* elfGetOrLoadEntityByName(elfScene* scene, const char* name)
{
    elfEntity* entity;
    elfPakIndex* index;
    FILE* file;

    for (entity = (elfEntity*)elfBeginList(scene->entities); entity;
         entity = (elfEntity*)elfGetListNext(scene->entities))
    {
        if (!strcmp(entity->name, name))
            return entity;
    }

    if (scene->pak)
    {
        index = elfGetPakIndexByName(scene->pak, name, ELF_ENTITY);
        if (index)
        {
            file = fopen(scene->pak->filePath, "rb");
            fseek(file, elfGetPakIndexOffset(index), SEEK_SET);
            if (feof(file))
                return NULL;

            entity = elfCreateEntityFromPak(file, name, scene);
            if (entity)
                elfAddSceneEntity(scene, entity);
            fclose(file);
            return entity;
        }
    }

    return NULL;
}

elfLight* elfGetOrLoadLightByName(elfScene* scene, const char* name)
{
    elfLight* light;
    elfPakIndex* index;
    FILE* file;

    for (light = (elfLight*)elfBeginList(scene->lights); light; light = (elfLight*)elfGetListNext(scene->lights))
    {
        if (!strcmp(light->name, name))
            return light;
    }

    if (scene->pak)
    {
        index = elfGetPakIndexByName(scene->pak, name, ELF_LIGHT);
        if (index)
        {
            file = fopen(scene->pak->filePath, "rb");
            fseek(file, elfGetPakIndexOffset(index), SEEK_SET);
            if (feof(file))
                return NULL;

            light = elfCreateLightFromPak(file, name, scene);
            if (light)
                elfAddSceneLight(scene, light);
            fclose(file);
            return light;
        }
    }

    return NULL;
}

elfArmature* elfGetOrLoadArmatureByName(elfScene* scene, const char* name)
{
    elfArmature* armature;
    elfPakIndex* index;
    FILE* file;

    for (armature = (elfArmature*)elfBeginList(scene->armatures); armature;
         armature = (elfArmature*)elfGetListNext(scene->armatures))
    {
        if (!strcmp(armature->name, name))
            return armature;
    }

    if (scene->pak)
    {
        index = elfGetPakIndexByName(scene->pak, name, ELF_ARMATURE);
        if (index)
        {
            file = fopen(scene->pak->filePath, "rb");
            fseek(file, elfGetPakIndexOffset(index), SEEK_SET);
            if (feof(file))
                return NULL;

            armature = elfCreateArmatureFromPak(file, name, scene);
            elfAppendListObject(scene->armatures, (elfObject*)armature);
            fclose(file);
            return armature;
        }
    }

    return NULL;
}

elfParticles* elfGetOrLoadParticlesByName(elfScene* scene, const char* name)
{
    elfParticles* particles;
    elfPakIndex* index;
    FILE* file;

    for (particles = (elfParticles*)elfBeginList(scene->particles); particles;
         particles = (elfParticles*)elfGetListNext(scene->particles))
    {
        if (!strcmp(particles->name, name))
            return particles;
    }

    if (scene->pak)
    {
        index = elfGetPakIndexByName(scene->pak, name, ELF_PARTICLES);
        if (index)
        {
            file = fopen(scene->pak->filePath, "rb");
            fseek(file, elfGetPakIndexOffset(index), SEEK_SET);
            if (feof(file))
                return NULL;

            particles = elfCreateParticlesFromPak(file, name, scene);
            if (particles)
                elfAddSceneParticles(scene, particles);
            fclose(file);
            return particles;
        }
    }

    return NULL;
}

elfSprite* elfGetOrLoadSpriteByName(elfScene* scene, const char* name)
{
    elfSprite* sprite;
    elfPakIndex* index;
    FILE* file;

    for (sprite = (elfSprite*)elfBeginList(scene->sprites); sprite; sprite = (elfSprite*)elfGetListNext(scene->sprites))
    {
        if (!strcmp(sprite->name, name))
            return sprite;
    }

    if (scene->pak)
    {
        index = elfGetPakIndexByName(scene->pak, name, ELF_SPRITE);
        if (index)
        {
            file = fopen(scene->pak->filePath, "rb");
            fseek(file, elfGetPakIndexOffset(index), SEEK_SET);
            if (feof(file))
                return NULL;

            sprite = elfCreateSpriteFromPak(file, name, scene);
            if (sprite)
                elfAddSceneSprite(scene, sprite);
            fclose(file);
            return sprite;
        }
    }

    return NULL;
}

elfActor* elfGetOrLoadActorByName(elfScene* scene, const char* name)
{
    elfActor* actor;

    actor = (elfActor*)elfGetOrLoadCameraByName(scene, name);
    if (actor)
        return actor;
    actor = (elfActor*)elfGetOrLoadEntityByName(scene, name);
    if (actor)
        return actor;
    actor = (elfActor*)elfGetOrLoadLightByName(scene, name);
    if (actor)
        return actor;
    actor = (elfActor*)elfGetOrLoadParticlesByName(scene, name);
    if (actor)
        return actor;

    return NULL;
}

void elfRemoveActor(elfActor* actor)
{
    elfJoint* joint;

    actor->scene = NULL;

    if (actor->object)
    {
        elfRemovePhysicsObjectCollisions(actor->object);
        elfSetPhysicsObjectWorld(actor->object, NULL);
    }
    if (actor->dobject)
    {
        elfRemovePhysicsObjectCollisions(actor->dobject);
        elfSetPhysicsObjectWorld(actor->dobject, NULL);
    }

    for (joint = (elfJoint*)elfBeginList(actor->joints); joint; joint = (elfJoint*)elfGetListNext(actor->joints))
    {
        elfSetJointWorld(joint, NULL);
    }

    elfDecRef((elfObject*)actor->joints);
    actor->joints = elfCreateList();
    elfIncRef((elfObject*)actor->joints);
}

bool elfRemoveSceneCamera(elfScene* scene, const char* name)
{
    elfCamera* cam;

    for (cam = (elfCamera*)elfBeginList(scene->cameras); cam != NULL; cam = (elfCamera*)elfGetListNext(scene->cameras))
    {
        if (!strcmp(cam->name, name))
        {
            elfRemoveActor((elfActor*)cam);
            elfRemoveListObject(scene->cameras, (elfObject*)cam);
            if (scene->curCamera == cam)
                scene->curCamera = elfGetSceneCameraByIndex(scene, 0);
            return true;
        }
    }

    return false;
}

bool elfRemoveSceneEntity(elfScene* scene, const char* name)
{
    elfEntity* ent;

    for (ent = (elfEntity*)elfBeginList(scene->entities); ent != NULL;
         ent = (elfEntity*)elfGetListNext(scene->entities))
    {
        if (!strcmp(ent->name, name))
        {
            elfRemoveActor((elfActor*)ent);
            elfRemoveListObject(scene->entities, (elfObject*)ent);
            return true;
        }
    }

    return false;
}

bool elfRemoveSceneLight(elfScene* scene, const char* name)
{
    elfLight* lig;

    for (lig = (elfLight*)elfBeginList(scene->lights); lig != NULL; lig = (elfLight*)elfGetListNext(scene->lights))
    {
        if (!strcmp(lig->name, name))
        {
            elfRemoveActor((elfActor*)lig);
            elfRemoveListObject(scene->lights, (elfObject*)lig);
            return true;
        }
    }

    return false;
}

bool elfRemoveSceneParticles(elfScene* scene, const char* name)
{
    elfParticles* par;

    for (par = (elfParticles*)elfBeginList(scene->particles); par != NULL;
         par = (elfParticles*)elfGetListNext(scene->particles))
    {
        if (!strcmp(par->name, name))
        {
            elfRemoveActor((elfActor*)par);
            elfRemoveListObject(scene->particles, (elfObject*)par);
            return true;
        }
    }

    return false;
}

bool elfRemoveSceneSprite(elfScene* scene, const char* name)
{
    elfSprite* spr;

    for (spr = (elfSprite*)elfBeginList(scene->sprites); spr != NULL; spr = (elfSprite*)elfGetListNext(scene->sprites))
    {
        if (!strcmp(spr->name, name))
        {
            elfRemoveActor((elfActor*)spr);
            elfRemoveListObject(scene->sprites, (elfObject*)spr);
            return true;
        }
    }

    return false;
}

bool elfRemoveSceneCameraByIndex(elfScene* scene, int idx)
{
    elfCamera* cam;
    int i;

    if (idx < 0 || idx > elfGetListLength(scene->cameras) - 1)
        return false;

    for (i = 0, cam = (elfCamera*)elfBeginList(scene->cameras); cam != NULL;
         cam = (elfCamera*)elfGetListNext(scene->cameras), i++)
    {
        if (i == idx)
        {
            elfRemoveActor((elfActor*)cam);
            elfRemoveListObject(scene->cameras, (elfObject*)cam);
            if (scene->curCamera == cam)
            {
                scene->curCamera = elfGetSceneCameraByIndex(scene, 0);
            }
            return true;
        }
    }

    return false;
}

bool elfRemoveSceneEntityByIndex(elfScene* scene, int idx)
{
    elfEntity* ent;
    int i;

    if (idx < 0 || idx > elfGetListLength(scene->entities) - 1)
        return false;

    for (i = 0, ent = (elfEntity*)elfBeginList(scene->entities); ent != NULL;
         ent = (elfEntity*)elfGetListNext(scene->entities), i++)
    {
        if (i == idx)
        {
            elfRemoveActor((elfActor*)ent);
            elfRemoveListObject(scene->entities, (elfObject*)ent);
            return true;
        }
    }

    return false;
}

bool elfRemoveSceneLightByIndex(elfScene* scene, int idx)
{
    elfLight* lig;
    int i;

    if (idx < 0 || idx > elfGetListLength(scene->lights) - 1)
        return false;

    for (i = 0, lig = (elfLight*)elfBeginList(scene->lights); lig != NULL;
         lig = (elfLight*)elfGetListNext(scene->lights), i++)
    {
        if (i == idx)
        {
            elfRemoveActor((elfActor*)lig);
            elfRemoveListObject(scene->lights, (elfObject*)lig);
            return true;
        }
    }

    return false;
}

bool elfRemoveSceneParticlesByIndex(elfScene* scene, int idx)
{
    elfParticles* par;
    int i;

    if (idx < 0 || idx > elfGetListLength(scene->particles) - 1)
        return false;

    for (i = 0, par = (elfParticles*)elfBeginList(scene->particles); par != NULL;
         par = (elfParticles*)elfGetListNext(scene->particles), i++)
    {
        if (i == idx)
        {
            elfRemoveActor((elfActor*)par);
            elfRemoveListObject(scene->particles, (elfObject*)par);
            return true;
        }
    }

    return false;
}

bool elfRemoveSceneSpriteByIndex(elfScene* scene, int idx)
{
    elfSprite* spr;
    int i;

    if (idx < 0 || idx > elfGetListLength(scene->sprites) - 1)
        return false;

    for (i = 0, spr = (elfSprite*)elfBeginList(scene->sprites); spr != NULL;
         spr = (elfSprite*)elfGetListNext(scene->sprites), i++)
    {
        if (i == idx)
        {
            elfRemoveActor((elfActor*)spr);
            elfRemoveListObject(scene->sprites, (elfObject*)spr);
            return true;
        }
    }

    return false;
}

bool elfRemoveSceneCameraByObject(elfScene* scene, elfCamera* camera)
{
    elfRemoveActor((elfActor*)camera);
    if (scene->curCamera == camera)
        scene->curCamera = elfGetSceneCameraByIndex(scene, 0);
    return elfRemoveListObject(scene->cameras, (elfObject*)camera);
}

bool elfRemoveSceneEntityByObject(elfScene* scene, elfEntity* entity)
{
    elfRemoveActor((elfActor*)entity);
    return elfRemoveListObject(scene->entities, (elfObject*)entity);
}

bool elfRemoveSceneLightByObject(elfScene* scene, elfLight* light)
{
    elfRemoveActor((elfActor*)light);
    return elfRemoveListObject(scene->lights, (elfObject*)light);
}

bool elfRemoveSceneParticlesByObject(elfScene* scene, elfParticles* particles)
{
    elfRemoveActor((elfActor*)particles);
    return elfRemoveListObject(scene->particles, (elfObject*)particles);
}

bool elfRemoveSceneSpriteByObject(elfScene* scene, elfSprite* sprite)
{
    elfRemoveActor((elfActor*)sprite);
    return elfRemoveListObject(scene->sprites, (elfObject*)sprite);
}

bool elfRemoveSceneActorByObject(elfScene* scene, elfActor* actor)
{
    bool result = false;

    // TODO Seriously???
    result = elfRemoveSceneCameraByObject(scene, (elfCamera*)actor);
    if (result)
        return result;
    result = elfRemoveSceneEntityByObject(scene, (elfEntity*)actor);
    if (result)
        return result;
    result = elfRemoveSceneLightByObject(scene, (elfLight*)actor);
    if (result)
        return result;
    result = elfRemoveSceneParticlesByObject(scene, (elfParticles*)actor);
    if (result)
        return result;
    result = elfRemoveSceneSpriteByObject(scene, (elfSprite*)actor);
    if (result)
        return result;

    return false;
}

void elfDrawScene(elfScene* scene)
{
    elfLight* light;
    elfEntity* ent;
    elfSprite* spr;
    elfParticles* par;
    float bias[16] = {0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f};
    float tempMat1[16];
    float tempMat2[16];
    gfxRenderTarget* renderTarget;
    int i;
    elfVec3f lpos;
    elfVec3f spos;
    elfVec3f dvec;
    float dist, att;
    bool found = false;

    if (!scene->curCamera)
        return;

    renderTarget = gfxGetCurRenderTarget();

    if (scene->occlusionCulling)
    {
        // draw occluders to depth buffer
        gfxSetShaderParamsDefault(&scene->shaderParams);
        elfSetCamera(scene->curCamera, &scene->shaderParams);
        scene->shaderParams.renderParams.colorWrite = false;
        scene->shaderParams.renderParams.alphaWrite = false;

        found = false;
        scene->entityQueueCount = 0;
        elfBeginList(scene->entityQueue);

        for (ent = (elfEntity*)elfBeginList(scene->entities); ent != NULL;
             ent = (elfEntity*)elfGetListNext(scene->entities))
        {
            if (!elfCullEntity(ent, scene->curCamera))
            {
                if (scene->entityQueueCount < elfGetListLength(scene->entityQueue))
                {
                    elfSetListCurPtr(scene->entityQueue, (elfObject*)ent);
                    elfGetListNext(scene->entityQueue);
                }
                else
                {
                    elfAppendListObject(scene->entityQueue, (elfObject*)ent);
                }
                scene->entityQueueCount++;
                if (ent->occluder)
                {
                    elfDrawEntity(ent, ELF_DRAW_DEPTH, &scene->shaderParams);
                    found = true;
                }
                ent->culled = false;
            }
            else
            {
                ent->culled = true;
            }
        }

        scene->spriteQueueCount = 0;
        elfBeginList(scene->spriteQueue);

        for (spr = (elfSprite*)elfBeginList(scene->sprites); spr != NULL;
             spr = (elfSprite*)elfGetListNext(scene->sprites))
        {
            if (!elfCullSprite(spr, scene->curCamera))
            {
                if (scene->spriteQueueCount < elfGetListLength(scene->spriteQueue))
                {
                    elfSetListCurPtr(scene->spriteQueue, (elfObject*)spr);
                    elfGetListNext(scene->spriteQueue);
                }
                else
                {
                    elfAppendListObject(scene->spriteQueue, (elfObject*)spr);
                }
                scene->spriteQueueCount++;
                if (spr->occluder)
                {
                    found = true;
                    elfDrawSprite(spr, ELF_DRAW_DEPTH, &scene->shaderParams);
                }
                spr->culled = false;
            }
            else
            {
                spr->culled = true;
            }
        }

        if (found)
        {
            // initiate occlusion queries
            gfxSetShaderParamsDefault(&scene->shaderParams);
            elfSetCamera(scene->curCamera, &scene->shaderParams);
            scene->shaderParams.renderParams.depthWrite = false;
            scene->shaderParams.renderParams.depthFunc = GFX_LEQUAL;
            scene->shaderParams.renderParams.colorWrite = false;
            scene->shaderParams.renderParams.alphaWrite = false;
            scene->shaderParams.renderParams.cullFace = false;

            for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
                 i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
            {
                gfxBeginQuery(ent->query);
                elfDrawEntityBoundingBox(ent, &scene->shaderParams);
                gfxEndQuery(ent->query);
            }

            // draw depth buffer
            gfxSetShaderParamsDefault(&scene->shaderParams);
            elfSetCamera(scene->curCamera, &scene->shaderParams);
            scene->shaderParams.renderParams.colorWrite = false;
            scene->shaderParams.renderParams.alphaWrite = false;

            for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
                 i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
            {
                if (ent->occluder)
                    continue;

                if (gfxGetQueryResult(ent->query) < 1)
                {
                    elfRemoveListObject(scene->entityQueue, (elfObject*)ent);
                    scene->entityQueueCount--;
                    i--;
                }
                else
                {
                    elfDrawEntity(ent, ELF_DRAW_DEPTH, &scene->shaderParams);
                }
            }

            for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue); i < scene->spriteQueueCount && spr != NULL;
                 i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
            {
                if (spr->occluder)
                    continue;

                elfDrawSprite(spr, ELF_DRAW_DEPTH, &scene->shaderParams);
            }
        }
        else
        {
            // draw depth buffer
            gfxSetShaderParamsDefault(&scene->shaderParams);
            elfSetCamera(scene->curCamera, &scene->shaderParams);
            scene->shaderParams.renderParams.colorWrite = false;
            scene->shaderParams.renderParams.alphaWrite = false;

            for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
                 i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
            {
                elfDrawEntity(ent, ELF_DRAW_DEPTH, &scene->shaderParams);
            }

            for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue); i < scene->spriteQueueCount && spr != NULL;
                 i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
            {
                elfDrawSprite(spr, ELF_DRAW_DEPTH, &scene->shaderParams);
            }
        }
    }
    else
    {
        // draw depth buffer
        gfxSetShaderParamsDefault(&scene->shaderParams);
        elfSetCamera(scene->curCamera, &scene->shaderParams);
        scene->shaderParams.renderParams.colorWrite = false;
        scene->shaderParams.renderParams.alphaWrite = false;

        scene->entityQueueCount = 0;
        elfBeginList(scene->entityQueue);

        for (ent = (elfEntity*)elfBeginList(scene->entities); ent != NULL;
             ent = (elfEntity*)elfGetListNext(scene->entities))
        {
            if (!elfCullEntity(ent, scene->curCamera))
            {
                if (scene->entityQueueCount < elfGetListLength(scene->entityQueue))
                {
                    elfSetListCurPtr(scene->entityQueue, (elfObject*)ent);
                    elfGetListNext(scene->entityQueue);
                }
                else
                {
                    elfAppendListObject(scene->entityQueue, (elfObject*)ent);
                }
                scene->entityQueueCount++;
                elfDrawEntity(ent, ELF_DRAW_DEPTH, &scene->shaderParams);
                ent->culled = false;
            }
            else
            {
                ent->culled = true;
            }
        }

        scene->spriteQueueCount = 0;
        elfBeginList(scene->spriteQueue);

        for (spr = (elfSprite*)elfBeginList(scene->sprites); spr != NULL;
             spr = (elfSprite*)elfGetListNext(scene->sprites))
        {
            if (!elfCullSprite(spr, scene->curCamera))
            {
                if (scene->spriteQueueCount < elfGetListLength(scene->spriteQueue))
                {
                    elfSetListCurPtr(scene->spriteQueue, (elfObject*)spr);
                    elfGetListNext(scene->spriteQueue);
                }
                else
                {
                    elfAppendListObject(scene->spriteQueue, (elfObject*)spr);
                }
                scene->spriteQueueCount++;
                elfDrawSprite(spr, ELF_DRAW_DEPTH, &scene->shaderParams);
                spr->culled = false;
            }
            else
            {
                spr->culled = true;
            }
        }
    }

    // draw ambient pass
    if (!elfAboutZero(scene->ambientColor.r) || !elfAboutZero(scene->ambientColor.g) ||
        !elfAboutZero(scene->ambientColor.b))
    {
        gfxSetShaderParamsDefault(&scene->shaderParams);
        elfSetCamera(scene->curCamera, &scene->shaderParams);

        scene->shaderParams.renderParams.depthWrite = false;
        scene->shaderParams.renderParams.depthFunc = GFX_EQUAL;
        scene->shaderParams.renderParams.blendMode = GFX_ADD;

        for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
             i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
        {
            elfDrawEntity(ent, ELF_DRAW_AMBIENT, &scene->shaderParams);
        }

        for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue); i < scene->spriteQueueCount && spr != NULL;
             i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
        {
            elfDrawSprite(spr, ELF_DRAW_AMBIENT, &scene->shaderParams);
        }
    }

    // draw non lighted stuff
    gfxSetShaderParamsDefault(&scene->shaderParams);
    elfSetCamera(scene->curCamera, &scene->shaderParams);

    scene->shaderParams.renderParams.depthWrite = false;
    scene->shaderParams.renderParams.depthFunc = GFX_EQUAL;
    scene->shaderParams.renderParams.blendMode = GFX_ADD;

    for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
         i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
    {
        elfDrawEntity(ent, ELF_DRAW_WITHOUT_LIGHTING, &scene->shaderParams);
    }

    for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue); i < scene->spriteQueueCount && spr != NULL;
         i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
    {
        elfDrawSprite(spr, ELF_DRAW_WITHOUT_LIGHTING, &scene->shaderParams);
    }

    // render lighting
    for (light = (elfLight*)elfBeginList(scene->lights); light != NULL;
         light = (elfLight*)elfGetListNext(scene->lights))
    {
        if (!light->visible)
            continue;

        if (light->lightType == ELF_SPOT_LIGHT)
        {
            elfSetCameraViewport(light->shadowCamera, 0, 0, elfGetShadowMapSize(), elfGetShadowMapSize());
            // need to call this for frustum culling...
            elfSetCamera(light->shadowCamera, &scene->shaderParams);

            // check are there any entities visible for the spot, if there aren't don't bother continuing, just skip to
            // the next light
            found = false;
            for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
                 i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
            {
                if (!elfCullEntity(ent, light->shadowCamera))
                {
                    found = true;
                    break;
                }
            }

            for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue); i < scene->spriteQueueCount && spr != NULL;
                 i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
            {
                if (!elfCullSprite(spr, light->shadowCamera))
                {
                    found = true;
                    break;
                }
            }

            if (!found)
                continue;
        }

        // render shadow map if needed
        if (light->lightType == ELF_SPOT_LIGHT && light->shadows && gfxGetVersion() >= 200)
        {
            gfxSetShaderParamsDefault(&scene->shaderParams);
            scene->shaderParams.renderParams.colorWrite = false;
            scene->shaderParams.renderParams.alphaWrite = false;
            scene->shaderParams.renderParams.offsetBias = 2.0f;
            scene->shaderParams.renderParams.offsetScale = 4.0f;
            elfSetCamera(light->shadowCamera, &scene->shaderParams);
            gfxSetShaderParams(&scene->shaderParams);

            gfxSetRenderTarget(rnd->shadowTarget);
            gfxClearDepthBuffer(1.0f);

            for (ent = (elfEntity*)elfBeginList(scene->entities); ent != NULL;
                 ent = (elfEntity*)elfGetListNext(scene->entities))
            {
                if (!elfCullEntity(ent, light->shadowCamera))
                {
                    elfDrawEntity(ent, ELF_DRAW_DEPTH, &scene->shaderParams);
                }
            }

            for (spr = (elfSprite*)elfBeginList(scene->sprites); spr != NULL;
                 spr = (elfSprite*)elfGetListNext(scene->sprites))
            {
                if (!elfCullSprite(spr, light->shadowCamera))
                {
                    elfDrawSprite(spr, ELF_DRAW_DEPTH, &scene->shaderParams);
                }
            }

            gfxMulMatrix4Matrix4(elfGetCameraProjectionMatrix(light->shadowCamera), bias, tempMat1);
            gfxMulMatrix4Matrix4(elfGetCameraModelviewMatrix(light->shadowCamera), tempMat1, tempMat2);
            gfxMatrix4GetInverse(elfGetCameraModelviewMatrix(scene->curCamera), tempMat1);
            gfxMulMatrix4Matrix4(tempMat1, tempMat2, light->projectionMatrix);

            if (renderTarget)
                gfxSetRenderTarget(renderTarget);
            else
                gfxDisableRenderTarget();
        }

        // render lighting
        gfxSetShaderParamsDefault(&scene->shaderParams);
        scene->shaderParams.renderParams.depthWrite = false;
        scene->shaderParams.renderParams.depthFunc = GFX_EQUAL;
        scene->shaderParams.renderParams.colorWrite = true;
        scene->shaderParams.renderParams.alphaWrite = true;
        scene->shaderParams.renderParams.blendMode = GFX_ADD;
        elfSetCamera(scene->curCamera, &scene->shaderParams);

        // Spot light with shadows
        // TODO Shadows are disabled, requires shadow mapping to be reimplemented for modern opengl
        light->shadows = false;
        if (light->lightType == ELF_SPOT_LIGHT && light->shadows)
        {
            scene->shaderParams.textureParams[GFX_MAX_TEXTURES - 1].type = GFX_SHADOW_MAP;
            scene->shaderParams.textureParams[GFX_MAX_TEXTURES - 1].texture = rnd->shadowMap;
            scene->shaderParams.textureParams[GFX_MAX_TEXTURES - 1].projectionMode = GFX_SHADOW_PROJECTION;
            memcpy(scene->shaderParams.textureParams[GFX_MAX_TEXTURES - 1].matrix, light->projectionMatrix,
                   sizeof(float) * 16);
        }
        else
        {
            scene->shaderParams.textureParams[GFX_MAX_TEXTURES - 1].type = GFX_COLOR_MAP;
            scene->shaderParams.textureParams[GFX_MAX_TEXTURES - 1].texture = NULL;
            scene->shaderParams.textureParams[GFX_MAX_TEXTURES - 1].projectionMode = GFX_PROJECTION_NONE;
            gfxMatrix4SetIdentity(scene->shaderParams.textureParams[GFX_MAX_TEXTURES - 1].matrix);
        }

        elfSetLight(light, scene->curCamera, &scene->shaderParams);

        // get the light position for culling point light entities
        lpos = elfGetActorPosition((elfActor*)light);
        for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
             i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
        {
            if (light->lightType == ELF_SPOT_LIGHT)
            {
                if (!elfCullEntity(ent, light->shadowCamera))
                {
                    elfDrawEntity(ent, ELF_DRAW_WITH_LIGHTING, &scene->shaderParams);
                }
            }
            else if (light->lightType == ELF_POINT_LIGHT)
            {
                if (gfxBoxSphereIntersect(&ent->cullAabbMin.x, &ent->cullAabbMax.x, &lpos.x,
                                          light->range + light->fadeRange))
                {
                    elfDrawEntity(ent, ELF_DRAW_WITH_LIGHTING, &scene->shaderParams);
                }
            }
            else
            {
                elfDrawEntity(ent, ELF_DRAW_WITH_LIGHTING, &scene->shaderParams);
            }
        }

        for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue); i < scene->spriteQueueCount && spr != NULL;
             i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
        {
            spos = elfGetActorPosition((elfActor*)spr);
            if (light->lightType == ELF_SPOT_LIGHT)
            {
                if (!elfCullSprite(spr, light->shadowCamera))
                {
                    elfDrawSprite(spr, ELF_DRAW_WITH_LIGHTING, &scene->shaderParams);
                }
            }
            else if (light->lightType == ELF_POINT_LIGHT)
            {
                dvec = elfSubVec3fVec3f(spos, lpos);
                dist = elfGetVec3fLength(dvec);
                dist -= spr->cullRadius;
                att = 1.0 - elfFloatMax(dist - light->range, 0.0f) / light->fadeRange;
                if (att > 0.0f)
                {
                    elfDrawSprite(spr, ELF_DRAW_WITH_LIGHTING, &scene->shaderParams);
                }
            }
            else
            {
                elfDrawSprite(spr, ELF_DRAW_WITH_LIGHTING, &scene->shaderParams);
            }
        }
    }

    if (scene->fog && gfxGetVersion() >= 200)
    {
        gfxSetShaderParamsDefault(&scene->shaderParams);
        scene->shaderParams.renderParams.depthWrite = false;
        scene->shaderParams.renderParams.depthFunc = GFX_EQUAL;
        scene->shaderParams.renderParams.colorWrite = true;
        scene->shaderParams.renderParams.alphaWrite = true;

        scene->shaderParams.fogParams.mode = GFX_FOG_LINEAR;
        scene->shaderParams.fogParams.start = scene->fogStart;
        scene->shaderParams.fogParams.end = scene->fogEnd;
        memcpy(&scene->shaderParams.fogParams.color.r, &scene->fogColor.r, sizeof(float) * 4);

        scene->shaderParams.shaderProgram = scene->composeFogShdr;
        scene->shaderParams.renderParams.blendMode = GFX_TRANSPARENT;

        elfSetCamera(scene->curCamera, &scene->shaderParams);

        for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
             i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
        {
            elfDrawEntity(ent, ELF_DRAW_AMBIENT, &scene->shaderParams);
        }

        for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue); i < scene->spriteQueueCount && spr != NULL;
             i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
        {
            elfDrawSprite(spr, ELF_DRAW_AMBIENT, &scene->shaderParams);
        }
    }

    // render particles
    gfxSetShaderParamsDefault(&scene->shaderParams);
    scene->shaderParams.renderParams.depthWrite = false;
    scene->shaderParams.renderParams.depthFunc = GFX_LEQUAL;
    scene->shaderParams.renderParams.colorWrite = true;
    scene->shaderParams.renderParams.alphaWrite = true;
    if (scene->fog)
    {
        scene->shaderParams.fogParams.mode = GFX_FOG_LINEAR;
        scene->shaderParams.fogParams.start = scene->fogStart;
        scene->shaderParams.fogParams.end = scene->fogEnd;
        memcpy(&scene->shaderParams.fogParams.color.r, &scene->fogColor.r, sizeof(float) * 4);
    }
    elfSetCamera(scene->curCamera, &scene->shaderParams);

    for (par = (elfParticles*)elfBeginList(scene->particles); par;
         par = (elfParticles*)elfGetListNext(scene->particles))
    {
        if (!elfCullParticles(par, scene->curCamera))
        {
            elfDrawParticles(par, scene->curCamera, &scene->shaderParams);
        }
    }

    // reset state just to be sure...
    gfxSetShaderParamsDefault(&scene->shaderParams);
    gfxSetShaderParams(&scene->shaderParams);

    // keep the query lists compact

    if (elfGetListLength(scene->entityQueue) > scene->entityQueueCount)
    {
        ent = (elfEntity*)elfRBeginList(scene->entityQueue);
        if (ent)
            elfRemoveListObject(scene->entityQueue, (elfObject*)ent);
    }

    if (elfGetListLength(scene->spriteQueue) > scene->spriteQueueCount)
    {
        spr = (elfSprite*)elfRBeginList(scene->spriteQueue);
        if (spr)
            elfRemoveListObject(scene->spriteQueue, (elfObject*)spr);
    }
}

// TODO Was commented out, was called eldDrawScene (same as above)
//      Looks like this was the deferred drawing method
/*
void elfDrawSceneDeferred(elfScene* scene)
{
    elfEntity* ent;
    elfSprite* spr;
    elfLight* lig;
    elfParticles* par;
    float bias[16] = {0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0};
    float tempMat1[16];
    float tempMat2[16];
    float invProjectionMatrix[16];
    bool found;
    int i;
    elfVec4f camOrient;
    elfVec3f lpos;
    float ldist;
    elfVec3f ldvec;
    elfVec3f lsspos;
    elfVec3f ldsspos;
    elfVec2f lmin;
    elfVec2f lmax;
    int wwidth;
    int wheight;

    if (!scene->curCamera)
        return;

    gfxBindGbuffer(eng->gbuffer, &scene->shaderParams);
    gfxClearBuffers(0.0, 0.0, 0.0, 0.0, 1.0);

    if (scene->occlusionCulling)
    {
        // draw occluders to depth buffer
        gfxSetShaderParamsDefault(&scene->shaderParams);
        elfSetCamera(scene->curCamera, &scene->shaderParams);
        scene->shaderParams.renderParams.colorWrite = ELF_FALSE;
        scene->shaderParams.renderParams.alphaWrite = ELF_FALSE;
        scene->shaderParams.gbuffer = eng->gbuffer;
        scene->shaderParams.gbufferMode = GFX_GBUFFER_DEPTH;

        found = ELF_FALSE;
        scene->entityQueueCount = 0;
        elfBeginList(scene->entityQueue);

        for (ent = (elfEntity*)elfBeginList(scene->entities); ent != NULL;
             ent = (elfEntity*)elfGetListNext(scene->entities))
        {
            if (!elfCullEntity(ent, scene->curCamera))
            {
                if (scene->entityQueueCount < elfGetListLength(scene->entityQueue))
                {
                    elfSetListCurPtr(scene->entityQueue, (elfObject*)ent);
                    elfGetListNext(scene->entityQueue);
                }
                else
                {
                    elfAppendListObject(scene->entityQueue, (elfObject*)ent);
                }
                scene->entityQueueCount++;
                if (ent->occluder)
                {
                    elfDrawEntity(ent, ELF_DRAW_DEPTH, &scene->shaderParams);
                    found = true;
                }
                ent->culled = ELF_FALSE;
            }
            else
            {
                ent->culled = true;
            }
        }

        scene->spriteQueueCount = 0;
        elfBeginList(scene->spriteQueue);

        for (spr = (elfSprite*)elfBeginList(scene->sprites); spr != NULL;
             spr = (elfSprite*)elfGetListNext(scene->sprites))
        {
            if (!elfCullSprite(spr, scene->curCamera))
            {
                if (scene->spriteQueueCount < elfGetListLength(scene->spriteQueue))
                {
                    elfSetListCurPtr(scene->spriteQueue, (elfObject*)spr);
                    elfGetListNext(scene->spriteQueue);
                }
                else
                {
                    elfAppendListObject(scene->spriteQueue, (elfObject*)spr);
                }
                scene->spriteQueueCount++;
                if (spr->occluder)
                {
                    found = true;
                    elfDrawSprite(spr, ELF_DRAW_DEPTH, &scene->shaderParams);
                }
                spr->culled = ELF_FALSE;
            }
            else
            {
                spr->culled = true;
            }
        }

        if (found)
        {
            // initiate occlusion queries
            gfxSetShaderParamsDefault(&scene->shaderParams);
            elfSetCamera(scene->curCamera, &scene->shaderParams);
            scene->shaderParams.renderParams.depthWrite = false;
            scene->shaderParams.renderParams.depthFunc = GFX_LEQUAL;
            scene->shaderParams.renderParams.colorWrite = GFX_FALSE;
            scene->shaderParams.renderParams.alphaWrite = GFX_FALSE;
            scene->shaderParams.renderParams.cullFace = GFX_FALSE;

            for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
                 i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
            {
                gfxBeginQuery(ent->query);
                elfDrawEntityBoundingBox(ent, &scene->shaderParams);
                gfxEndQuery(ent->query);
            }

            // draw depth buffer
            gfxSetShaderParamsDefault(&scene->shaderParams);
            elfSetCamera(scene->curCamera, &scene->shaderParams);
            scene->shaderParams.renderParams.colorWrite = ELF_FALSE;
            scene->shaderParams.renderParams.alphaWrite = ELF_FALSE;
            scene->shaderParams.gbuffer = eng->gbuffer;
            scene->shaderParams.gbufferMode = GFX_GBUFFER_DEPTH;

            for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
                 i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
            {
                if (ent->occluder)
                    continue;

                if (gfxGetQueryResult(ent->query) < 1)
                {
                    elfRemoveListObject(scene->entityQueue, (elfObject*)ent);
                    scene->entityQueueCount--;
                    i--;
                }
                else
                {
                    elfDrawEntity(ent, ELF_DRAW_DEPTH, &scene->shaderParams);
                }
            }

            for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue); i < scene->spriteQueueCount && spr != NULL;
                 i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
            {
                if (spr->occluder)
                    continue;

                elfDrawSprite(spr, ELF_DRAW_DEPTH, &scene->shaderParams);
            }
        }
        else
        {
            // draw depth buffer
            gfxSetShaderParamsDefault(&scene->shaderParams);
            elfSetCamera(scene->curCamera, &scene->shaderParams);
            scene->shaderParams.renderParams.colorWrite = ELF_FALSE;
            scene->shaderParams.renderParams.alphaWrite = ELF_FALSE;
            scene->shaderParams.gbuffer = eng->gbuffer;
            scene->shaderParams.gbufferMode = GFX_GBUFFER_DEPTH;

            for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
                 i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
            {
                elfDrawEntity(ent, ELF_DRAW_DEPTH, &scene->shaderParams);
            }

            for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue); i < scene->spriteQueueCount && spr != NULL;
                 i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
            {
                elfDrawSprite(spr, ELF_DRAW_DEPTH, &scene->shaderParams);
            }
        }
    }
    else
    {
        // draw depth buffer
        gfxSetShaderParamsDefault(&scene->shaderParams);
        elfSetCamera(scene->curCamera, &scene->shaderParams);
        scene->shaderParams.renderParams.colorWrite = ELF_FALSE;
        scene->shaderParams.renderParams.alphaWrite = ELF_FALSE;
        scene->shaderParams.gbuffer = eng->gbuffer;
        scene->shaderParams.gbufferMode = GFX_GBUFFER_DEPTH;

        scene->entityQueueCount = 0;
        elfBeginList(scene->entityQueue);

        for (ent = (elfEntity*)elfBeginList(scene->entities); ent != NULL;
             ent = (elfEntity*)elfGetListNext(scene->entities))
        {
            if (!elfCullEntity(ent, scene->curCamera))
            {
                if (scene->entityQueueCount < elfGetListLength(scene->entityQueue))
                {
                    elfSetListCurPtr(scene->entityQueue, (elfObject*)ent);
                    elfGetListNext(scene->entityQueue);
                }
                else
                {
                    elfAppendListObject(scene->entityQueue, (elfObject*)ent);
                }
                scene->entityQueueCount++;
                elfDrawEntity(ent, ELF_DRAW_DEPTH, &scene->shaderParams);
                ent->culled = ELF_FALSE;
            }
            else
            {
                ent->culled = ELF_TRUE;
            }
        }

        scene->spriteQueueCount = 0;
        elfBeginList(scene->spriteQueue);

        for (spr = (elfSprite*)elfBeginList(scene->sprites); spr != NULL;
             spr = (elfSprite*)elfGetListNext(scene->sprites))
        {
            if (!elfCullSprite(spr, scene->curCamera))
            {
                if (scene->spriteQueueCount < elfGetListLength(scene->spriteQueue))
                {
                    elfSetListCurPtr(scene->spriteQueue, (elfObject*)spr);
                    elfGetListNext(scene->spriteQueue);
                }
                else
                {
                    elfAppendListObject(scene->spriteQueue, (elfObject*)spr);
                }
                scene->spriteQueueCount++;
                elfDrawSprite(spr, ELF_DRAW_DEPTH, &scene->shaderParams);
                spr->culled = ELF_FALSE;
            }
            else
            {
                spr->culled = ELF_TRUE;
            }
        }
    }

    // draw gbuffers
    gfxSetShaderParamsDefault(&scene->shaderParams);
    elfSetCamera(scene->curCamera, &scene->shaderParams);
    scene->shaderParams.renderParams.depthWrite = GFX_FALSE;
    scene->shaderParams.renderParams.depthFunc = GFX_EQUAL;
    scene->shaderParams.gbuffer = eng->gbuffer;
    scene->shaderParams.gbufferMode = GFX_GBUFFER_FILL;

    for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
         i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
    {
        elfDrawEntity(ent, ELF_DRAW_WITH_LIGHTING, &scene->shaderParams);
    }

    for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue); i < scene->spriteQueueCount && spr != NULL;
         i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
    {
        elfDrawSprite(spr, ELF_DRAW_WITH_LIGHTING, &scene->shaderParams);
    }

    // get inverse projection matrix for retrieving vertex position from depth
    gfxMatrix4GetInverse(scene->curCamera->projectionMatrix, invProjectionMatrix);

    // draw lighting
    gfxBindGbufferLight(eng->gbuffer, &scene->shaderParams);
    gfxClearColorBuffer(0.0, 0.0, 0.0, 1.0);

    camOrient = elfGetActorOrientation((elfActor*)scene->curCamera);
    wwidth = elfGetWindowWidth();
    wheight = elfGetWindowHeight();

    for (lig = (elfLight*)elfBeginList(scene->lights); lig; lig = (elfLight*)elfGetListNext(scene->lights))
    {
        if (!lig->visible)
            continue;

        // cull lights that aren't affecting any pixels on the screen
        lpos = elfGetActorPosition((elfActor*)lig);
        ldist = lig->range + 1.0 / lig->fadeSpeed;

        ldvec.x = 0.0;
        ldvec.y = ldist;
        ldvec.z = 0.0;
        ldvec = elfMulQuaVec3f(camOrient, ldvec);
        ldvec = elfAddVec3fVec3f(lpos, ldvec);

        lsspos = elfProjectCameraPoint(scene->curCamera, lpos.x, lpos.y, lpos.z);
        ldsspos = elfProjectCameraPoint(scene->curCamera, ldvec.x, ldvec.y, ldvec.z);

        ldvec = elfSubVec3fVec3f(ldsspos, lsspos);
        ldist = elfGetVec3fLength(ldvec);

        lmin.x = lsspos.x - ldist;
        lmin.y = lsspos.y - ldist;
        lmax.x = lsspos.x + ldist;
        lmax.y = lsspos.y + ldist;

        if (lmin.x < 0)
            lmin.x = 0;
        if (lmin.y < 0)
            lmin.y = 0;
        if (lmin.x > wwidth)
            lmin.x = wwidth;
        if (lmin.y > wheight)
            lmin.y = wheight;
        if (lmax.x < 0)
            lmax.x = 0;
        if (lmax.y < 0)
            lmax.y = 0;
        if (lmax.x > wwidth)
            lmax.x = wwidth;
        if (lmax.y > wheight)
            lmax.y = wheight;

        if (lmax.x - lmin.x == 0)
            continue;
        if (lmax.y - lmin.y == 0)
            continue;

        // need to call this for frustum culling...
        if (lig->lightType == ELF_SPOT_LIGHT)
        {
            elfSetCameraViewport(lig->shadowCamera, 0, 0, elfGetShadowMapSize(), elfGetShadowMapSize());
            elfSetCamera(lig->shadowCamera, &scene->shaderParams);

            // check are there any entities visible for the spot, if there aren't don't bother continuing,
            // just skip to the next light
            found = ELF_FALSE;
            for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
                 i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
            {
                if (!elfCullEntity(ent, lig->shadowCamera))
                {
                    found = ELF_TRUE;
                    break;
                }
            }

            for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue); i < scene->spriteQueueCount && spr != NULL;
                 i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
            {
                if (!elfCullSprite(spr, lig->shadowCamera))
                {
                    found = ELF_TRUE;
                    break;
                }
            }

            if (!found)
                continue;

            // render shadow map if needed
            if (lig->shadows)
            {
                gfxDisableScissor();

                gfxSetShaderParamsDefault(&scene->shaderParams);
                scene->shaderParams.renderParams.colorWrite = GFX_FALSE;
                scene->shaderParams.renderParams.alphaWrite = GFX_FALSE;
                scene->shaderParams.renderParams.offsetBias = 2.0;
                scene->shaderParams.renderParams.offsetScale = 4.0;
                elfSetCamera(lig->shadowCamera, &scene->shaderParams);
                gfxSetShaderParams(&scene->shaderParams);

                gfxSetRenderTarget(rnd->shadowTarget);
                gfxClearDepthBuffer(1.0);

                for (ent = (elfEntity*)elfBeginList(scene->entities); ent != NULL;
                     ent = (elfEntity*)elfGetListNext(scene->entities))
                {
                    if (!elfCullEntity(ent, lig->shadowCamera))
                    {
                        elfDrawEntity(ent, ELF_DRAW_DEPTH, &scene->shaderParams);
                    }
                }

                for (spr = (elfSprite*)elfBeginList(scene->sprites); spr != NULL;
                     spr = (elfSprite*)elfGetListNext(scene->sprites))
                {
                    if (!elfCullSprite(spr, lig->shadowCamera))
                    {
                        elfDrawSprite(spr, ELF_DRAW_DEPTH, &scene->shaderParams);
                    }
                }

                gfxMulMatrix4Matrix4(elfGetCameraProjectionMatrix(lig->shadowCamera), bias, tempMat1);
                gfxMulMatrix4Matrix4(elfGetCameraModelviewMatrix(lig->shadowCamera), tempMat1, tempMat2);
                gfxMatrix4GetInverseFast(elfGetCameraModelviewMatrix(scene->curCamera), tempMat1);
                gfxMulMatrix4Matrix4(tempMat1, tempMat2, lig->projectionMatrix);
            }
        }

        gfxSetScissor((int)lmin.x, (int)lmin.y, (int)(lmax.x - lmin.x), (int)(lmax.y - lmin.y));

        gfxBindGbufferLight(eng->gbuffer, &scene->shaderParams);

        gfxSetShaderParamsDefault(&scene->shaderParams);

        if (lig->lightType == ELF_SPOT_LIGHT && lig->shadows)
        {
            scene->shaderParams.textureParams[GFX_MAX_TEXTURES - 1].type = GFX_SHADOW_MAP;
            scene->shaderParams.textureParams[GFX_MAX_TEXTURES - 1].texture = rnd->shadowMap;
            scene->shaderParams.textureParams[GFX_MAX_TEXTURES - 1].projectionMode = GFX_SHADOW_PROJECTION;
            memcpy(scene->shaderParams.textureParams[GFX_MAX_TEXTURES - 1].matrix, lig->projectionMatrix,
                   sizeof(float) * 16);
        }

        gfxSetViewport(0, 0, elfGetWindowWidth(), elfGetWindowHeight());
        gfxGetOrthographicProjectionMatrix(0.0, (float)elfGetWindowWidth(), 0.0, (float)elfGetWindowHeight(), -1.0, 1.0,
                                           scene->shaderParams.projectionMatrix);
        scene->shaderParams.renderParams.depthTest = GFX_FALSE;
        scene->shaderParams.renderParams.depthWrite = GFX_FALSE;
        scene->shaderParams.renderParams.blendMode = GFX_ADD;
        scene->shaderParams.gbufferMode = GFX_GBUFFER_LIGHTING;
        memcpy(scene->shaderParams.invProjectionMatrix, invProjectionMatrix, sizeof(float) * 16);
        scene->shaderParams.textureParams[0].texture = gfxGetGbufferDepth(eng->gbuffer);
        scene->shaderParams.textureParams[1].texture = gfxGetGbufferBuf1(eng->gbuffer);
        scene->shaderParams.textureParams[2].texture = gfxGetGbufferBuf3(eng->gbuffer);

        elfSetLight(lig, scene->curCamera, &scene->shaderParams);

        gfxSetShaderParams(&scene->shaderParams);

        gfxDrawTextured_2dQuad(0.0, 0.0, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());
    }

    gfxDisableScissor();

    gfxBindGbufferMain(eng->gbuffer, &scene->shaderParams);

    // combine lighting
    gfxSetShaderParamsDefault(&scene->shaderParams);
    scene->shaderParams.renderParams.depthTest = GFX_FALSE;
    scene->shaderParams.renderParams.depthWrite = GFX_FALSE;
    scene->shaderParams.shaderProgram = scene->composeMainShdr;
    scene->shaderParams.textureParams[0].texture = gfxGetGbufferBuf2(eng->gbuffer);
    scene->shaderParams.textureParams[1].texture = gfxGetGbufferDiffuse(eng->gbuffer);
    scene->shaderParams.textureParams[2].texture = gfxGetGbufferSpecular(eng->gbuffer);

    gfxSetViewport(0, 0, elfGetWindowWidth(), elfGetWindowHeight());
    gfxGetOrthographicProjectionMatrix(0.0, (float)elfGetWindowWidth(), 0.0, (float)elfGetWindowHeight(), -1.0, 1.0,
                                       scene->shaderParams.projectionMatrix);

    gfxSetShaderParams(&scene->shaderParams);

    gfxDrawTextured_2dQuad(0.0, 0.0, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());

    // draw ambient
    if (!elfAboutZero(scene->ambientColor.r) || !elfAboutZero(scene->ambientColor.g) ||
        !elfAboutZero(scene->ambientColor.b))
    {
        scene->shaderParams.shaderProgram = NULL;
        scene->shaderParams.renderParams.blendMode = GFX_ADD;
        scene->shaderParams.textureParams[0].texture = gfxGetGbufferBuf2(eng->gbuffer);
        scene->shaderParams.textureParams[1].texture = NULL;
        scene->shaderParams.textureParams[2].texture = NULL;
        memcpy(&scene->shaderParams.materialParams.color.r, &scene->ambientColor.r, sizeof(float) * 4);

        gfxSetShaderParams(&scene->shaderParams);

        gfxDrawTextured_2dQuad(0.0, 0.0, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());
    }

    // draw non lighted stuff
    gfxSetShaderParamsDefault(&scene->shaderParams);
    elfSetCamera(scene->curCamera, &scene->shaderParams);
    scene->shaderParams.renderParams.depthWrite = GFX_FALSE;
    scene->shaderParams.renderParams.depthFunc = GFX_EQUAL;

    for (i = 0, ent = (elfEntity*)elfBeginList(scene->entityQueue); i < scene->entityQueueCount && ent != NULL;
         i++, ent = (elfEntity*)elfGetListNext(scene->entityQueue))
    {
        elfDrawEntity(ent, ELF_DRAW_WITHOUT_LIGHTING, &scene->shaderParams);
    }

    for (i = 0, spr = (elfSprite*)elfBeginList(scene->spriteQueue); i < scene->spriteQueueCount && spr != NULL;
         i++, spr = (elfSprite*)elfGetListNext(scene->spriteQueue))
    {
        elfDrawSprite(spr, ELF_DRAW_WITHOUT_LIGHTING, &scene->shaderParams);
    }

    // render particles
    gfxSetShaderParamsDefault(&scene->shaderParams);
    scene->shaderParams.renderParams.depthWrite = GFX_FALSE;
    scene->shaderParams.renderParams.depthFunc = GFX_LEQUAL;
    scene->shaderParams.renderParams.colorWrite = GFX_TRUE;
    scene->shaderParams.renderParams.alphaWrite = GFX_TRUE;
    elfSetCamera(scene->curCamera, &scene->shaderParams);

    for (par = (elfParticles*)elfBeginList(scene->particles); par;
         par = (elfParticles*)elfGetListNext(scene->particles))
    {
        if (!elfCullParticles(par, scene->curCamera))
        {
            elfDrawParticles(par, scene->curCamera, &scene->shaderParams);
        }
    }

    gfxDisableRenderTarget();

    if (!eng->postProcess)
    {
        // draw final image
        gfxSetShaderParamsDefault(&scene->shaderParams);
        scene->shaderParams.renderParams.depthTest = GFX_FALSE;
        scene->shaderParams.renderParams.depthWrite = GFX_FALSE;
        scene->shaderParams.textureParams[0].texture = gfxGetGbufferMain(eng->gbuffer);

        gfxSetViewport(0, 0, elfGetWindowWidth(), elfGetWindowHeight());
        gfxGetOrthographicProjectionMatrix(0.0, (float)elfGetWindowWidth(), 0.0, (float)elfGetWindowHeight(), -1.0, 1.0,
                                           scene->shaderParams.projectionMatrix);

        gfxSetShaderParams(&scene->shaderParams);

        gfxDrawTextured_2dQuad(0.0, 0.0, (float)elfGetWindowWidth(), (float)elfGetWindowHeight());
    }

    // reset state just to be sure...
    gfxSetShaderParamsDefault(&scene->shaderParams);
    gfxSetShaderParams(&scene->shaderParams);

    // keep the query lists compact

    if (elfGetListLength(scene->entityQueue) > scene->entityQueueCount)
    {
        ent = (elfEntity*)elfRBeginList(scene->entityQueue);
        if (ent)
            elfRemoveListObject(scene->entityQueue, (elfObject*)ent);
    }

    if (elfGetListLength(scene->spriteQueue) > scene->spriteQueueCount)
    {
        spr = (elfSprite*)elfRBeginList(scene->spriteQueue);
        if (spr)
            elfRemoveListObject(scene->spriteQueue, (elfObject*)spr);
    }
}
*/

void elfDrawSceneDebug(elfScene* scene)
{
    elfEntity* ent;
    elfLight* lig;
    elfCamera* cam;
    elfParticles* par;
    elfSprite* spr;

    if (!scene->curCamera)
        return;

    gfxSetShaderParamsDefault(&scene->shaderParams);
    scene->shaderParams.renderParams.depthWrite = false;
    scene->shaderParams.renderParams.depthTest = false;
    scene->shaderParams.renderParams.blendMode = GFX_ADD;
    elfSetCamera(scene->curCamera, &scene->shaderParams);

    for (ent = (elfEntity*)elfBeginList(scene->entities); ent != NULL;
         ent = (elfEntity*)elfGetListNext(scene->entities))
    {
        elfDrawEntityDebug(ent, &scene->shaderParams);
    }

    gfxSetShaderParamsDefault(&scene->shaderParams);
    scene->shaderParams.renderParams.depthWrite = false;
    scene->shaderParams.renderParams.depthTest = false;
    elfSetCamera(scene->curCamera, &scene->shaderParams);

    for (par = (elfParticles*)elfBeginList(scene->particles); par != NULL;
         par = (elfParticles*)elfGetListNext(scene->particles))
    {
        elfDrawParticlesDebug(par, &scene->shaderParams);
    }

    gfxSetShaderParamsDefault(&scene->shaderParams);
    scene->shaderParams.renderParams.depthWrite = false;
    scene->shaderParams.renderParams.depthTest = false;
    elfSetCamera(scene->curCamera, &scene->shaderParams);

    for (spr = (elfSprite*)elfBeginList(scene->sprites); spr != NULL; spr = (elfSprite*)elfGetListNext(scene->sprites))
    {
        elfDrawSpriteDebug(spr, &scene->shaderParams);
    }

    gfxSetShaderParamsDefault(&scene->shaderParams);
    scene->shaderParams.renderParams.depthWrite = false;
    scene->shaderParams.renderParams.depthTest = false;
    elfSetCamera(scene->curCamera, &scene->shaderParams);

    for (lig = (elfLight*)elfBeginList(scene->lights); lig != NULL; lig = (elfLight*)elfGetListNext(scene->lights))
    {
        elfDrawLightDebug(lig, &scene->shaderParams);
    }

    gfxSetShaderParamsDefault(&scene->shaderParams);
    scene->shaderParams.renderParams.depthWrite = false;
    scene->shaderParams.renderParams.depthTest = false;
    elfSetCamera(scene->curCamera, &scene->shaderParams);

    for (cam = (elfCamera*)elfBeginList(scene->cameras); cam != NULL; cam = (elfCamera*)elfGetListNext(scene->cameras))
    {
        if (cam == scene->curCamera)
            continue;
        elfDrawCameraDebug(cam, &scene->shaderParams);
    }

    // reset state just to be sure...
    gfxSetShaderParamsDefault(&scene->shaderParams);
    gfxSetShaderParams(&scene->shaderParams);
}

elfList* elfGetSceneScripts(elfScene* scene)
{
    elfList* scripts;

    elfCamera* cam;
    elfEntity* ent;
    elfLight* lig;
    elfParticles* par;
    elfSprite* spr;

    scripts = elfCreateList();

    for (cam = (elfCamera*)elfBeginList(scene->cameras); cam; cam = (elfCamera*)elfGetListNext(scene->cameras))
    {
        if (cam->script && !elfGetResourceById(scripts, cam->script->id))
        {
            elfAppendListObject(scripts, (elfObject*)cam->script);
        }
    }

    for (ent = (elfEntity*)elfBeginList(scene->entities); ent; ent = (elfEntity*)elfGetListNext(scene->entities))
    {
        if (ent->script && !elfGetResourceById(scripts, ent->script->id))
        {
            elfAppendListObject(scripts, (elfObject*)ent->script);
        }
    }

    for (lig = (elfLight*)elfBeginList(scene->lights); lig; lig = (elfLight*)elfGetListNext(scene->lights))
    {
        if (lig->script && !elfGetResourceById(scripts, lig->script->id))
        {
            elfAppendListObject(scripts, (elfObject*)lig->script);
        }
    }

    for (par = (elfParticles*)elfBeginList(scene->particles); par;
         par = (elfParticles*)elfGetListNext(scene->particles))
    {
        if (par->script && !elfGetResourceById(scripts, par->script->id))
        {
            elfAppendListObject(scripts, (elfObject*)par->script);
        }
    }

    for (spr = (elfSprite*)elfBeginList(scene->sprites); spr; spr = (elfSprite*)elfGetListNext(scene->sprites))
    {
        if (spr->script && !elfGetResourceById(scripts, spr->script->id))
        {
            elfAppendListObject(scripts, (elfObject*)spr->script);
        }
    }

    return scripts;
}

elfList* elfGetSceneTextures(elfScene* scene)
{
    elfList* textures;

    elfMaterial* mat;
    elfEntity* ent;
    elfParticles* par;
    elfSprite* spr;

    textures = elfCreateList();

    for (ent = (elfEntity*)elfBeginList(scene->entities); ent; ent = (elfEntity*)elfGetListNext(scene->entities))
    {
        for (mat = (elfMaterial*)elfBeginList(ent->materials); mat; mat = (elfMaterial*)elfGetListNext(ent->materials))
        {
            if (mat->diffuseMap && !elfGetResourceById(textures, mat->diffuseMap->id))
                elfAppendListObject(textures, (elfObject*)mat->diffuseMap);
            if (mat->normalMap && !elfGetResourceById(textures, mat->normalMap->id))
                elfAppendListObject(textures, (elfObject*)mat->normalMap);
            if (mat->heightMap && !elfGetResourceById(textures, mat->heightMap->id))
                elfAppendListObject(textures, (elfObject*)mat->heightMap);
            if (mat->specularMap && !elfGetResourceById(textures, mat->specularMap->id))
                elfAppendListObject(textures, (elfObject*)mat->specularMap);
            if (mat->lightMap && !elfGetResourceById(textures, mat->lightMap->id))
                elfAppendListObject(textures, (elfObject*)mat->lightMap);
        }
    }

    for (par = (elfParticles*)elfBeginList(scene->particles); par;
         par = (elfParticles*)elfGetListNext(scene->particles))
    {
        if (par->texture && !elfGetResourceById(textures, par->texture->id))
            elfAppendListObject(textures, (elfObject*)par->texture);
    }

    for (spr = (elfSprite*)elfBeginList(scene->sprites); spr; spr = (elfSprite*)elfGetListNext(scene->sprites))
    {
        mat = spr->material;
        if (!mat)
            continue;

        if (mat->diffuseMap && !elfGetResourceById(textures, mat->diffuseMap->id))
            elfAppendListObject(textures, (elfObject*)mat->diffuseMap);
        if (mat->normalMap && !elfGetResourceById(textures, mat->normalMap->id))
            elfAppendListObject(textures, (elfObject*)mat->normalMap);
        if (mat->heightMap && !elfGetResourceById(textures, mat->heightMap->id))
            elfAppendListObject(textures, (elfObject*)mat->heightMap);
        if (mat->specularMap && !elfGetResourceById(textures, mat->specularMap->id))
            elfAppendListObject(textures, (elfObject*)mat->specularMap);
        if (mat->lightMap && !elfGetResourceById(textures, mat->lightMap->id))
            elfAppendListObject(textures, (elfObject*)mat->lightMap);
    }

    return textures;
}

elfList* elfGetSceneMaterials(elfScene* scene)
{
    elfList* materials;

    elfMaterial* mat;
    elfEntity* ent;
    elfSprite* spr;

    materials = elfCreateList();

    for (ent = (elfEntity*)elfBeginList(scene->entities); ent; ent = (elfEntity*)elfGetListNext(scene->entities))
    {
        for (mat = (elfMaterial*)elfBeginList(ent->materials); mat; mat = (elfMaterial*)elfGetListNext(ent->materials))
        {
            if (!elfGetResourceById(materials, mat->id))
            {
                elfAppendListObject(materials, (elfObject*)mat);
            }
        }
    }

    for (spr = (elfSprite*)elfBeginList(scene->sprites); spr; spr = (elfSprite*)elfGetListNext(scene->sprites))
    {
        if (spr->material && !elfGetResourceById(materials, spr->material->id))
        {
            elfAppendListObject(materials, (elfObject*)spr->material);
        }
    }

    return materials;
}

elfList* elfGetSceneModels(elfScene* scene)
{
    elfList* models;

    elfEntity* ent;
    elfParticles* par;

    models = elfCreateList();

    for (ent = (elfEntity*)elfBeginList(scene->entities); ent; ent = (elfEntity*)elfGetListNext(scene->entities))
    {
        if (ent->model && !elfGetResourceById(models, ent->model->id))
        {
            elfAppendListObject(models, (elfObject*)ent->model);
        }
    }

    for (par = (elfParticles*)elfBeginList(scene->particles); par;
         par = (elfParticles*)elfGetListNext(scene->particles))
    {
        if (par->model && !elfGetResourceById(models, par->model->id))
        {
            elfAppendListObject(models, (elfObject*)par->model);
        }
    }

    return models;
}
