#include "nelf/pak/Pak.h"

#include <SOIL2/SOIL2.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "nelf/Config.h"
#include "nelf/Engine.h"
#include "nelf/General.h"
#include "nelf/Ipo.h"
#include "nelf/List.h"
#include "nelf/Log.h"
#include "nelf/Math.h"
#include "nelf/Object.h"
#include "nelf/Property.h"
#include "nelf/String.h"
#include "nelf/actor/Actor.h"
#include "nelf/actor/Camera.h"
#include "nelf/actor/Entity.h"
#include "nelf/actor/Light.h"
#include "nelf/actor/Particle.h"
#include "nelf/actor/Particles.h"
#include "nelf/actor/Sprite.h"
#include "nelf/errorCode.h"
#include "nelf/gfx/gfxAttributeType.h"
#include "nelf/gfx/gfxFormatType.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/gfx/gfxObject.h"
#include "nelf/gfx/gfxTexture.h"
#include "nelf/gfx/gfxTextureFilterType.h"
#include "nelf/gfx/gfxTextureFormat.h"
#include "nelf/gfx/gfxTextureWrapMode.h"
#include "nelf/gfx/gfxVertexArray.h"
#include "nelf/gfx/gfxVertexData.h"
#include "nelf/gfx/gfxVertexDataType.h"
#include "nelf/gfx/gfxVertexIndex.h"
#include "nelf/objectType.h"
#include "nelf/pak/PakIndex.h"
#include "nelf/pak/nameLength.h"
#include "nelf/pak/pakMagic.h"
#include "nelf/pak/pakVersion.h"
#include "nelf/propertyType.h"
#include "nelf/resource/Armature.h"
#include "nelf/resource/Material.h"
#include "nelf/resource/Model.h"
#include "nelf/resource/Resource.h"
#include "nelf/resource/Resources.h"
#include "nelf/resource/Scene.h"
#include "nelf/resource/Script.h"
#include "nelf/resource/Texture.h"

elfPak* elfCreatePakFromFile(const char* filePath)
{
    elfPak* pak;
    elfPakIndex* index;
    FILE* file;
    int magic;
    int version;
    int indexCount;
    int i;

    unsigned char type;
    char name[ELF_NAME_LENGTH];
    int offset;

    file = fopen(filePath, "rb");
    if (!file)
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: can't open \"%s\"\n", filePath);
        return NULL;
    }

    magic = 0;
    fread((char*)&magic, sizeof(int), 1, file);

    if (magic != ELF_PAK_MAGIC)
    {
        elfSetError(ELF_INVALID_FILE, "error: \"%s\" is not a elf pak file\n", filePath);
        fclose(file);
        return NULL;
    }

    fread((char*)&version, sizeof(int), 1, file);

    if (version < ELF_PAK_VERSION)
    {
        elfSetError(ELF_INVALID_FILE, "error: can't load \"%s\", old .pak version\n", filePath);
        fclose(file);
        return NULL;
    }

    if (version > ELF_PAK_VERSION)
    {
        elfSetError(ELF_INVALID_FILE, "error: can't load \"%s\", new .pak version\n", filePath);
        return NULL;
    }

    pak = (elfPak*)malloc(sizeof(elfPak));
    memset(pak, 0x0, sizeof(elfPak));
    pak->objType = ELF_PAK;
    pak->objDestr = elfDestroyPak;

    elfIncObj(ELF_PAK);

    pak->filePath = elfCreateString(filePath);

    pak->indexes = elfCreateList();
    elfIncRef((elfObject*)pak->indexes);

    indexCount = 0;
    fread((char*)&indexCount, sizeof(int), 1, file);

    for (i = 0; i < indexCount; i++)
    {
        type = 0;
        offset = 0;
        fread((char*)&type, sizeof(unsigned char), 1, file);

        switch (type)
        {
        case ELF_TEXTURE:
            pak->textureCount++;
            break;
        case ELF_MATERIAL:
            pak->materialCount++;
            break;
        case ELF_MODEL:
            pak->modelCount++;
            break;
        case ELF_CAMERA:
            pak->cameraCount++;
            break;
        case ELF_ENTITY:
            pak->entityCount++;
            break;
        case ELF_LIGHT:
            pak->lightCount++;
            break;
        case ELF_ARMATURE:
            pak->armatureCount++;
            break;
        case ELF_SCENE:
            pak->sceneCount++;
            break;
        case ELF_SCRIPT:
            pak->scriptCount++;
            break;
        }

        fread(name, sizeof(char), ELF_NAME_LENGTH, file);
        fread((char*)&offset, sizeof(int), 1, file);

        index = elfCreatePakIndex();
        index->indexType = type;
        index->name = elfCreateString(name);
        index->offset = offset;

        elfAppendListObject(pak->indexes, (elfObject*)index);
    }

    fclose(file);

    return pak;
}

void elfDestroyPak(void* data)
{
    elfPak* pak = (elfPak*)data;

    if (pak->filePath)
        elfDestroyString(pak->filePath);

    elfDecRef((elfObject*)pak->indexes);

    free(pak);

    elfDecObj(ELF_PAK);
}

const char* elfGetPakFilePath(elfPak* pak) { return pak->filePath; }

int elfGetPakIndexCount(elfPak* pak) { return elfGetListLength(pak->indexes); }

elfPakIndex* elfGetPakIndexByName(elfPak* pak, const char* name, unsigned char type)
{
    elfPakIndex* index;

    for (index = (elfPakIndex*)elfBeginList(pak->indexes); index; index = (elfPakIndex*)elfGetListNext(pak->indexes))
    {
        if (!strcmp(index->name, name) && index->indexType == type)
            return index;
    }

    return NULL;
}

elfPakIndex* elfGetPakIndexByIndex(elfPak* pak, int idx)
{
    if (idx < 0 || idx > elfGetListLength(pak->indexes) - 1)
        return NULL;
    return (elfPakIndex*)elfGetListObject(pak->indexes, idx);
}

int elfGetActorHeaderSizeBytes(elfActor* actor)
{
    int sizeBytes;
    elfProperty* property;
    elfBezierCurve* curve;

    sizeBytes = 0;

    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // name
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // parent
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // script
    sizeBytes += sizeof(float) * 3;               // position
    sizeBytes += sizeof(float) * 3;               // rotation

    sizeBytes += sizeof(unsigned char);  // curve count
    for (curve = (elfBezierCurve*)elfBeginList(actor->ipo->curves); curve;
         curve = (elfBezierCurve*)elfGetListNext(actor->ipo->curves))
    {
        sizeBytes += sizeof(unsigned char);                                // type
        sizeBytes += sizeof(unsigned char);                                // interpolation
        sizeBytes += sizeof(int);                                          // point count
        sizeBytes += sizeof(float) * 6 * elfGetListLength(curve->points);  // points
    }

    sizeBytes += sizeof(unsigned char);  // physics
    sizeBytes += sizeof(unsigned char);  // shape
    sizeBytes += sizeof(float) * 3;      // bounding lengths
    sizeBytes += sizeof(float) * 3;      // bounding offset
    sizeBytes += sizeof(float);          // mass
    sizeBytes += sizeof(float);          // linear damp
    sizeBytes += sizeof(float);          // angular damp
    sizeBytes += sizeof(float);          // linear sleep threshold
    sizeBytes += sizeof(float);          // angular sleep threshold
    sizeBytes += sizeof(float);          // restitution
    sizeBytes += sizeof(float) * 3;      // anisotropic friction
    sizeBytes += sizeof(float) * 3;      // linear factor
    sizeBytes += sizeof(float) * 3;      // angular factor

    sizeBytes += sizeof(int);  // property count
    for (property = (elfProperty*)elfBeginList(actor->properties); property;
         property = (elfProperty*)elfGetListNext(actor->properties))
    {
        sizeBytes += sizeof(unsigned char);           // prop type
        sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // prop name
        if (property->propertyType == ELF_PROPERTY_STRING)
        {
            sizeBytes += sizeof(int);  // string data length
            sizeBytes += sizeof(char) * strlen(property->sval);
        }
        else if (property->propertyType == ELF_PROPERTY_FLOAT)
            sizeBytes += sizeof(float);
        else if (property->propertyType == ELF_PROPERTY_INT)
            sizeBytes += sizeof(int);
        else if (property->propertyType == ELF_PROPERTY_BOOL)
            sizeBytes += sizeof(unsigned char);
    }

    return sizeBytes;
}

int elfGetArmatureSizeBytes(elfArmature* armature)
{
    int sizeBytes;
    int i;

    sizeBytes = 0;

    sizeBytes += sizeof(int);  // magic
    sizeBytes += sizeof(int);  // frame count
    sizeBytes += sizeof(int);  // bone count

    for (i = 0; i < (int)armature->boneCount; i++)
    {
        sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // name
        sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // parent
        sizeBytes += sizeof(int);                     // id
        sizeBytes += sizeof(float) * 3;               // position
        sizeBytes += sizeof(float) * 4;               // quaternion

        sizeBytes += sizeof(float) * 3 * armature->frameCount;  // positions
        sizeBytes += sizeof(float) * 4 * armature->frameCount;  // quaternions
    }

    return sizeBytes;
}

int elfGetCameraSizeBytes(elfCamera* camera)
{
    int sizeBytes;

    sizeBytes = 0;

    sizeBytes += sizeof(int);                                    // magic
    sizeBytes += elfGetActorHeaderSizeBytes((elfActor*)camera);  // actor header
    sizeBytes += sizeof(float);                                  // fov
    sizeBytes += sizeof(float);                                  // clip near
    sizeBytes += sizeof(float);                                  // clip far

    return sizeBytes;
}

int elfGetEntitySizeBytes(elfEntity* entity)
{
    int sizeBytes;

    sizeBytes = 0;

    sizeBytes += sizeof(int);                                                         // magic
    sizeBytes += elfGetActorHeaderSizeBytes((elfActor*)entity);                       // actor header
    sizeBytes += sizeof(float) * 3;                                                   // scale
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;                                      // model
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;                                      // armature
    sizeBytes += sizeof(unsigned char);                                               // occluder flag
    sizeBytes += sizeof(int);                                                         // material count
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH * elfGetEntityMaterialCount(entity);  // materials

    return sizeBytes;
}

int elfGetLightSizeBytes(elfLight* light)
{
    int sizeBytes;

    sizeBytes = 0;

    sizeBytes += sizeof(int);                                   // magic
    sizeBytes += elfGetActorHeaderSizeBytes((elfActor*)light);  // actor header
    sizeBytes += sizeof(unsigned char);                         // type
    sizeBytes += sizeof(float) * 4;                             // color
    sizeBytes += sizeof(float);                                 // range
    sizeBytes += sizeof(float);                                 // fade range
    sizeBytes += sizeof(float);                                 // inner cone
    sizeBytes += sizeof(float);                                 // outer cone
    sizeBytes += sizeof(unsigned char);                         // shadow caster
    sizeBytes += sizeof(unsigned char);                         // shaft
    sizeBytes += sizeof(float);                                 // shaft size
    sizeBytes += sizeof(float);                                 // shaft intensity
    sizeBytes += sizeof(float);                                 // shaft fade off

    return sizeBytes;
}

