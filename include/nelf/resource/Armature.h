#pragma once

#include "nelf/Vector.h"
#include "nelf/resourceHeader.h"

struct elfList;
struct elfEntity;
struct gfxShaderParams;

struct elfBoneFrame
{
    elfVec3f pos;
    elfVec4f qua;
    elfVec3f offsetPos;
    elfVec4f offsetQua;
};

struct elfBone
{
    ELF_RESOURCE_HEADER;
    elfBone* parent;
    elfVec3f pos;
    elfVec4f qua;
    elfVec3f curPos;
    elfVec4f curQua;
    elfVec3f curOffsetPos;
    elfVec4f curOffsetQua;
    elfBoneFrame* frames;
    elfList* children;
    elfArmature* armature;
};

// TODO Should probably be called elfSkeleton instead?
struct elfArmature
{
    ELF_RESOURCE_HEADER;
    char* filePath;
    int frameCount;
    int boneCount;
    elfList* rootBones;
    elfBone** bones;
    float curFrame;
    elfVec3f bbMin;
    elfVec3f bbMax;
};

// Bone

elfBone* elfCreateBone(const char* name);
void elfDestroyBone(void* data);

elfArmature* elfGetBoneArmature(elfBone* bone);

elfBone* elfGetBoneParent(elfBone* bone);
elfBone* elfGetBoneChildByName(elfBone* bone, const char* name);
elfBone* elfGetBoneChildById(elfBone* bone, int id);
elfBone* elfGetBoneChildByIndex(elfBone* bone, int idx);
elfVec3f elfGetBonePosition(elfBone* bone);
elfVec3f elfGetBoneRotation(elfBone* bone);
elfVec4f elfGetBoneOrientation(elfBone* bone);

void elfSetBoneArmature(elfBone* bone, elfArmature* armature);

int elfGetBoneMaxId(elfBone* bone);

void elfPopulateBoneArray(elfBone* bone, elfBone** bones);

// Armature

void elfUpdateArmatureBones(elfArmature* armature);

elfArmature* elfCreateArmature(const char* name);

void elfDeformEntityWithArmature(elfArmature* armature, elfEntity* entity, float frame);

void elfDestroyArmature(void* data);

elfBone* elfGetBoneFromArmatureByName(const char* name, elfArmature* armature);
elfBone* elfGetBoneFromArmatureById(int id, elfArmature* armature);

void elfAddRootBoneToArmature(elfArmature* armature, elfBone* bone);

void elfDrawBoneHierarchy(elfBone* bone, gfxShaderParams* shaderParams);

void elfDrawArmatureDebug(elfArmature* armature, gfxShaderParams* shaderParams);