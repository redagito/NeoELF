#include "nelf/resource/Armature.h"

#include <cstdlib>
#include <cstring>

#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/Object.h"
#include "nelf/RenderStation.h"
#include "nelf/String.h"
#include "nelf/actor/Entity.h"
#include "nelf/gfx/gfxFormatType.h"
#include "nelf/gfx/gfxMath.h"
#include "nelf/gfx/gfxObject.h"
#include "nelf/gfx/gfxShaderParams.h"
#include "nelf/gfx/gfxVertexData.h"
#include "nelf/gfx/gfxVertexDataType.h"
#include "nelf/objectType.h"
#include "nelf/resource/Model.h"
#include "nelf/resource/Resources.h"

elfBone* elfCreateBone(const char* name)
{
    elfBone* bone;

    bone = (elfBone*)malloc(sizeof(elfBone));
    memset(bone, 0x0, sizeof(elfBone));
    bone->objType = ELF_BONE;
    bone->objDestr = elfDestroyBone;

    if (name)
        bone->name = elfCreateString(name);

    bone->children = elfCreateList();
    elfIncRef((elfObject*)bone->children);

    elfIncObj(ELF_BONE);

    return bone;
}

void elfDestroyBone(void* data)
{
    elfBone* bone = (elfBone*)data;

    if (bone->name)
        elfDestroyString(bone->name);
    if (bone->frames)
        free(bone->frames);

    elfDecRef((elfObject*)bone->children);

    free(bone);

    elfDecObj(ELF_BONE);
}

elfArmature* elfGetBoneArmature(elfBone* bone) { return bone->armature; }

elfBone* elfGetBoneParent(elfBone* bone) { return bone->parent; }

elfBone* elfGetBoneChildByName(elfBone* bone, const char* name)
{
    elfBone* cbone;

    for (cbone = (elfBone*)elfBeginList(bone->children); cbone; cbone = (elfBone*)elfGetListNext(bone->children))
    {
        if (!strcmp(cbone->name, name))
        {
            return cbone;
        }
    }

    return nullptr;
}

elfBone* elfGetBoneChildById(elfBone* bone, int id)
{
    elfBone* cbone;

    for (cbone = (elfBone*)elfBeginList(bone->children); cbone; cbone = (elfBone*)elfGetListNext(bone->children))
    {
        if (cbone->id == id)
        {
            return cbone;
        }
    }

    return nullptr;
}

elfBone* elfGetBoneChildByIndex(elfBone* bone, int idx) { return (elfBone*)elfGetListObject(bone->children, idx); }

elfVec3f elfGetBonePosition(elfBone* bone) { return bone->curPos; }

elfVec3f elfGetBoneRotation(elfBone* bone)
{
    elfVec3f result;

    gfxQuaToEuler(&bone->curQua.x, &result.x);

    return result;
}

elfVec4f elfGetBoneOrientation(elfBone* bone) { return bone->qua; }

void elfSetBoneArmature(elfBone* bone, elfArmature* armature)
{
    elfBone* cbone;

    bone->armature = armature;

    for (cbone = (elfBone*)elfBeginList(bone->children); cbone; cbone = (elfBone*)elfGetListNext(bone->children))
    {
        elfSetBoneArmature(cbone, armature);
    }
}

int elfGetBoneMaxId(elfBone* bone)
{
    int maxId;
    int curMaxId;
    elfBone* cbone;

    maxId = bone->id;

    for (cbone = (elfBone*)elfBeginList(bone->children); cbone; cbone = (elfBone*)elfGetListNext(bone->children))
    {
        curMaxId = elfGetBoneMaxId(cbone);
        if (curMaxId > maxId)
            maxId = curMaxId;
    }

    return maxId;
}

void elfPopulateBoneArray(elfBone* bone, elfBone** bones)
{
    elfBone* cbone;

    bones[bone->id] = bone;

    for (cbone = (elfBone*)elfBeginList(bone->children); cbone; cbone = (elfBone*)elfGetListNext(bone->children))
    {
        elfPopulateBoneArray(cbone, bones);
    }
}

void elfUpdateArmatureBones(elfArmature* armature)
{
    int maxId;
    int curMaxId;
    elfBone* cbone;

    maxId = 0;
    for (cbone = (elfBone*)elfBeginList(armature->rootBones); cbone;
         cbone = (elfBone*)elfGetListNext(armature->rootBones))
    {
        curMaxId = elfGetBoneMaxId(cbone);
        if (curMaxId > maxId)
            maxId = curMaxId;
    }

    if (armature->bones)
        free(armature->bones);
    armature->bones = (elfBone**)malloc(sizeof(elfBone*) * (maxId + 1));
    memset(armature->bones, 0x0, sizeof(elfBone*) * (maxId + 1));

    armature->boneCount = maxId + 1;

    for (cbone = (elfBone*)elfBeginList(armature->rootBones); cbone;
         cbone = (elfBone*)elfGetListNext(armature->rootBones))
    {
        elfPopulateBoneArray(cbone, armature->bones);
    }
}