int elfGetMaterialSizeBytes(elfMaterial* material)
{
    int sizeBytes;

    sizeBytes = 0;

    sizeBytes += sizeof(int);                         // magic
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;      // name
    sizeBytes += sizeof(float) * 4;                   // diffuse color
    sizeBytes += sizeof(float) * 4;                   // ambient color
    sizeBytes += sizeof(float) * 4;                   // specular color
    sizeBytes += sizeof(float);                       // specular power
    sizeBytes += sizeof(unsigned char);               // lighting
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH * 5;  // textures
    sizeBytes += sizeof(float);                       // parallax scale
    sizeBytes += sizeof(unsigned char);               // alpha test
    sizeBytes += sizeof(float);                       // alpha threshold

    return sizeBytes;
}

int elfGetModelSizeBytes(elfModel* model)
{
    int sizeBytes;

    sizeBytes = 0;

    sizeBytes += sizeof(int);                     // magic
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // name
    sizeBytes += sizeof(int);                     // vertice count
    sizeBytes += sizeof(int);                     // frame count
    sizeBytes += sizeof(int);                     // indice count
    sizeBytes += sizeof(int);                     // area count
    sizeBytes += sizeof(unsigned char);           // normals
    sizeBytes += sizeof(unsigned char);           // tex coords
    sizeBytes += sizeof(unsigned char);           // weights & boneids
    sizeBytes += sizeof(unsigned char);           // junk

    sizeBytes += sizeof(float) * 3 * model->verticeCount;  // vertices

    sizeBytes += sizeof(unsigned int) * model->areaCount;    // area indice counts
    sizeBytes += sizeof(unsigned int) * model->indiceCount;  // indices

    sizeBytes += sizeof(float) * 3 * model->verticeCount;  // normals
    if (model->texCoords)
        sizeBytes += sizeof(float) * 2 * model->verticeCount;  // texcoords

    if (model->weights && model->boneids)
    {
        sizeBytes += sizeof(float) * 4 * model->verticeCount;      // weights
        sizeBytes += sizeof(short int) * 4 * model->verticeCount;  // boneids
    }

    return sizeBytes;
}

int elfGetSceneSizeBytes(elfScene* scene)
{
    int sizeBytes;

    sizeBytes = 0;

    sizeBytes += sizeof(int);                     // magic
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // name
    sizeBytes += sizeof(float) * 4;               // ambience

    return sizeBytes;
}

int elfGetParticlesSizeBytes(elfParticles* particles)
{
    int sizeBytes;

    sizeBytes = 0;

    sizeBytes += sizeof(int);  // magic
    sizeBytes += elfGetActorHeaderSizeBytes((elfActor*)particles);
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // texture
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // model
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // entity
    sizeBytes += sizeof(int);                     // max count
    sizeBytes += sizeof(unsigned char);           // draw mode
    sizeBytes += sizeof(int);                     // spawn count
    sizeBytes += sizeof(unsigned char);           // spawn
    sizeBytes += sizeof(float) * 3;               // gravity
    sizeBytes += sizeof(float);                   // size min
    sizeBytes += sizeof(float);                   // size max
    sizeBytes += sizeof(float);                   // size growth mix
    sizeBytes += sizeof(float);                   // size growth max
    sizeBytes += sizeof(float);                   // rotation min
    sizeBytes += sizeof(float);                   // rotation max
    sizeBytes += sizeof(float);                   // rotation growth min
    sizeBytes += sizeof(float);                   // rotation growth max
    sizeBytes += sizeof(float);                   // life span min
    sizeBytes += sizeof(float);                   // life span max
    sizeBytes += sizeof(float);                   // fade speed min
    sizeBytes += sizeof(float);                   // fade speed max
    sizeBytes += sizeof(float) * 3;               // velocity min
    sizeBytes += sizeof(float) * 3;               // velocity max
    sizeBytes += sizeof(float) * 3;               // position min
    sizeBytes += sizeof(float) * 3;               // position max
    sizeBytes += sizeof(float) * 4;               // color min
    sizeBytes += sizeof(float) * 4;               // color max

    return sizeBytes;
}

int elfGetScriptSizeBytes(elfScript* script)
{
    int sizeBytes;

    sizeBytes = 0;

    sizeBytes += sizeof(int);                     // magic
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // name
    sizeBytes += sizeof(int);                     // length
    if (script->text)
        sizeBytes += sizeof(char) * strlen(script->text);  // text

    return sizeBytes;
}

int elfGetSpriteSizeBytes(elfSprite* sprite)
{
    int sizeBytes;

    sizeBytes = 0;

    sizeBytes += sizeof(int);  // magic
    sizeBytes += elfGetActorHeaderSizeBytes((elfActor*)sprite);
    sizeBytes += sizeof(float) * 2;               // scale
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // material
    sizeBytes += sizeof(unsigned char);           // face camera

    return sizeBytes;
}

int elfGetTextureSizeBytes(elfTexture* texture)
{
    int sizeBytes;

    sizeBytes = 0;

    sizeBytes += sizeof(int);                     // magic
    sizeBytes += sizeof(char) * ELF_NAME_LENGTH;  // name
    sizeBytes += sizeof(unsigned char);           // type
    sizeBytes += sizeof(int);                     // data length
    sizeBytes += texture->dataSize;

    return sizeBytes;
}

void elfReadActorHeader(elfActor* actor, FILE* file, elfScene* scene)
{
    char name[ELF_NAME_LENGTH];
    char parentName[ELF_NAME_LENGTH];
    char scriptName[ELF_NAME_LENGTH];
    float position[3];
    float rotation[3];
    elfScript* script;
    int i, j;
    elfBezierPoint* point;
    elfBezierCurve* curve;
    unsigned char curveCount;
    int pointCount;
    unsigned char physics;
    float boundingLengths[3];
    float boundingOffset[3];
    unsigned char shape;
    float mass;
    float linDamp;
    float angDamp;
    float linSleep;
    float angSleep;
    float restitution;
    float anisFric[3];
    float linearFactor[3];
    float angularFactor[3];
    int propertyCount;
    char propName[ELF_NAME_LENGTH];
    elfProperty* property;
    int length;

    fread(name, sizeof(char), ELF_NAME_LENGTH, file);
    fread(parentName, sizeof(char), ELF_NAME_LENGTH, file);
    fread(scriptName, sizeof(char), ELF_NAME_LENGTH, file);

    actor->name = elfCreateString(name);
    if (scene)
        actor->filePath = elfCreateString(elfGetSceneFilePath(scene));

    fread((char*)position, sizeof(float), 3, file);
    fread((char*)rotation, sizeof(float), 3, file);

    elfSetActorPosition(actor, position[0], position[1], position[2]);
    elfSetActorRotation(actor, rotation[0], rotation[1], rotation[2]);

    script = NULL;

    if (scene && strlen(scriptName) > 0)
    {
        script = elfGetOrLoadScriptByName(scene, scriptName);
        if (script)
            elfSetActorScript(actor, script);
    }

    curveCount = 0;
    fread((char*)&curveCount, sizeof(unsigned char), 1, file);
    for (i = 0; i < curveCount; i++)
    {
        curve = elfCreateBezierCurve();
        fread((char*)&curve->curveType, sizeof(unsigned char), 1, file);
        fread((char*)&curve->interpolation, sizeof(unsigned char), 1, file);

        pointCount = 0;
        fread((char*)&pointCount, sizeof(int), 1, file);
        for (j = 0; j < pointCount; j++)
        {
            point = elfCreateBezierPoint();
            fread((char*)&point->c1.x, sizeof(float), 2, file);
            fread((char*)&point->p.x, sizeof(float), 2, file);
            fread((char*)&point->c2.x, sizeof(float), 2, file);

            elfAddBezierCurvePoint(curve, point);
        }

        elfAddIpoCurve(actor->ipo, curve);
    }

    fread((char*)&physics, sizeof(unsigned char), 1, file);
    fread((char*)&shape, sizeof(unsigned char), 1, file);
    fread((char*)boundingLengths, sizeof(float), 3, file);
    fread((char*)boundingOffset, sizeof(float), 3, file);
    fread((char*)&mass, sizeof(float), 1, file);
    fread((char*)&linDamp, sizeof(float), 1, file);
    fread((char*)&angDamp, sizeof(float), 1, file);
    fread((char*)&linSleep, sizeof(float), 1, file);
    fread((char*)&angSleep, sizeof(float), 1, file);
    fread((char*)&restitution, sizeof(float), 1, file);
    fread((char*)anisFric, sizeof(float), 3, file);
    fread((char*)linearFactor, sizeof(float), 3, file);
    fread((char*)angularFactor, sizeof(float), 3, file);

    elfSetActorShape(actor, shape);

    elfSetActorBoundingLengths(actor, boundingLengths[0], boundingLengths[1], boundingLengths[2]);
    elfSetActorBoundingOffset(actor, boundingOffset[0], boundingOffset[1], boundingOffset[2]);
    elfResetActorBoundingOffsetSetFlag(actor);

    elfSetActorMass(actor, mass);
    elfSetActorDamping(actor, linDamp, angDamp);
    elfSetActorSleep(actor, linSleep, angSleep);
    elfSetActorRestitution(actor, restitution);
    elfSetActorAnisotropicFriction(actor, anisFric[0], anisFric[1], anisFric[2]);
    elfSetActorLinearFactor(actor, linearFactor[0], linearFactor[1], linearFactor[2]);
    elfSetActorAngularFactor(actor, angularFactor[0], angularFactor[1], angularFactor[2]);
    elfSetActorPhysics(actor, physics);

    fread((char*)&propertyCount, sizeof(int), 1, file);
    for (i = 0; i < propertyCount; i++)
    {
        property = elfCreateProperty(NULL);

        fread((char*)&property->propertyType, sizeof(unsigned char), 1, file);
        fread(propName, sizeof(char), ELF_NAME_LENGTH, file);

        property->name = elfCreateString(propName);

        if (property->propertyType == ELF_PROPERTY_STRING)
        {
            fread((char*)&length, sizeof(int), 1, file);

            property->sval = (char*)malloc(sizeof(char) * (length + 1));
            elfIncObj(ELF_STRING);

            fread(property->sval, sizeof(char), length, file);
            property->sval[length] = '\0';
        }
        else if (property->propertyType == ELF_PROPERTY_FLOAT)
            fread((char*)&property->fval, sizeof(float), 1, file);
        else if (property->propertyType == ELF_PROPERTY_INT)
            fread((char*)&property->fval, sizeof(int), 1, file);
        else if (property->propertyType == ELF_PROPERTY_BOOL)
            fread((char*)&property->fval, sizeof(unsigned char), 1, file);

        elfAppendListObject(actor->properties, (elfObject*)property);
    }
}

elfArmature* elfCreateArmatureFromPak(FILE* file, const char* name, elfScene* scene)
{
    elfArmature* armature;
    char rname[ELF_NAME_LENGTH];
    char parent[ELF_NAME_LENGTH];
    int magic;
    elfBone* bone;
    elfBone* parentBone;
    elfList* bones;
    elfList* boneParents;
    elfString* strObj;
    int i, j;
    float boneInvQua[4];

    fread((char*)&magic, sizeof(int), 1, file);

    if (magic != ELF_ARMATURE_MAGIC)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid armature \"%s//%s\", wrong magic number\n",
                    elfGetSceneFilePath(scene), name);
        return NULL;
    }

    armature = elfCreateArmature(NULL);

    fread(rname, sizeof(char), ELF_NAME_LENGTH, file);
    fread((char*)&armature->frameCount, sizeof(int), 1, file);
    fread((char*)&armature->boneCount, sizeof(int), 1, file);

    armature->name = elfCreateString(rname);
    armature->filePath = elfCreateString(elfGetSceneFilePath(scene));

    // lol...
    armature->bbMin.x = armature->bbMin.y = armature->bbMin.z = 999999.0;
    armature->bbMax.x = armature->bbMax.y = armature->bbMax.z = -999999.0;

    if (armature->boneCount > 0)
    {
        bones = elfCreateList();
        boneParents = elfCreateList();

        for (i = 0; i < (int)armature->boneCount; i++)
        {
            bone = elfCreateBone(NULL);

            fread(rname, sizeof(char), ELF_NAME_LENGTH, file);
            fread(parent, sizeof(char), ELF_NAME_LENGTH, file);
            fread((char*)&bone->id, sizeof(int), 1, file);
            fread((char*)&bone->pos.x, sizeof(float), 3, file);
            fread((char*)&bone->qua.x, sizeof(float), 4, file);

            gfxQuaGetInverse(&bone->qua.x, boneInvQua);

            bone->name = elfCreateString(rname);

            if (armature->frameCount > 0)
            {
                bone->frames = (elfBoneFrame*)malloc(sizeof(elfBoneFrame) * armature->frameCount);
                for (j = 0; j < armature->frameCount; j++)
                {
                    fread((char*)&bone->frames[j].pos.x, sizeof(float), 3, file);
                    fread((char*)&bone->frames[j].qua.x, sizeof(float), 4, file);
                    gfxMulQuaQua(&bone->frames[j].qua.x, boneInvQua, &bone->frames[j].offsetQua.x);
                    bone->frames[j].offsetPos.x = bone->frames[j].pos.x - bone->pos.x;
                    bone->frames[j].offsetPos.y = bone->frames[j].pos.y - bone->pos.y;
                    bone->frames[j].offsetPos.z = bone->frames[j].pos.z - bone->pos.z;
                    if (bone->frames[j].pos.x < armature->bbMin.x)
                        armature->bbMin.x = bone->frames[j].pos.x;
                    if (bone->frames[j].pos.y < armature->bbMin.y)
                        armature->bbMin.y = bone->frames[j].pos.y;
                    if (bone->frames[j].pos.z < armature->bbMin.z)
                        armature->bbMin.z = bone->frames[j].pos.z;
                    if (bone->frames[j].pos.x > armature->bbMax.x)
                        armature->bbMax.x = bone->frames[j].pos.x;
                    if (bone->frames[j].pos.y > armature->bbMax.y)
                        armature->bbMax.y = bone->frames[j].pos.y;
                    if (bone->frames[j].pos.z > armature->bbMax.z)
                        armature->bbMax.z = bone->frames[j].pos.z;
                }
            }

            strObj = elfCreateStringObject();
            if (strlen(parent) > 0)
                strObj->str = elfCreateString(parent);
            elfAppendListObject(boneParents, (elfObject*)strObj);
            elfAppendListObject(bones, (elfObject*)bone);
        }

        for (bone = (elfBone*)elfBeginList(bones), strObj = (elfString*)elfBeginList(boneParents); bone && strObj;
             bone = (elfBone*)elfGetListNext(bones), strObj = (elfString*)elfGetListNext(boneParents))
        {
            if (strObj->str)
            {
                parentBone = (elfBone*)elfGetResource(bones, strObj->str);
                if (parentBone)
                {
                    bone->parent = parentBone;
                    elfAppendListObject(bone->parent->children, (elfObject*)bone);
                }
                elfSeekList(bones, (elfObject*)bone);
            }
        }

        for (bone = (elfBone*)elfBeginList(bones); bone; bone = (elfBone*)elfGetListNext(bones))
        {
            if (!bone->parent)
            {
                elfAddRootBoneToArmature(armature, bone);
            }
        }

        elfDestroyList(boneParents);
        elfDestroyList(bones);
    }

    return armature;
}

elfCamera* elfCreateCameraFromPak(FILE* file, const char* name, elfScene* scene)
{
    elfCamera* camera;
    int magic;
    float fov = 0.0f;
    float clipNear = 0.0f;
    float clipFar = 0.0f;

    fread((char*)&magic, sizeof(int), 1, file);

    if (magic != ELF_CAMERA_MAGIC)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid camera \"%s//%s\", wrong magic number\n",
                    elfGetSceneFilePath(scene), name);
        return NULL;
    }

    camera = elfCreateCamera(NULL);
    elfReadActorHeader((elfActor*)camera, file, scene);

    fread((char*)&fov, sizeof(float), 1, file);
    fread((char*)&clipNear, sizeof(float), 1, file);
    fread((char*)&clipFar, sizeof(float), 1, file);

    elfSetCameraFov(camera, fov);
    elfSetCameraClip(camera, clipNear, clipFar);

    return camera;
}

elfEntity* elfCreateEntityFromPak(FILE* file, const char* name, elfScene* scene)
{
    elfEntity* entity;
    elfModel* rmodel;
    elfArmature* rarmature;
    elfMaterial* rmaterial;
    unsigned int materialCount;
    int i, j;
    int magic = 0;
    float scale[3] = {0.0f, 0.0f, 0.0f};
    char model[ELF_NAME_LENGTH];
    char armature[ELF_NAME_LENGTH];
    char material[ELF_NAME_LENGTH];
    elfVec3f boundingLengths;
    elfVec3f boundingOffset;

    fread((char*)&magic, sizeof(int), 1, file);

    if (magic != ELF_ENTITY_MAGIC)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid entity \"%s//%s\", wrong magic number\n",
                    elfGetSceneFilePath(scene), name);
        return NULL;
    }

    entity = elfCreateEntity(NULL);
    elfReadActorHeader((elfActor*)entity, file, scene);

    fread((char*)scale, sizeof(float), 3, file);

    boundingLengths = elfGetActorBoundingLengths((elfActor*)entity);
    boundingOffset = elfGetActorBoundingOffset((elfActor*)entity);

    fread(model, sizeof(char), ELF_NAME_LENGTH, file);
    if (strlen(model))
    {
        rmodel = elfGetOrLoadModelByName(scene, model);
        elfSetEntityModel(entity, rmodel);
    }

    if (!elfAboutZero(boundingLengths.x) || !elfAboutZero(boundingLengths.y) || !elfAboutZero(boundingLengths.z))
        elfSetActorBoundingLengths((elfActor*)entity, boundingLengths.x, boundingLengths.y, boundingLengths.z);
    if (!elfAboutZero(boundingOffset.x) || !elfAboutZero(boundingOffset.y) || !elfAboutZero(boundingOffset.z))
        elfSetActorBoundingOffset((elfActor*)entity, boundingOffset.x, boundingOffset.y, boundingOffset.z);

    fread(armature, sizeof(char), ELF_NAME_LENGTH, file);
    if (strlen(armature))
    {
        rarmature = elfGetOrLoadArmatureByName(scene, armature);
        if (rarmature)
            elfSetEntityArmature(entity, rarmature);
    }

    // scale must be set after setting a model, setting a model resets the scale
    elfSetEntityScale(entity, scale[0], scale[1], scale[2]);

    fread((char*)&entity->occluder, sizeof(unsigned char), 1, file);
    elfSetEntityOccluder(entity, entity->occluder);

    fread((char*)&materialCount, sizeof(int), 1, file);

    for (i = 0, j = 0; i < (int)materialCount; i++)
    {
        memset(material, 0x0, sizeof(char) * ELF_NAME_LENGTH);
        fread(material, sizeof(char), ELF_NAME_LENGTH, file);

        rmaterial = NULL;
        if (strlen(material))
            rmaterial = elfGetOrLoadMaterialByName(scene, material);
        if (rmaterial)
        {
            if (elfGetEntityMaterialCount(entity) > j)
                elfSetEntityMaterial(entity, j, rmaterial);
            else
                elfAddEntityMaterial(entity, rmaterial);
            j++;
        }
    }

    // check if normal/displacement maps are needed
    elfGenerateEntityTangents(entity);

    return entity;
}

elfLight* elfCreateLightFromPak(FILE* file, const char* name, elfScene* scene)
{
    elfLight* light;
    int magic = 0;

    fread((char*)&magic, sizeof(int), 1, file);

    if (magic != ELF_LIGHT_MAGIC)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid light \"%s//%s\", wrong magic number\n",
                    elfGetSceneFilePath(scene), name);
        return NULL;
    }

    light = elfCreateLight(NULL);
    elfReadActorHeader((elfActor*)light, file, scene);

    fread((char*)&light->lightType, sizeof(unsigned char), 1, file);
    fread((char*)&light->color.r, sizeof(float), 4, file);
    fread((char*)&light->range, sizeof(float), 1, file);
    fread((char*)&light->fadeRange, sizeof(float), 1, file);
    fread((char*)&light->innerCone, sizeof(float), 1, file);
    fread((char*)&light->outerCone, sizeof(float), 1, file);
    fread((char*)&light->shadows, sizeof(unsigned char), 1, file);
    fread((char*)&light->shaft, sizeof(unsigned char), 1, file);
    fread((char*)&light->shaftSize, sizeof(float), 1, file);
    fread((char*)&light->shaftIntensity, sizeof(float), 1, file);
    fread((char*)&light->shaftFadeOff, sizeof(float), 1, file);

    elfSetLightType(light, light->lightType);
    elfSetLightColor(light, light->color.r, light->color.g, light->color.b, light->color.a);
    elfSetLightRange(light, light->range, light->fadeRange);
    elfSetLightShadows(light, light->shadows);
    elfSetLightCone(light, light->innerCone, light->outerCone);
    elfSetLightShaft(light, light->shaft);
    elfSetLightShaftSize(light, light->shaftSize);
    elfSetLightShaftIntensity(light, light->shaftIntensity);
    elfSetLightShaftFadeOff(light, light->shaftFadeOff);

    return light;
}