elfArmature* elfCreateArmature(const char* name)
{
    elfArmature* armature;

    armature = (elfArmature*)malloc(sizeof(elfArmature));
    memset(armature, 0x0, sizeof(elfArmature));
    armature->objType = ELF_ARMATURE;
    armature->objDestr = elfDestroyArmature;

    armature->rootBones = elfCreateList();
    elfIncRef((elfObject*)armature->rootBones);

    if (name)
        armature->name = elfCreateString(name);

    armature->id = ++res->armatureIdCounter;

    elfIncObj(ELF_ARMATURE);

    return armature;
}

void elfDeformEntityWithArmature(elfArmature* armature, elfEntity* entity, float frame)
{
    int i, j;
    int id;
    float tempVec1[3];
    float tempVec2[3];
    int cid;
    int nid;
    float t;
    float tempQua[4];
    float* vertexBuffer;
    float* normalBuffer;
    float* origVertexBuffer;
    float* origNormalBuffer;
    elfBone* bone;
    elfModel* model;

    model = elfGetEntityModel(entity);

    if (!model || !armature->boneCount || !model->boneids || !model->weights)
        return;

    cid = ((int)frame) - 1;
    if (cid < 0)
        cid = 0;
    nid = cid + 1;
    if (frame > armature->frameCount)
        cid = nid = armature->frameCount - 1;
    t = frame - ((int)frame);

    armature->curFrame = frame;
    if (armature->curFrame > armature->frameCount)
        armature->curFrame = armature->frameCount;

    for (i = 0; i < armature->boneCount; i++)
    {
        bone = armature->bones[i];
        if (!bone)
            continue;

        bone->curOffsetPos.x =
            bone->frames[cid].offsetPos.x + (bone->frames[nid].offsetPos.x - bone->frames[cid].offsetPos.x) * t;
        bone->curOffsetPos.y =
            bone->frames[cid].offsetPos.y + (bone->frames[nid].offsetPos.y - bone->frames[cid].offsetPos.y) * t;
        bone->curOffsetPos.z =
            bone->frames[cid].offsetPos.z + (bone->frames[nid].offsetPos.z - bone->frames[cid].offsetPos.z) * t;
        bone->curPos.x = bone->pos.x + bone->curOffsetPos.x;
        bone->curPos.y = bone->pos.y + bone->curOffsetPos.y;
        bone->curPos.z = bone->pos.z + bone->curOffsetPos.z;

        gfxQuaSlerp(&bone->frames[cid].offsetQua.x, &bone->frames[nid].offsetQua.x, t, &bone->curOffsetQua.x);
        gfxMulQuaQua(&bone->qua.x, &bone->curOffsetQua.x, tempQua);
        memcpy(&bone->curQua.x, tempQua, sizeof(float) * 4);
    }

    if (!entity->vertices)
    {
        entity->vertices = gfxCreateVertexData(3 * model->verticeCount, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
        gfxIncRef((gfxObject*)entity->vertices);
    }

    if (!entity->normals)
    {
        entity->normals = gfxCreateVertexData(3 * model->verticeCount, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
        gfxIncRef((gfxObject*)entity->normals);
    }

    vertexBuffer = (float*)gfxGetVertexDataBuffer(entity->vertices);
    normalBuffer = (float*)gfxGetVertexDataBuffer(entity->normals);
    origVertexBuffer = (float*)gfxGetVertexDataBuffer(model->vertices);
    origNormalBuffer = (float*)gfxGetVertexDataBuffer(model->normals);

    for (i = 0; i < model->verticeCount; i++)
    {
        memset(&vertexBuffer[i * 3], 0x0, sizeof(float) * 3);
        memset(&normalBuffer[i * 3], 0x0, sizeof(float) * 3);

        for (j = 0; j < 4; j++)
        {
            id = model->boneids[i * 4 + j];

            if (id < 0 || id > (int)armature->boneCount - 1 || !(bone = armature->bones[id]))
                continue;

            memcpy(tempVec1, &origVertexBuffer[i * 3], sizeof(float) * 3);
            tempVec1[0] -= bone->pos.x;
            tempVec1[1] -= bone->pos.y;
            tempVec1[2] -= bone->pos.z;
            gfxMulQuaVec(&bone->curOffsetQua.x, tempVec1, tempVec2);
            tempVec2[0] += bone->pos.x;
            tempVec2[1] += bone->pos.y;
            tempVec2[2] += bone->pos.z;
            tempVec2[0] += bone->curOffsetPos.x;
            tempVec2[1] += bone->curOffsetPos.y;
            tempVec2[2] += bone->curOffsetPos.z;

            vertexBuffer[i * 3] += tempVec2[0] * model->weights[i * 4 + j];
            vertexBuffer[i * 3 + 1] += tempVec2[1] * model->weights[i * 4 + j];
            vertexBuffer[i * 3 + 2] += tempVec2[2] * model->weights[i * 4 + j];

            gfxMulQuaVec(&bone->curOffsetQua.x, &origNormalBuffer[i * 3], tempVec1);

            normalBuffer[i * 3] += tempVec1[0] * model->weights[i * 4 + j];
            normalBuffer[i * 3 + 1] += tempVec1[1] * model->weights[i * 4 + j];
            normalBuffer[i * 3 + 2] += tempVec1[2] * model->weights[i * 4 + j];
        }
    }

    gfxUpdateVertexData(entity->vertices);
    gfxUpdateVertexData(entity->normals);
}

void elfDestroyArmature(void* data)
{
    elfArmature* armature = (elfArmature*)data;

    if (armature->name)
        elfDestroyString(armature->name);
    if (armature->filePath)
        elfDestroyString(armature->filePath);

    elfDecRef((elfObject*)armature->rootBones);

    if (armature->bones)
        free(armature->bones);

    free(armature);

    elfDecObj(ELF_ARMATURE);
}

elfBone* elfGetBoneFromArmatureByName(const char* name, elfArmature* armature)
{
    int i;

    for (i = 0; i < (int)armature->boneCount; i++)
    {
        if (armature->bones[i] && !strcmp(armature->bones[i]->name, name))
        {
            return armature->bones[i];
        }
    }

    return NULL;
}

elfBone* elfGetBoneFromArmatureById(int id, elfArmature* armature)
{
    if (id < 0 || id > armature->boneCount - 1)
        return NULL;

    return armature->bones[id];
}

void elfAddRootBoneToArmature(elfArmature* armature, elfBone* bone)
{
    if (bone->parent)
        return;
    elfAppendListObject(armature->rootBones, (elfObject*)bone);
    elfSetBoneArmature(bone, armature);
    elfUpdateArmatureBones(armature);
}

void elfDrawBoneHierarchy(elfBone* bone, gfxShaderParams* shaderParams)
{
    elfBone* cbone;
    elfVec3f pos;
    elfVec3f axis;
    elfVec4f orient;
    float* vertexBuffer;

    pos = bone->curPos;
    orient = bone->curQua;

    vertexBuffer = (float*)gfxGetVertexDataBuffer(renderStation->lines);

    vertexBuffer[0] = pos.x;
    vertexBuffer[1] = pos.y;
    vertexBuffer[2] = pos.z;

    axis.x = 0.2f;
    axis.y = 0.0f;
    axis.z = 0.0f;
    axis = elfMulQuaVec3f(orient, axis);
    vertexBuffer[3] = pos.x + axis.x;
    vertexBuffer[4] = pos.y + axis.y;
    vertexBuffer[5] = pos.z + axis.z;

    gfxSetColor(&shaderParams->materialParams.diffuseColor, 0.0f, 0.0f, 1.0f, 1.0f);
    gfxSetShaderParams(shaderParams);

    elfDrawLines(2, renderStation->lines);

    axis.x = 0.0f;
    axis.y = 0.2f;
    axis.z = 0.0f;
    axis = elfMulQuaVec3f(orient, axis);
    vertexBuffer[3] = pos.x + axis.x;
    vertexBuffer[4] = pos.y + axis.y;
    vertexBuffer[5] = pos.z + axis.z;

    gfxSetColor(&shaderParams->materialParams.diffuseColor, 0.0f, 1.0f, 0.0f, 1.0f);
    gfxSetShaderParams(shaderParams);

    elfDrawLines(2, renderStation->lines);

    axis.x = 0.0f;
    axis.y = 0.0f;
    axis.z = 0.2f;
    axis = elfMulQuaVec3f(orient, axis);
    vertexBuffer[3] = pos.x + axis.x;
    vertexBuffer[4] = pos.y + axis.y;
    vertexBuffer[5] = pos.z + axis.z;

    gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 0.0f, 0.0f, 1.0f);
    gfxSetShaderParams(shaderParams);

    elfDrawLines(2, renderStation->lines);

    for (cbone = (elfBone*)elfBeginList(bone->children); cbone; cbone = (elfBone*)elfGetListNext(bone->children))
    {
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 0.5f, 0.2f, 1.0f);
        gfxSetShaderParams(shaderParams);

        vertexBuffer[3] = cbone->curPos.x;
        vertexBuffer[4] = cbone->curPos.y;
        vertexBuffer[5] = cbone->curPos.z;

        elfDrawLines(2, renderStation->lines);

        elfDrawBoneHierarchy(cbone, shaderParams);
    }
}

void elfDrawArmatureDebug(elfArmature* armature, gfxShaderParams* shaderParams)
{
    elfBone* bone;

    for (bone = (elfBone*)elfBeginList(armature->rootBones); bone; bone = (elfBone*)elfGetListNext(armature->rootBones))
    {
        elfDrawBoneHierarchy(bone, shaderParams);
    }
}