elfMaterial* elfCreateMaterialFromPak(FILE* file, const char* name, elfScene* scene)
{
    elfMaterial* material;
    elfTexture* rtexture;
    int magic;
    char rname[ELF_NAME_LENGTH];
    char texture[ELF_NAME_LENGTH];
    float parallaxScale;
    unsigned char alphaTest;
    float alphaThreshold;

    fread((char*)&magic, sizeof(int), 1, file);

    if (magic != ELF_MATERIAL_MAGIC)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid material \"%s//%s\", wrong magic number\n",
                    elfGetSceneFilePath(scene), name);
        return NULL;
    }

    memset(rname, 0x0, sizeof(char) * ELF_NAME_LENGTH);
    fread(rname, sizeof(char), ELF_NAME_LENGTH, file);

    material = elfCreateMaterial(NULL);

    material->name = elfCreateString(rname);
    material->filePath = elfCreateString(elfGetSceneFilePath(scene));

    fread((char*)&material->diffuseColor.r, sizeof(float), 4, file);
    fread((char*)&material->ambientColor.r, sizeof(float), 4, file);
    fread((char*)&material->specularColor.r, sizeof(float), 4, file);
    fread((char*)&material->specPower, sizeof(float), 1, file);
    fread((char*)&material->lighting, sizeof(unsigned char), 1, file);

    elfSetMaterialDiffuseColor(material, material->diffuseColor.r, material->diffuseColor.g, material->diffuseColor.b,
                               material->diffuseColor.a);
    elfSetMaterialSpecularColor(material, material->specularColor.r, material->specularColor.g,
                                material->specularColor.b, material->specularColor.a);
    elfSetMaterialAmbientColor(material, material->ambientColor.r, material->ambientColor.g, material->ambientColor.b,
                               material->ambientColor.a);
    elfSetMaterialSpecularPower(material, material->specPower);
    elfSetMaterialLighting(material, material->lighting);

    fread(texture, sizeof(char), ELF_NAME_LENGTH, file);
    if (strlen(texture) > 0)
        rtexture = elfGetOrLoadTextureByName(scene, texture);
    else
        rtexture = NULL;
    if (rtexture)
        elfSetMaterialDiffuseMap(material, rtexture);

    fread(texture, sizeof(char), ELF_NAME_LENGTH, file);
    if (strlen(texture) > 0)
        rtexture = elfGetOrLoadTextureByName(scene, texture);
    else
        rtexture = NULL;
    if (rtexture)
        elfSetMaterialNormalMap(material, rtexture);

    fread(texture, sizeof(char), ELF_NAME_LENGTH, file);
    if (strlen(texture) > 0)
        rtexture = elfGetOrLoadTextureByName(scene, texture);
    else
        rtexture = NULL;
    if (rtexture)
        elfSetMaterialHeightMap(material, rtexture);

    fread(texture, sizeof(char), ELF_NAME_LENGTH, file);
    if (strlen(texture) > 0)
        rtexture = elfGetOrLoadTextureByName(scene, texture);
    else
        rtexture = NULL;
    if (rtexture)
        elfSetMaterialSpecularMap(material, rtexture);

    fread(texture, sizeof(char), ELF_NAME_LENGTH, file);
    if (strlen(texture) > 0)
        rtexture = elfGetOrLoadTextureByName(scene, texture);
    else
        rtexture = NULL;
    if (rtexture)
        elfSetMaterialLightMap(material, rtexture);

    fread((char*)&parallaxScale, sizeof(float), 1, file);
    fread((char*)&alphaTest, sizeof(unsigned char), 1, file);
    fread((char*)&alphaThreshold, sizeof(float), 1, file);

    elfSetMaterialParallaxScale(material, parallaxScale);
    elfSetMaterialAlphaTest(material, alphaTest);
    elfSetMaterialAlphaThreshold(material, alphaThreshold);

    return material;
}

elfModel* elfCreateModelFromPak(FILE* file, const char* name, elfScene* scene)
{
    elfModel* model = NULL;
    int magic = 0;
    int i = 0;
    char rname[ELF_NAME_LENGTH];
    unsigned int indicesRead = 0;
    unsigned char isNormals;
    unsigned char isTexCoords;
    unsigned char isWeightsAndBoneids;
    unsigned char junk;
    float weights[4];
    float length;
    short int boneids[4];
    float* vertexBuffer;

    // read magic
    fread((char*)&magic, sizeof(int), 1, file);

    if (magic != ELF_MODEL_MAGIC)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid model \"%s\", wrong magic number\n", name);
        return NULL;
    }

    model = elfCreateModel(NULL);

    // read name
    fread(rname, sizeof(char), ELF_NAME_LENGTH, file);

    model->name = elfCreateString(rname);
    model->filePath = elfCreateString(elfGetSceneFilePath(scene));

    // read header
    fread((char*)&model->verticeCount, sizeof(int), 1, file);
    fread((char*)&model->frameCount, sizeof(int), 1, file);
    fread((char*)&model->indiceCount, sizeof(int), 1, file);
    fread((char*)&model->areaCount, sizeof(int), 1, file);
    fread((char*)&isNormals, sizeof(unsigned char), 1, file);
    fread((char*)&isTexCoords, sizeof(unsigned char), 1, file);
    fread((char*)&isWeightsAndBoneids, sizeof(unsigned char), 1, file);
    fread((char*)&junk, sizeof(unsigned char), 1, file);

    if (model->verticeCount < 3)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid model \"%s\", invalid vertex count\n", name);
        elfDestroyModel(model);
        return NULL;
    }
    if (model->frameCount < 1)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid model \"%s\", invalid frame count\n", name);
        elfDestroyModel(model);
        return NULL;
    }
    if (model->indiceCount < 3)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid model \"%s\", invalid indice count\n", name);
        elfDestroyModel(model);
        return NULL;
    }
    if (model->areaCount < 1)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid model \"%s\", invalid area count\n", name);
        elfDestroyModel(model);
        return NULL;
    }

    // read vertices
    model->vertices = gfxCreateVertexData(3 * model->verticeCount, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
    gfxIncRef((gfxObject*)model->vertices);

    fread((char*)gfxGetVertexDataBuffer(model->vertices), sizeof(float), 3 * model->verticeCount, file);

    // read index
    model->index = (unsigned int*)malloc(sizeof(unsigned int) * model->indiceCount);

    model->areas = (elfModelArea*)malloc(sizeof(elfModelArea) * model->areaCount);
    memset(model->areas, 0x0, sizeof(elfModelArea) * model->areaCount);

    for (i = 0; i < model->areaCount; i++)
    {
        fread((char*)&model->areas[i].indiceCount, sizeof(int), 1, file);
        if (model->areas[i].indiceCount)
        {
            model->areas[i].index = gfxCreateVertexData(model->areas[i].indiceCount, GFX_UINT, GFX_VERTEX_DATA_STATIC);
            gfxIncRef((gfxObject*)model->areas[i].index);

            fread((char*)gfxGetVertexDataBuffer(model->areas[i].index), sizeof(unsigned int),
                  model->areas[i].indiceCount, file);

            memcpy(&model->index[indicesRead], gfxGetVertexDataBuffer(model->areas[i].index),
                   gfxGetVertexDataSizeBytes(model->areas[i].index));

            indicesRead += model->areas[i].indiceCount;
        }
    }

    model->normals = gfxCreateVertexData(3 * model->verticeCount, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
    gfxIncRef((gfxObject*)model->normals);

    fread((char*)gfxGetVertexDataBuffer(model->normals), sizeof(float), 3 * model->verticeCount, file);

    // read tex coords
    if (isTexCoords > 0)
    {
        model->texCoords = gfxCreateVertexData(2 * model->verticeCount, GFX_FLOAT, GFX_VERTEX_DATA_STATIC);
        gfxIncRef((gfxObject*)model->texCoords);

        fread((char*)gfxGetVertexDataBuffer(model->texCoords), sizeof(float), 2 * model->verticeCount, file);
    }

    // read weights and bone ids
    if (isWeightsAndBoneids > 0)
    {
        model->weights = (float*)malloc(sizeof(float) * 4 * model->verticeCount);
        for (i = 0; i < model->verticeCount; i++)
        {
            fread((char*)weights, sizeof(float), 4, file);
            if (weights[0] > 1.0f)
                weights[0] = 1.0f;
            if (weights[1] > 1.0f)
                weights[1] = 1.0f;
            if (weights[2] > 1.0f)
                weights[2] = 1.0f;
            if (weights[3] > 1.0f)
                weights[3] = 1.0f;
            if (weights[0] < 0.0f)
                weights[0] = 0.0f;
            if (weights[1] < 0.0f)
                weights[1] = 0.0f;
            if (weights[2] < 0.0f)
                weights[2] = 0.0f;
            if (weights[3] < 0.0f)
                weights[3] = 0.0f;
            length = 1.0f / (weights[0] + weights[1] + weights[2] + weights[3]);
            model->weights[i * 4] = weights[0] * length;
            model->weights[i * 4 + 1] = weights[1] * length;
            model->weights[i * 4 + 2] = weights[2] * length;
            model->weights[i * 4 + 3] = weights[3] * length;
        }
        model->boneids = (int*)malloc(sizeof(int) * 4 * model->verticeCount);
        for (i = 0; i < model->verticeCount; i++)
        {
            fread((char*)boneids, sizeof(short int), 4, file);
            model->boneids[i * 4] = boneids[0];
            model->boneids[i * 4 + 1] = boneids[1];
            model->boneids[i * 4 + 2] = boneids[2];
            model->boneids[i * 4 + 3] = boneids[3];
        }
    }

    vertexBuffer = (float*)gfxGetVertexDataBuffer(model->vertices);

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

    model->vertexArray = gfxCreateVertexArray(true);
    gfxIncRef((gfxObject*)model->vertexArray);

    gfxSetVertexArrayData(model->vertexArray, GFX_VERTEX, model->vertices);
    gfxSetVertexArrayData(model->vertexArray, GFX_NORMAL, model->normals);
    if (isTexCoords > 0)
        gfxSetVertexArrayData(model->vertexArray, GFX_TEX_COORD, model->texCoords);

    for (i = 0; i < model->areaCount; i++)
    {
        if (model->areas[i].indiceCount > 0)
        {
            model->areas[i].vertexIndex = gfxCreateVertexIndex(true, model->areas[i].index);
            gfxIncRef((gfxObject*)model->areas[i].vertexIndex);
        }
    }

    return model;
}

elfParticles* elfCreateParticlesFromPak(FILE* file, const char* name, elfScene* scene)
{
    elfParticles* particles;
    elfTexture* rtexture;
    elfModel* rmodel;
    elfEntity* rentity;
    int magic = 0;
    char texture[ELF_NAME_LENGTH];
    char model[ELF_NAME_LENGTH];
    char entity[ELF_NAME_LENGTH];

    fread((char*)&magic, sizeof(int), 1, file);

    if (magic != ELF_PARTICLES_MAGIC)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid particles \"%s//%s\", wrong magic number\n",
                    elfGetSceneFilePath(scene), name);
        return NULL;
    }

    particles = elfCreateParticles(NULL, 10);
    elfReadActorHeader((elfActor*)particles, file, scene);

    fread(texture, sizeof(char), ELF_NAME_LENGTH, file);
    if (strlen(texture))
    {
        rtexture = elfGetOrLoadTextureByName(scene, texture);
        elfSetParticlesTexture(particles, rtexture);
    }

    fread(model, sizeof(char), ELF_NAME_LENGTH, file);
    if (strlen(model))
    {
        rmodel = elfGetOrLoadModelByName(scene, model);
        elfSetParticlesModel(particles, rmodel);
    }

    fread(entity, sizeof(char), ELF_NAME_LENGTH, file);
    if (strlen(entity))
    {
        rentity = elfGetOrLoadEntityByName(scene, entity);
        elfSetParticlesEntity(particles, rentity);
    }

    fread((char*)&particles->maxCount, sizeof(int), 1, file);
    fread((char*)&particles->drawMode, sizeof(unsigned char), 1, file);

    elfSetParticlesMaxCount(particles, particles->maxCount);
    elfSetParticlesDrawMode(particles, particles->drawMode);

    fread((char*)&particles->spawnCount, sizeof(int), 1, file);
    fread((char*)&particles->spawn, sizeof(unsigned char), 1, file);
    fread((char*)&particles->gravity.x, sizeof(float), 3, file);
    fread((char*)&particles->sizeMin, sizeof(float), 1, file);
    fread((char*)&particles->sizeMax, sizeof(float), 1, file);
    fread((char*)&particles->sizeGrowthMin, sizeof(float), 1, file);
    fread((char*)&particles->sizeGrowthMax, sizeof(float), 1, file);
    fread((char*)&particles->rotationMin, sizeof(float), 1, file);
    fread((char*)&particles->rotationMax, sizeof(float), 1, file);
    fread((char*)&particles->rotationGrowthMin, sizeof(float), 1, file);
    fread((char*)&particles->rotationGrowthMax, sizeof(float), 1, file);
    fread((char*)&particles->lifeSpanMin, sizeof(float), 1, file);
    fread((char*)&particles->lifeSpanMax, sizeof(float), 1, file);
    fread((char*)&particles->fadeSpeedMin, sizeof(float), 1, file);
    fread((char*)&particles->fadeSpeedMax, sizeof(float), 1, file);
    fread((char*)&particles->velocityMin.x, sizeof(float), 3, file);
    fread((char*)&particles->velocityMax.x, sizeof(float), 3, file);
    fread((char*)&particles->positionMin.x, sizeof(float), 3, file);
    fread((char*)&particles->positionMax.x, sizeof(float), 3, file);
    fread((char*)&particles->colorMin.r, sizeof(float), 4, file);
    fread((char*)&particles->colorMax.r, sizeof(float), 4, file);

    elfSetParticlesSpawnCount(particles, particles->spawnCount);
    elfSetParticlesSpawn(particles, particles->spawn);
    elfSetParticlesGravity(particles, particles->gravity.x, particles->gravity.y, particles->gravity.z);
    elfSetParticlesSize(particles, particles->sizeMin, particles->sizeMax);
    elfSetParticlesSizeGrowth(particles, particles->sizeGrowthMin, particles->sizeGrowthMax);
    elfSetParticlesRotation(particles, particles->rotationMin, particles->rotationMax);
    elfSetParticlesRotationGrowth(particles, particles->rotationGrowthMin, particles->rotationGrowthMax);
    elfSetParticlesLifeSpan(particles, particles->lifeSpanMin, particles->lifeSpanMax);
    elfSetParticlesFadeSpeed(particles, particles->fadeSpeedMin, particles->fadeSpeedMax);
    elfSetParticlesVelocityMin(particles, particles->velocityMin.x, particles->velocityMin.y, particles->velocityMin.z);
    elfSetParticlesVelocityMax(particles, particles->velocityMax.x, particles->velocityMax.y, particles->velocityMax.z);
    elfSetParticlesPositionMin(particles, particles->positionMin.x, particles->positionMin.y, particles->positionMin.z);
    elfSetParticlesPositionMax(particles, particles->positionMax.x, particles->positionMax.y, particles->positionMax.z);
    elfSetParticlesColorMin(particles, particles->colorMin.r, particles->colorMin.g, particles->colorMin.b,
                            particles->colorMin.a);
    elfSetParticlesColorMax(particles, particles->colorMax.r, particles->colorMax.g, particles->colorMax.b,
                            particles->colorMax.a);

    return particles;
}

elfScript* elfCreateScriptFromPak(FILE* file, const char* name, elfScene* scene)
{
    elfScript* script;
    int magic = 0;
    char rname[ELF_NAME_LENGTH];
    unsigned int length;
    char* text;

    fread((char*)&magic, sizeof(int), 1, file);

    if (magic != ELF_SCRIPT_MAGIC)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid script \"%s\", wrong magic number\n", name);
        return NULL;
    }

    memset(rname, 0x0, sizeof(char) * ELF_NAME_LENGTH);
    fread(rname, sizeof(char), ELF_NAME_LENGTH, file);

    script = elfCreateScript(NULL);

    script->name = elfCreateString(name);
    script->filePath = elfCreateString(elfGetSceneFilePath(scene));

    fread((char*)&length, sizeof(unsigned int), 1, file);
    if (length > 0)
    {
        text = (char*)malloc(sizeof(char) * length + 1);
        memset(text, 0x0, sizeof(char) * length + 1);
        fread(text, sizeof(char), length, file);
        text[length] = '\0';
        elfSetScriptText(script, text);
        free(text);
    }

    return script;
}

elfSprite* elfCreateSpriteFromPak(FILE* file, const char* name, elfScene* scene)
{
    elfSprite* sprite;
    elfMaterial* rmaterial;
    int magic = 0;
    float scale[2] = {0.0f, 0.0f};
    char material[ELF_NAME_LENGTH];

    fread((char*)&magic, sizeof(int), 1, file);

    if (magic != ELF_SPRITE_MAGIC)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid sprite \"%s//%s\", wrong magic number\n",
                    elfGetSceneFilePath(scene), name);
        return NULL;
    }

    sprite = elfCreateSprite(NULL);
    elfReadActorHeader((elfActor*)sprite, file, scene);

    fread((char*)scale, sizeof(float), 2, file);

    fread(material, sizeof(char), ELF_NAME_LENGTH, file);
    if (strlen(material))
    {
        rmaterial = elfGetOrLoadMaterialByName(scene, material);
        elfSetSpriteMaterial(sprite, rmaterial);
    }

    elfSetSpriteScale(sprite, scale[0], scale[1]);

    fread((char*)&sprite->faceCamera, sizeof(unsigned char), 1, file);
    elfSetSpriteFaceCamera(sprite, sprite->faceCamera);

    return sprite;
}

elfTexture* elfCreateTextureFromPak(FILE* file, const char* name, elfScene* scene)
{
    elfTexture* texture;
    unsigned char* mem;
    int magic;
    char rname[ELF_NAME_LENGTH];
    unsigned char type;
    int width;
    int height;
    unsigned char bpp;
    unsigned int length;
    int format;
    int internalFormat;
    int dataFormat;
    unsigned char* data = nullptr;

    fread((char*)&magic, sizeof(int), 1, file);

    if (magic != ELF_TEXTURE_MAGIC)
    {
        elfSetError(ELF_INVALID_FILE, "error: invalid texture \"%s//%s\", wrong magic number\n",
                    elfGetSceneFilePath(scene), name);
        return NULL;
    }

    fread(rname, sizeof(char), ELF_NAME_LENGTH, file);
    fread((char*)&type, sizeof(unsigned char), 1, file);

    if (type == 1)
    {
        fread((char*)&length, sizeof(int), 1, file);

        mem = (unsigned char*)malloc(length);
        fread(mem, sizeof(unsigned char), length, file);

        int channels = 0;
        data = SOIL_load_image_from_memory(mem, length, &width, &height, &channels, SOIL_LOAD_AUTO);

        bpp = channels * 8;

        free(mem);
    }
    else
    {
        elfSetError(ELF_UNKNOWN_FORMAT, "error: can't load texture \"%s//%s\", unknown format\n",
                    elfGetSceneFilePath(scene), rname);
        return NULL;
    }

    switch (bpp)
    {
    case 8:
        format = GFX_LUMINANCE;
        internalFormat = GFX_LUMINANCE;
        dataFormat = GFX_UBYTE;
        break;
    case 16:
        format = GFX_LUMINANCE_ALPHA;
        internalFormat = GFX_LUMINANCE_ALPHA;
        dataFormat = GFX_UBYTE;
        break;
    case 24:
        format = GFX_BGR;
        internalFormat = eng->config->textureCompress ? GFX_COMPRESSED_RGB : GFX_RGB;
        dataFormat = GFX_UBYTE;
        break;
    case 32:
        format = GFX_BGRA;
        internalFormat = eng->config->textureCompress ? GFX_COMPRESSED_RGBA : GFX_RGBA;
        dataFormat = GFX_UBYTE;
        break;
    case 48:
        format = GFX_BGR;
        internalFormat = eng->config->textureCompress ? GFX_COMPRESSED_RGB : GFX_RGB;
        dataFormat = GFX_USHORT;
        break;
    default:
        elfSetError(ELF_INVALID_FILE, "error: unsupported bits per pixel value [%d] in texture \"%s//%s\"\n", (int)bpp,
                    elfGetSceneFilePath(scene), rname);
        free(data);
        return NULL;
    }

    texture = elfCreateTexture();

    texture->name = elfCreateString(rname);
    texture->filePath = elfCreateString(elfGetSceneFilePath(scene));
    texture->texture = gfxCreate2dTexture(width, height, eng->config->textureAnisotropy, GFX_REPEAT, GFX_LINEAR, format,
                                          internalFormat, dataFormat, data);

    free(data);

    if (!texture->texture)
    {
        elfSetError(ELF_CANT_CREATE, "error: can't create texture \"%s//%s\"\n", elfGetSceneFilePath(scene), rname);
        elfDestroyTexture(texture);
        return NULL;
    }

    return texture;
}

elfScene* elfCreateSceneFromPak(const char* name, elfPak* pak)
{
    elfScene* scene = nullptr;
    elfCamera* camera = nullptr;
    elfEntity* entity = nullptr;
    elfLight* light = nullptr;
    elfSprite* sprite = nullptr;
    elfParticles* particles = nullptr;
    elfPakIndex* index = nullptr;
    FILE* file = nullptr;
    int magic = 0;
    char rname[ELF_NAME_LENGTH];
    float ambientColor[4];

    scene = elfCreateScene(NULL);

    scene->name = elfCreateString(name);
    scene->filePath = elfCreateString(elfGetPakFilePath(pak));

    scene->pak = pak;
    elfIncRef((elfObject*)pak);

    bool sceneRead = false;
    for (index = (elfPakIndex*)elfBeginList(pak->indexes); index; index = (elfPakIndex*)elfGetListNext(pak->indexes))
    {
        if (index->indexType == ELF_CAMERA)
            camera = elfGetOrLoadCameraByName(scene, index->name);
        else if (index->indexType == ELF_ENTITY)
            entity = elfGetOrLoadEntityByName(scene, index->name);
        else if (index->indexType == ELF_LIGHT)
            light = elfGetOrLoadLightByName(scene, index->name);
        else if (index->indexType == ELF_SPRITE)
            sprite = elfGetOrLoadSpriteByName(scene, index->name);
        else if (index->indexType == ELF_PARTICLES)
            particles = elfGetOrLoadParticlesByName(scene, index->name);
        else if (index->indexType == ELF_SCENE && !sceneRead)
        {
            file = fopen(elfGetPakFilePath(pak), "rb");
            if (file)
            {
                sceneRead = true;
                fseek(file, elfGetPakIndexOffset(index), SEEK_SET);

                fread((char*)&magic, sizeof(int), 1, file);
                if (magic != ELF_SCENE_MAGIC)
                {
                    printf("warning: scene header section of \"%s\" is invalid\n", elfGetPakFilePath(pak));
                    continue;
                }

                fread(rname, sizeof(char), ELF_NAME_LENGTH, file);
                if (!name || strlen(name) < 1)
                {
                    if (scene->name)
                        elfDestroyString(scene->name);
                    scene->name = elfCreateString(rname);
                }

                fread((char*)ambientColor, sizeof(float), 4, file);

                elfSetSceneAmbientColor(scene, ambientColor[0], ambientColor[1], ambientColor[2], ambientColor[3]);

                fclose(file);
            }
        }
        elfSeekList(pak->indexes, (elfObject*)index);
    }

    return scene;
}

void elfWriteNameToFile(const char* name, FILE* file)
{
    int empty;
    int length;
    char estr[ELF_NAME_LENGTH];

    memset(estr, 0x0, sizeof(char) * ELF_NAME_LENGTH);

    length = strlen(name);
    if (length > ELF_NAME_LENGTH - 1)
        length = ELF_NAME_LENGTH - 1;

    empty = ELF_NAME_LENGTH - length;

    fwrite(name, sizeof(char), length, file);
    fwrite(estr, sizeof(char), empty, file);
}

void elfWriteActorHeader(elfActor* actor, FILE* file)
{
    float position[3];
    float rotation[3];
    elfBezierPoint* point;
    elfBezierCurve* curve;
    unsigned char curveCount;
    int pointCount;
    unsigned char physics;
    elfVec3f boundingLengths;
    elfVec3f boundingOffset;
    unsigned char shape;
    float mass;
    float linDamp;
    float angDamp;
    float linSleep;
    float angSleep;
    float restitution;
    elfVec3f anisFric;
    elfVec3f linearFactor;
    elfVec3f angularFactor;
    int propertyCount;
    elfProperty* property;
    int length;

    elfWriteNameToFile(actor->name, file);
    elfWriteNameToFile("", file);
    if (actor->script)
        elfWriteNameToFile(actor->script->name, file);
    else
        elfWriteNameToFile("", file);

    elfGetActorPosition_(actor, position);
    elfGetActorRotation_(actor, rotation);

    fwrite((char*)position, sizeof(float), 3, file);
    fwrite((char*)rotation, sizeof(float), 3, file);

    curveCount = elfGetListLength(actor->ipo->curves);
    fwrite((char*)&curveCount, sizeof(unsigned char), 1, file);

    for (curve = (elfBezierCurve*)elfBeginList(actor->ipo->curves); curve;
         curve = (elfBezierCurve*)elfGetListNext(actor->ipo->curves))
    {
        fwrite((char*)&curve->curveType, sizeof(unsigned char), 1, file);
        fwrite((char*)&curve->interpolation, sizeof(unsigned char), 1, file);

        pointCount = elfGetListLength(curve->points);
        fwrite((char*)&pointCount, sizeof(int), 1, file);
        for (point = (elfBezierPoint*)elfBeginList(curve->points); point;
             point = (elfBezierPoint*)elfGetListNext(curve->points))
        {
            fwrite((char*)&point->c1.x, sizeof(float), 2, file);
            fwrite((char*)&point->p.x, sizeof(float), 2, file);
            fwrite((char*)&point->c2.x, sizeof(float), 2, file);
        }
    }

    physics = elfGetActorPhysics(actor);
    shape = (unsigned char)elfGetActorShape(actor);
    boundingLengths = elfGetActorBoundingLengths(actor);
    boundingOffset = elfGetActorBoundingOffset(actor);
    mass = elfGetActorMass(actor);
    linDamp = elfGetActorLinearDamping(actor);
    angDamp = elfGetActorAngularDamping(actor);
    linSleep = elfGetActorLinearSleep(actor);
    angSleep = elfGetActorAngularSleep(actor);
    restitution = elfGetActorRestitution(actor);
    anisFric = elfGetActorAnisotropicFriction(actor);
    linearFactor = elfGetActorLinearFactor(actor);
    angularFactor = elfGetActorAngularFactor(actor);

    fwrite((char*)&physics, sizeof(unsigned char), 1, file);
    fwrite((char*)&shape, sizeof(unsigned char), 1, file);
    fwrite((char*)&boundingLengths.x, sizeof(float), 3, file);
    fwrite((char*)&boundingOffset.x, sizeof(float), 3, file);
    fwrite((char*)&mass, sizeof(float), 1, file);
    fwrite((char*)&linDamp, sizeof(float), 1, file);
    fwrite((char*)&angDamp, sizeof(float), 1, file);
    fwrite((char*)&linSleep, sizeof(float), 1, file);
    fwrite((char*)&angSleep, sizeof(float), 1, file);
    fwrite((char*)&restitution, sizeof(float), 1, file);
    fwrite((char*)&anisFric.x, sizeof(float), 3, file);
    fwrite((char*)&linearFactor.x, sizeof(float), 3, file);
    fwrite((char*)&angularFactor.x, sizeof(float), 3, file);

    propertyCount = elfGetListLength(actor->properties);
    fwrite((char*)&propertyCount, sizeof(int), 1, file);
    for (property = (elfProperty*)elfBeginList(actor->properties); property;
         property = (elfProperty*)elfGetListNext(actor->properties))
    {
        fwrite((char*)&property->propertyType, sizeof(unsigned char), 1, file);
        elfWriteNameToFile(property->name, file);

        if (property->propertyType == ELF_PROPERTY_STRING)
        {
            length = strlen(property->sval);
            fwrite((char*)&length, sizeof(int), 1, file);

            if (property->sval)
                fwrite(property->sval, sizeof(char), length, file);
        }
        else if (property->propertyType == ELF_PROPERTY_FLOAT)
            fwrite((char*)&property->fval, sizeof(float), 1, file);
        else if (property->propertyType == ELF_PROPERTY_INT)
            fwrite((char*)&property->fval, sizeof(int), 1, file);
        else if (property->propertyType == ELF_PROPERTY_BOOL)
            fwrite((char*)&property->fval, sizeof(unsigned char), 1, file);
    }
}

void elfWriteArmatureToFile(elfArmature* armature, FILE* file)
{
    int magic;
    elfBone* bone;
    int i, j;

    magic = ELF_ARMATURE_MAGIC;
    fwrite((char*)&magic, sizeof(int), 1, file);

    elfWriteNameToFile(armature->name, file);
    fwrite((char*)&armature->frameCount, sizeof(int), 1, file);
    fwrite((char*)&armature->boneCount, sizeof(int), 1, file);

    for (i = 0; i < (int)armature->boneCount; i++)
    {
        bone = armature->bones[i];
        if (!bone)
            continue;

        elfWriteNameToFile(bone->name, file);
        if (bone->parent)
            elfWriteNameToFile(bone->parent->name, file);
        else
            elfWriteNameToFile("", file);
        fwrite((char*)&bone->id, sizeof(int), 1, file);
        fwrite((char*)&bone->pos.x, sizeof(float), 3, file);
        fwrite((char*)&bone->qua.x, sizeof(float), 4, file);

        for (j = 0; j < armature->frameCount; j++)
        {
            fwrite((char*)&bone->frames[j].pos.x, sizeof(float), 3, file);
            fwrite((char*)&bone->frames[j].qua.x, sizeof(float), 4, file);
        }
    }
}

void elfWriteCameraToFile(elfCamera* camera, FILE* file)
{
    int magic;

    magic = 179532111;
    fwrite((char*)&magic, sizeof(int), 1, file);

    elfWriteActorHeader((elfActor*)camera, file);

    fwrite((char*)&camera->fov, sizeof(float), 1, file);
    fwrite((char*)&camera->clipNear, sizeof(float), 1, file);
    fwrite((char*)&camera->clipFar, sizeof(float), 1, file);
}

void elfWriteEntityToFile(elfEntity* entity, FILE* file)
{
    int magic = 0;
    float scale[3];
    unsigned int materialCount;
    elfMaterial* mat;

    magic = ELF_ENTITY_MAGIC;
    fwrite((char*)&magic, sizeof(int), 1, file);

    elfWriteActorHeader((elfActor*)entity, file);

    elfGetEntityScale_(entity, scale);
    fwrite((char*)scale, sizeof(float), 3, file);

    if (entity->model)
        elfWriteNameToFile(entity->model->name, file);
    else
        elfWriteNameToFile("", file);

    if (entity->armature)
        elfWriteNameToFile(entity->armature->name, file);
    else
        elfWriteNameToFile("", file);

    fwrite((char*)&entity->occluder, sizeof(unsigned char), 1, file);

    materialCount = elfGetEntityMaterialCount(entity);
    fwrite((char*)&materialCount, sizeof(unsigned int), 1, file);

    for (mat = (elfMaterial*)elfBeginList(entity->materials); mat;
         mat = (elfMaterial*)elfGetListNext(entity->materials))
    {
        elfWriteNameToFile(mat->name, file);
    }
}

void elfWriteLightToFile(elfLight* light, FILE* file)
{
    int magic = 0;

    magic = ELF_LIGHT_MAGIC;
    fwrite((char*)&magic, sizeof(int), 1, file);

    elfWriteActorHeader((elfActor*)light, file);

    fwrite((char*)&light->lightType, sizeof(unsigned char), 1, file);
    fwrite((char*)&light->color.r, sizeof(float), 4, file);
    fwrite((char*)&light->range, sizeof(float), 1, file);
    fwrite((char*)&light->fadeRange, sizeof(float), 1, file);
    fwrite((char*)&light->innerCone, sizeof(float), 1, file);
    fwrite((char*)&light->outerCone, sizeof(float), 1, file);
    fwrite((char*)&light->shadows, sizeof(unsigned char), 1, file);
    fwrite((char*)&light->shaft, sizeof(unsigned char), 1, file);
    fwrite((char*)&light->shaftSize, sizeof(float), 1, file);
    fwrite((char*)&light->shaftIntensity, sizeof(float), 1, file);
    fwrite((char*)&light->shaftFadeOff, sizeof(float), 1, file);
}

void elfWriteMaterialToFile(elfMaterial* material, FILE* file)
{
    int magic;

    magic = ELF_MATERIAL_MAGIC;
    fwrite((char*)&magic, sizeof(int), 1, file);

    elfWriteNameToFile(material->name, file);

    fwrite((char*)&material->diffuseColor.r, sizeof(float), 4, file);
    fwrite((char*)&material->ambientColor.r, sizeof(float), 4, file);
    fwrite((char*)&material->specularColor.r, sizeof(float), 4, file);
    fwrite((char*)&material->specPower, sizeof(float), 1, file);
    fwrite((char*)&material->lighting, sizeof(unsigned char), 1, file);

    if (material->diffuseMap)
        elfWriteNameToFile(material->diffuseMap->name, file);
    else
        elfWriteNameToFile("", file);
    if (material->normalMap)
        elfWriteNameToFile(material->normalMap->name, file);
    else
        elfWriteNameToFile("", file);
    if (material->heightMap)
        elfWriteNameToFile(material->heightMap->name, file);
    else
        elfWriteNameToFile("", file);
    if (material->specularMap)
        elfWriteNameToFile(material->specularMap->name, file);
    else
        elfWriteNameToFile("", file);
    if (material->lightMap)
        elfWriteNameToFile(material->lightMap->name, file);
    else
        elfWriteNameToFile("", file);

    fwrite((char*)&material->parallaxScale, sizeof(float), 1, file);
    fwrite((char*)&material->alphaTest, sizeof(unsigned char), 1, file);
    fwrite((char*)&material->alphaThreshold, sizeof(float), 1, file);
}

void elfWriteModelToFile(elfModel* model, FILE* file)
{
    int magic = 0;
    unsigned char isNormals;
    unsigned char isTexCoords;
    unsigned char isWeightsAndBoneids;
    unsigned char junk;
    int i = 0;
    short int boneids[4];

    magic = ELF_MODEL_MAGIC;
    fwrite((char*)&magic, sizeof(int), 1, file);

    elfWriteNameToFile(model->name, file);

    isNormals = 1;
    isTexCoords = 0;
    isWeightsAndBoneids = 0;
    junk = 0;
    if (model->texCoords)
        isTexCoords = 1;
    if (model->weights && model->boneids)
        isWeightsAndBoneids = 1;

    fwrite((char*)&model->verticeCount, sizeof(int), 1, file);
    fwrite((char*)&model->frameCount, sizeof(int), 1, file);
    fwrite((char*)&model->indiceCount, sizeof(int), 1, file);
    fwrite((char*)&model->areaCount, sizeof(int), 1, file);
    fwrite((char*)&isNormals, sizeof(unsigned char), 1, file);
    fwrite((char*)&isTexCoords, sizeof(unsigned char), 1, file);
    fwrite((char*)&isWeightsAndBoneids, sizeof(unsigned char), 1, file);
    fwrite((char*)&junk, sizeof(unsigned char), 1, file);

    fwrite((char*)gfxGetVertexDataBuffer(model->vertices), sizeof(float), 3 * model->verticeCount, file);

    for (i = 0; i < model->areaCount; i++)
    {
        fwrite((char*)&model->areas[i].indiceCount, sizeof(int), 1, file);
        if (model->areas[i].indiceCount)
        {
            fwrite((char*)gfxGetVertexDataBuffer(model->areas[i].index), sizeof(unsigned int),
                   model->areas[i].indiceCount, file);
        }
    }

    fwrite((char*)gfxGetVertexDataBuffer(model->normals), sizeof(float), 3 * model->verticeCount, file);

    // read tex coords
    if (isTexCoords > 0)
        fwrite((char*)gfxGetVertexDataBuffer(model->texCoords), sizeof(float), 2 * model->verticeCount, file);

    // read weights and bone ids
    if (isWeightsAndBoneids > 0)
    {
        fwrite((char*)model->weights, sizeof(float), 4 * model->verticeCount, file);

        for (i = 0; i < model->verticeCount; i++)
        {
            boneids[0] = model->boneids[i * 4];
            boneids[1] = model->boneids[i * 4 + 1];
            boneids[2] = model->boneids[i * 4 + 2];
            boneids[3] = model->boneids[i * 4 + 3];

            fwrite((char*)boneids, sizeof(short int), 4, file);
        }
    }
}

void elfWriteParticlesToFile(elfParticles* particles, FILE* file)
{
    int magic = 0;

    magic = ELF_PARTICLES_MAGIC;
    fwrite((char*)&magic, sizeof(int), 1, file);

    elfWriteActorHeader((elfActor*)particles, file);

    if (particles->texture)
        elfWriteNameToFile(particles->texture->name, file);
    else
        elfWriteNameToFile("", file);

    if (particles->model)
        elfWriteNameToFile(particles->model->name, file);
    else
        elfWriteNameToFile("", file);

    if (particles->entity)
        elfWriteNameToFile(particles->entity->name, file);
    else
        elfWriteNameToFile("", file);

    fwrite((char*)&particles->maxCount, sizeof(int), 1, file);
    fwrite((char*)&particles->drawMode, sizeof(unsigned char), 1, file);

    fwrite((char*)&particles->spawnCount, sizeof(int), 1, file);
    fwrite((char*)&particles->spawn, sizeof(unsigned char), 1, file);
    fwrite((char*)&particles->gravity.x, sizeof(float), 3, file);
    fwrite((char*)&particles->sizeMin, sizeof(float), 1, file);
    fwrite((char*)&particles->sizeMax, sizeof(float), 1, file);
    fwrite((char*)&particles->sizeGrowthMin, sizeof(float), 1, file);
    fwrite((char*)&particles->sizeGrowthMax, sizeof(float), 1, file);
    fwrite((char*)&particles->rotationMin, sizeof(float), 1, file);
    fwrite((char*)&particles->rotationMax, sizeof(float), 1, file);
    fwrite((char*)&particles->rotationGrowthMin, sizeof(float), 1, file);
    fwrite((char*)&particles->rotationGrowthMax, sizeof(float), 1, file);
    fwrite((char*)&particles->lifeSpanMin, sizeof(float), 1, file);
    fwrite((char*)&particles->lifeSpanMax, sizeof(float), 1, file);
    fwrite((char*)&particles->fadeSpeedMin, sizeof(float), 1, file);
    fwrite((char*)&particles->fadeSpeedMax, sizeof(float), 1, file);
    fwrite((char*)&particles->velocityMin.x, sizeof(float), 3, file);
    fwrite((char*)&particles->velocityMax.x, sizeof(float), 3, file);
    fwrite((char*)&particles->positionMin.x, sizeof(float), 3, file);
    fwrite((char*)&particles->positionMax.x, sizeof(float), 3, file);
    fwrite((char*)&particles->colorMin.r, sizeof(float), 4, file);
    fwrite((char*)&particles->colorMax.r, sizeof(float), 4, file);
}

void elfWriteSceneToFile(elfScene* scene, FILE* file)
{
    int magic;

    magic = ELF_SCENE_MAGIC;
    fwrite((char*)&magic, sizeof(int), 1, file);

    elfWriteNameToFile(scene->name, file);

    fwrite((char*)&scene->ambientColor.r, sizeof(float), 4, file);
}

void elfWriteScriptToFile(elfScript* script, FILE* file)
{
    int magic = 0;
    unsigned int length;

    magic = ELF_SCRIPT_MAGIC;
    fwrite((char*)&magic, sizeof(int), 1, file);

    elfWriteNameToFile(script->name, file);

    length = strlen(script->text);
    fwrite((char*)&length, sizeof(unsigned int), 1, file);
    if (length > 0)
    {
        fwrite(script->text, sizeof(char), length, file);
    }
}

void elfWriteSpriteToFile(elfSprite* sprite, FILE* file)
{
    int magic = 0;

    magic = ELF_SPRITE_MAGIC;
    fwrite((char*)&magic, sizeof(int), 1, file);

    elfWriteActorHeader((elfActor*)sprite, file);

    fwrite((char*)&sprite->scale.x, sizeof(float), 2, file);

    if (sprite->material)
        elfWriteNameToFile(sprite->material->name, file);
    else
        elfWriteNameToFile("", file);

    fwrite((char*)&sprite->faceCamera, sizeof(unsigned char), 1, file);
}

void elfWriteTextureToFile(elfTexture* texture, FILE* file)
{
    int magic;
    unsigned char type;

    magic = ELF_TEXTURE_MAGIC;
    fwrite((char*)&magic, sizeof(int), 1, file);

    elfWriteNameToFile(texture->name, file);

    type = 1;
    fwrite((char*)&type, sizeof(unsigned char), 1, file);

    fwrite((char*)&texture->dataSize, sizeof(int), 1, file);
    fwrite((char*)texture->data, 1, texture->dataSize, file);
}

void elfWriteResourceIndexToFile(elfResource* resource, unsigned int* offset, FILE* file)
{
    unsigned char ucval;
    unsigned int ival;

    ucval = resource->objType;
    fwrite((char*)&ucval, sizeof(unsigned char), 1, file);

    elfWriteNameToFile(resource->name, file);

    ival = *offset;
    fwrite((char*)&ival, sizeof(int), 1, file);

    switch (resource->objType)
    {
    case ELF_SCENE:
        *offset += elfGetSceneSizeBytes((elfScene*)resource);
        break;
    case ELF_SCRIPT:
        *offset += elfGetScriptSizeBytes((elfScript*)resource);
        break;
    case ELF_TEXTURE:
        *offset += elfGetTextureSizeBytes((elfTexture*)resource);
        break;
    case ELF_MODEL:
        *offset += elfGetModelSizeBytes((elfModel*)resource);
        break;
    case ELF_MATERIAL:
        *offset += elfGetMaterialSizeBytes((elfMaterial*)resource);
        break;
    case ELF_CAMERA:
        *offset += elfGetCameraSizeBytes((elfCamera*)resource);
        break;
    case ELF_ENTITY:
        *offset += elfGetEntitySizeBytes((elfEntity*)resource);
        break;
    case ELF_LIGHT:
        *offset += elfGetLightSizeBytes((elfLight*)resource);
        break;
    case ELF_PARTICLES:
        *offset += elfGetParticlesSizeBytes((elfParticles*)resource);
        break;
    case ELF_SPRITE:
        *offset += elfGetSpriteSizeBytes((elfSprite*)resource);
        break;
    case ELF_ARMATURE:
        *offset += elfGetArmatureSizeBytes((elfArmature*)resource);
        break;
    }
}

void elfWriteResourceIndexesToFile(elfList* resources, unsigned int* offset, FILE* file)
{
    elfResource* res;

    for (res = (elfResource*)elfBeginList(resources); res; res = (elfResource*)elfGetListNext(resources))
    {
        elfWriteResourceIndexToFile(res, offset, file);
    }
}

void elfWriteResourcesToFile(elfList* resources, FILE* file)
{
    elfResource* res;

    for (res = (elfResource*)elfBeginList(resources); res; res = (elfResource*)elfGetListNext(resources))
    {
        switch (res->objType)
        {
        case ELF_SCENE:
            elfWriteSceneToFile((elfScene*)res, file);
            break;
        case ELF_SCRIPT:
            elfWriteScriptToFile((elfScript*)res, file);
            break;
        case ELF_TEXTURE:
            elfWriteTextureToFile((elfTexture*)res, file);
            break;
        case ELF_MODEL:
            elfWriteModelToFile((elfModel*)res, file);
            break;
        case ELF_MATERIAL:
            elfWriteMaterialToFile((elfMaterial*)res, file);
            break;
        case ELF_CAMERA:
            elfWriteCameraToFile((elfCamera*)res, file);
            break;
        case ELF_ENTITY:
            elfWriteEntityToFile((elfEntity*)res, file);
            break;
        case ELF_LIGHT:
            elfWriteLightToFile((elfLight*)res, file);
            break;
        case ELF_PARTICLES:
            elfWriteParticlesToFile((elfParticles*)res, file);
            break;
        case ELF_SPRITE:
            elfWriteSpriteToFile((elfSprite*)res, file);
            break;
        case ELF_ARMATURE:
            elfWriteArmatureToFile((elfArmature*)res, file);
            break;
        }
    }
}

void elfAddTextureForSaving(elfList* textures, elfTexture* texture)
{
    if (texture && !elfGetResourceById(textures, texture->id))
    {
        if (elfLoadTextureData(texture))
        {
            elfSetResourceUniqueName(textures, (elfResource*)texture);
            elfAppendListObject(textures, (elfObject*)texture);
        }
    }
    else
    {
        return;
    }
}

unsigned char elfSaveSceneToPak(elfScene* scene, const char* filePath)
{
    unsigned int offset;
    int ival;

    FILE* file;

    elfList* scenes;
    elfList* scripts;
    elfList* textures;
    elfList* materials;
    elfList* models;
    elfList* cameras;
    elfList* entities;
    elfList* lights;
    elfList* armatures;
    elfList* particles;
    elfList* sprites;

    elfMaterial* mat;
    elfCamera* cam;
    elfEntity* ent;
    elfLight* lig;
    elfParticles* par;
    elfSprite* spr;

    scenes = elfCreateList();
    scripts = elfCreateList();
    textures = elfCreateList();
    materials = elfCreateList();
    models = elfCreateList();
    cameras = elfCreateList();
    entities = elfCreateList();
    lights = elfCreateList();
    armatures = elfCreateList();
    particles = elfCreateList();
    sprites = elfCreateList();

    elfIncRef((elfObject*)scenes);
    elfIncRef((elfObject*)scripts);
    elfIncRef((elfObject*)textures);
    elfIncRef((elfObject*)materials);
    elfIncRef((elfObject*)models);
    elfIncRef((elfObject*)cameras);
    elfIncRef((elfObject*)entities);
    elfIncRef((elfObject*)lights);
    elfIncRef((elfObject*)armatures);
    elfIncRef((elfObject*)particles);
    elfIncRef((elfObject*)sprites);

    elfAppendListObject(scenes, (elfObject*)scene);

    for (cam = (elfCamera*)elfBeginList(scene->cameras); cam; cam = (elfCamera*)elfGetListNext(scene->cameras))
    {
        if (cam->script && !elfGetResourceById(scripts, cam->script->id))
        {
            elfSetResourceUniqueName(scripts, (elfResource*)cam->script);
            elfAppendListObject(scripts, (elfObject*)cam->script);
        }

        elfSetResourceUniqueName(cameras, (elfResource*)cam);
        elfAppendListObject(cameras, (elfObject*)cam);
    }

    for (ent = (elfEntity*)elfBeginList(scene->entities); ent; ent = (elfEntity*)elfGetListNext(scene->entities))
    {
        if (ent->script && !elfGetResourceById(scripts, ent->script->id))
        {
            elfSetResourceUniqueName(scripts, (elfResource*)ent->script);
            elfAppendListObject(scripts, (elfObject*)ent->script);
        }

        if (ent->model && !elfGetResourceById(models, ent->model->id))
        {
            elfSetResourceUniqueName(models, (elfResource*)ent->model);
            elfAppendListObject(models, (elfObject*)ent->model);
        }

        if (ent->armature && !elfGetResourceById(armatures, ent->armature->id))
        {
            elfSetResourceUniqueName(armatures, (elfResource*)ent->armature);
            elfAppendListObject(armatures, (elfObject*)ent->armature);
        }

        for (mat = (elfMaterial*)elfBeginList(ent->materials); mat; mat = (elfMaterial*)elfGetListNext(ent->materials))
        {
            if (!elfGetResourceById(materials, mat->id))
            {
                elfSetResourceUniqueName(materials, (elfResource*)mat);
                elfAppendListObject(materials, (elfObject*)mat);

                elfAddTextureForSaving(textures, mat->diffuseMap);
                elfAddTextureForSaving(textures, mat->normalMap);
                elfAddTextureForSaving(textures, mat->heightMap);
                elfAddTextureForSaving(textures, mat->specularMap);
                elfAddTextureForSaving(textures, mat->lightMap);
            }
        }

        elfSetResourceUniqueName(entities, (elfResource*)ent);
        elfAppendListObject(entities, (elfObject*)ent);
    }

    for (lig = (elfLight*)elfBeginList(scene->lights); lig; lig = (elfLight*)elfGetListNext(scene->lights))
    {
        if (lig->script && !elfGetResourceById(scripts, lig->script->id))
        {
            elfSetResourceUniqueName(scripts, (elfResource*)lig->script);
            elfAppendListObject(scripts, (elfObject*)lig->script);
        }

        elfSetResourceUniqueName(lights, (elfResource*)lig);
        elfAppendListObject(lights, (elfObject*)lig);
    }

    for (par = (elfParticles*)elfBeginList(scene->particles); par;
         par = (elfParticles*)elfGetListNext(scene->particles))
    {
        if (par->script && !elfGetResourceById(scripts, par->script->id))
        {
            elfSetResourceUniqueName(scripts, (elfResource*)par->script);
            elfAppendListObject(scripts, (elfObject*)par->script);
        }

        if (par->texture && !elfGetResourceById(textures, par->texture->id))
        {
            if (elfLoadTextureData(par->texture))
            {
                elfSetResourceUniqueName(textures, (elfResource*)par->texture);
                elfAppendListObject(textures, (elfObject*)par->texture);
            }
        }

        if (par->model && !elfGetResourceById(models, par->model->id))
        {
            elfSetResourceUniqueName(models, (elfResource*)par->model);
            elfAppendListObject(models, (elfObject*)par->model);
        }

        if (par->entity && !elfGetResourceById(entities, par->entity->id))
        {
            elfSetResourceUniqueName(entities, (elfResource*)par->entity);
            elfAppendListObject(entities, (elfObject*)par->entity);
        }

        elfSetResourceUniqueName(particles, (elfResource*)par);
        elfAppendListObject(particles, (elfObject*)par);
    }

    for (spr = (elfSprite*)elfBeginList(scene->sprites); spr; spr = (elfSprite*)elfGetListNext(scene->sprites))
    {
        if (spr->script && !elfGetResourceById(scripts, spr->script->id))
        {
            elfSetResourceUniqueName(scripts, (elfResource*)spr->script);
            elfAppendListObject(scripts, (elfObject*)spr->script);
        }

        mat = spr->material;

        if (mat && !elfGetResourceById(materials, mat->id))
        {
            elfSetResourceUniqueName(materials, (elfResource*)mat);
            elfAppendListObject(materials, (elfObject*)mat);

            elfAddTextureForSaving(textures, mat->diffuseMap);
            elfAddTextureForSaving(textures, mat->normalMap);
            elfAddTextureForSaving(textures, mat->heightMap);
            elfAddTextureForSaving(textures, mat->specularMap);
            elfAddTextureForSaving(textures, mat->lightMap);
        }

        elfSetResourceUniqueName(sprites, (elfResource*)spr);
        elfAppendListObject(sprites, (elfObject*)spr);
    }

    file = fopen(filePath, "wb");
    if (!file)
    {
        elfSetError(ELF_CANT_OPEN_FILE, "error: can't open file \"%s\" for writing\n", filePath);

        elfDecRef((elfObject*)scenes);
        elfDecRef((elfObject*)scripts);
        elfDecRef((elfObject*)textures);
        elfDecRef((elfObject*)materials);
        elfDecRef((elfObject*)models);
        elfDecRef((elfObject*)cameras);
        elfDecRef((elfObject*)entities);
        elfDecRef((elfObject*)lights);
        elfDecRef((elfObject*)armatures);
        elfDecRef((elfObject*)particles);
        elfDecRef((elfObject*)sprites);

        return false;
    }

    offset = 0;
    offset += sizeof(unsigned char);           // index type
    offset += sizeof(char) * ELF_NAME_LENGTH;  // index name
    offset += sizeof(int);                     // index offset
    offset *= elfGetListLength(scenes) + elfGetListLength(scripts) + elfGetListLength(textures) +
              elfGetListLength(materials) + elfGetListLength(models) + elfGetListLength(cameras) +
              elfGetListLength(entities) + elfGetListLength(lights) + elfGetListLength(armatures) +
              elfGetListLength(particles) + elfGetListLength(sprites);

    offset += sizeof(int);  // magic
    offset += sizeof(int);  // version
    offset += sizeof(int);  // number of indexes

    ival = ELF_PAK_MAGIC;

    fwrite((char*)&ival, sizeof(int), 1, file);  // magic

    ival = ELF_PAK_VERSION;

    fwrite((char*)&ival, sizeof(int), 1, file);  // pak version

    ival = elfGetListLength(scenes) + elfGetListLength(scripts) + elfGetListLength(textures) +
           elfGetListLength(materials) + elfGetListLength(models) + elfGetListLength(cameras) +
           elfGetListLength(entities) + elfGetListLength(lights) + elfGetListLength(armatures) +
           elfGetListLength(particles) + elfGetListLength(sprites);

    fwrite((char*)&ival, sizeof(int), 1, file);  // index count

    elfWriteResourceIndexesToFile(scenes, &offset, file);
    elfWriteResourceIndexesToFile(scripts, &offset, file);
    elfWriteResourceIndexesToFile(textures, &offset, file);
    elfWriteResourceIndexesToFile(materials, &offset, file);
    elfWriteResourceIndexesToFile(models, &offset, file);
    elfWriteResourceIndexesToFile(cameras, &offset, file);
    elfWriteResourceIndexesToFile(entities, &offset, file);
    elfWriteResourceIndexesToFile(lights, &offset, file);
    elfWriteResourceIndexesToFile(armatures, &offset, file);
    elfWriteResourceIndexesToFile(particles, &offset, file);
    elfWriteResourceIndexesToFile(sprites, &offset, file);

    elfWriteResourcesToFile(scenes, file);
    elfWriteResourcesToFile(scripts, file);
    elfWriteResourcesToFile(textures, file);
    elfWriteResourcesToFile(materials, file);
    elfWriteResourcesToFile(models, file);
    elfWriteResourcesToFile(cameras, file);
    elfWriteResourcesToFile(entities, file);
    elfWriteResourcesToFile(lights, file);
    elfWriteResourcesToFile(armatures, file);
    elfWriteResourcesToFile(particles, file);
    elfWriteResourcesToFile(sprites, file);

    fclose(file);

    elfDecRef((elfObject*)scenes);
    elfDecRef((elfObject*)scripts);
    elfDecRef((elfObject*)textures);
    elfDecRef((elfObject*)materials);
    elfDecRef((elfObject*)models);
    elfDecRef((elfObject*)cameras);
    elfDecRef((elfObject*)entities);
    elfDecRef((elfObject*)lights);
    elfDecRef((elfObject*)armatures);
    elfDecRef((elfObject*)particles);
    elfDecRef((elfObject*)sprites);

    return true;
}
