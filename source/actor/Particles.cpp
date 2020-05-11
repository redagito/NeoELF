#include "nelf/actor/Particles.h"

#include <cstdlib>
#include <cstring>

#include "nelf/Actor.h"
#include "nelf/Camera.h"
#include "nelf/Entity.h"
#include "nelf/General.h"
#include "nelf/List.h"
#include "nelf/Math.h"
#include "nelf/Model.h"
#include "nelf/Object.h"
#include "nelf/Particle.h"
#include "nelf/Random.h"
#include "nelf/RenderStation.h"
#include "nelf/Resources.h"
#include "nelf/String.h"
#include "nelf/objectType.h"
#include "nelf/particleDrawMode.h"

elfParticles* elfCreateParticles(const char* name, int maxCount)
{
    elfParticles* particles;
    float* texCoordBuffer;
    float* colorBuffer;
    int i, j, k;

    if (maxCount < 1)
        return NULL;

    particles = (elfParticles*)malloc(sizeof(elfParticles));
    memset(particles, 0x0, sizeof(elfParticles));
    particles->objType = ELF_PARTICLES;
    particles->objDestr = elfDestroyParticles;

    elfInitActor((elfActor*)particles, false);

    particles->maxCount = maxCount;
    particles->particles = elfCreateList();
    elfIncRef((elfObject*)particles->particles);

    particles->drawMode = ELF_ADD;
    particles->spawnCount = 50;
    particles->spawnDelay = 0.02f;
    particles->spawn = true;
    particles->sizeMin = 1.0f;
    particles->sizeMax = 1.0f;
    particles->sizeGrowthMin = 0.0f;
    particles->sizeGrowthMax = 0.0f;
    particles->rotationMin = 0.0f;
    particles->rotationMax = 0.0f;
    particles->rotationGrowthMin = 0.0f;
    particles->rotationGrowthMax = 0.0f;
    particles->lifeSpanMin = 3.0f;
    particles->lifeSpanMax = 3.0f;
    particles->fadeSpeedMin = 0.33f;
    particles->fadeSpeedMax = 0.33f;
    particles->velocityMin.x = -10.0f;
    particles->velocityMin.y = -10.0f;
    particles->velocityMin.z = -10.0f;
    particles->velocityMax.x = 10.0f;
    particles->velocityMax.y = 10.0f;
    particles->velocityMax.z = 10.0f;
    particles->colorMin.r = 1.0f;
    particles->colorMin.g = 1.0f;
    particles->colorMin.b = 1.0f;
    particles->colorMin.a = 1.0f;
    particles->colorMax.r = 1.0f;
    particles->colorMax.g = 1.0f;
    particles->colorMax.b = 1.0f;
    particles->colorMax.a = 1.0f;

    particles->vertices = gfxCreateVertexData(3 * 6 * maxCount, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    particles->texCoords = gfxCreateVertexData(2 * 6 * maxCount, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    particles->colors = gfxCreateVertexData(4 * 6 * maxCount, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    particles->vertexArray = gfxCreateVertexArray(GFX_FALSE);
    gfxSetVertexArrayData(particles->vertexArray, GFX_VERTEX, particles->vertices);
    gfxSetVertexArrayData(particles->vertexArray, GFX_TEX_COORD, particles->texCoords);
    gfxSetVertexArrayData(particles->vertexArray, GFX_COLOR, particles->colors);

    gfxIncRef((gfxObject*)particles->vertices);
    gfxIncRef((gfxObject*)particles->texCoords);
    gfxIncRef((gfxObject*)particles->colors);
    gfxIncRef((gfxObject*)particles->vertexArray);

    texCoordBuffer = (float*)gfxGetVertexDataBuffer(particles->texCoords);
    colorBuffer = (float*)gfxGetVertexDataBuffer(particles->colors);

    for (i = 0; i < particles->maxCount; i++)
    {
        j = i * 12;
        k = i * 24;

        texCoordBuffer[j] = 0.0f;
        texCoordBuffer[j + 1] = 1.0f;
        texCoordBuffer[j + 2] = 0.0f;
        texCoordBuffer[j + 3] = 0.0f;
        texCoordBuffer[j + 4] = 1.0f;
        texCoordBuffer[j + 5] = 0.0f;

        texCoordBuffer[j + 6] = 0.0f;
        texCoordBuffer[j + 7] = 1.0f;
        texCoordBuffer[j + 8] = 1.0f;
        texCoordBuffer[j + 9] = 0.0f;
        texCoordBuffer[j + 10] = 1.0f;
        texCoordBuffer[j + 11] = 1.0f;

        colorBuffer[k] = 1.0f;
        colorBuffer[k + 1] = 1.0f;
        colorBuffer[k + 2] = 1.0f;
        colorBuffer[k + 3] = 1.0f;
        colorBuffer[k + 4] = 1.0f;
        colorBuffer[k + 5] = 1.0f;
        colorBuffer[k + 6] = 1.0f;
        colorBuffer[k + 7] = 1.0f;
        colorBuffer[k + 8] = 1.0f;
        colorBuffer[k + 9] = 1.0f;
        colorBuffer[k + 10] = 1.0f;
        colorBuffer[k + 11] = 1.0f;
        colorBuffer[k + 12] = 1.0f;
        colorBuffer[k + 13] = 1.0f;
        colorBuffer[k + 14] = 1.0f;
        colorBuffer[k + 15] = 1.0f;
        colorBuffer[k + 16] = 1.0f;
        colorBuffer[k + 17] = 1.0f;
        colorBuffer[k + 18] = 1.0f;
        colorBuffer[k + 19] = 1.0f;
        colorBuffer[k + 20] = 1.0f;
        colorBuffer[k + 21] = 1.0f;
        colorBuffer[k + 22] = 1.0f;
        colorBuffer[k + 23] = 1.0f;
    }

    particles->dobject = elfCreatePhysicsObjectBox(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
    elfSetPhysicsObjectActor(particles->dobject, (elfActor*)particles);
    elfIncRef((elfObject*)particles->dobject);

    particles->pbbLengths.x = particles->pbbLengths.y = particles->pbbLengths.z = 1.0f;

    if (name)
        particles->name = elfCreateString(name);

    particles->id = ++res->particlesIdCounter;

    elfIncObj(ELF_PARTICLES);

    return particles;
}

void elfInitNewParticle(elfParticles* particles, elfParticle* particle)
{
    static int num;
    static float* vertices;
    static elfVec4f orient;
    static elfVec3f localPos;
    static elfVec3f result;

    particle->lifeSpan = elfRandomFloatRange(particles->lifeSpanMin, particles->lifeSpanMax);
    particle->fadeSpeed = elfRandomFloatRange(particles->fadeSpeedMin, particles->fadeSpeedMax);
    particle->size = elfRandomFloatRange(particles->sizeMin, particles->sizeMax);
    particle->sizeGrowth = elfRandomFloatRange(particles->sizeGrowthMin, particles->sizeGrowthMax);
    particle->rotation = elfRandomFloatRange(particles->rotationMin, particles->rotationMax);
    particle->rotationGrowth = elfRandomFloatRange(particles->rotationGrowthMin, particles->rotationGrowthMax);
    if (particles->model && elfGetModelVertexCount(particles->model) > 0)
    {
        elfGetActorPosition_((elfActor*)particles, &particle->position.x);
        num = elfRandomIntRange(0, elfGetModelVertexCount(particles->model));
        vertices = elfGetModelVertices(particles->model);
        particle->position.x += vertices[3 * num];
        particle->position.y += vertices[3 * num + 1];
        particle->position.z += vertices[3 * num + 2];
    }
    else if (particles->entity && particles->entity->model && elfGetModelVertexCount(particles->entity->model) > 0)
    {
        elfGetActorPosition_((elfActor*)particles->entity, &particle->position.x);
        num = elfRandomIntRange(0, elfGetModelVertexCount(particles->entity->model));
        if (!particles->entity->vertices)
            vertices = (float*)gfxGetVertexDataBuffer(particles->entity->model->vertices);
        else
            vertices = (float*)gfxGetVertexDataBuffer(particles->entity->vertices);
        localPos.x = vertices[3 * num];
        localPos.y = vertices[3 * num + 1];
        localPos.z = vertices[3 * num + 2];
        elfGetActorOrientation_((elfActor*)particles->entity, &orient.x);
        gfxMulQuaVec(&orient.x, &localPos.x, &result.x);
        particle->position.x += result.x;
        particle->position.y += result.y;
        particle->position.z += result.z;
    }
    else
    {
        elfGetActorPosition_((elfActor*)particles, &particle->position.x);
        particle->position.x += elfRandomFloatRange(particles->positionMin.x, particles->positionMax.x);
        particle->position.y += elfRandomFloatRange(particles->positionMin.y, particles->positionMax.y);
        particle->position.z += elfRandomFloatRange(particles->positionMin.z, particles->positionMax.z);
    }
    particle->velocity.x = elfRandomFloatRange(particles->velocityMin.x, particles->velocityMax.x);
    particle->velocity.y = elfRandomFloatRange(particles->velocityMin.y, particles->velocityMax.y);
    particle->velocity.z = elfRandomFloatRange(particles->velocityMin.z, particles->velocityMax.z);
    particle->color.r = elfRandomFloatRange(particles->colorMin.r, particles->colorMax.r);
    particle->color.g = elfRandomFloatRange(particles->colorMin.g, particles->colorMax.g);
    particle->color.b = elfRandomFloatRange(particles->colorMin.b, particles->colorMax.b);
    particle->color.a = elfRandomFloatRange(particles->colorMin.a, particles->colorMax.a);
}

void elfCalcParticlesAabb(elfParticles* particles)
{
    elfVec3f position;

    particles->cullAabbMin.x =
        particles->lifeSpanMax * (particles->velocityMin.x + elfFloatMin(0.0f, particles->gravity.x));
    particles->cullAabbMin.y =
        particles->lifeSpanMax * (particles->velocityMin.y + elfFloatMin(0.0f, particles->gravity.y));
    particles->cullAabbMin.z =
        particles->lifeSpanMax * (particles->velocityMin.z + elfFloatMin(0.0f, particles->gravity.z));

    particles->cullAabbMax.x =
        particles->lifeSpanMax * (particles->velocityMax.x + elfFloatMax(0.0f, particles->gravity.x));
    particles->cullAabbMax.y =
        particles->lifeSpanMax * (particles->velocityMax.y + elfFloatMax(0.0f, particles->gravity.y));
    particles->cullAabbMax.z =
        particles->lifeSpanMax * (particles->velocityMax.z + elfFloatMax(0.0f, particles->gravity.z));

    if (particles->model)
    {
        particles->cullAabbMin.x += particles->model->bbMin.x;
        particles->cullAabbMin.y += particles->model->bbMin.y;
        particles->cullAabbMin.z += particles->model->bbMin.z;

        particles->cullAabbMax.x += particles->model->bbMax.x;
        particles->cullAabbMax.y += particles->model->bbMax.y;
        particles->cullAabbMax.z += particles->model->bbMax.z;
    }
    else if (particles->entity)
    {
        particles->cullAabbMin.x += particles->entity->cullAabbMin.x;
        particles->cullAabbMin.y += particles->entity->cullAabbMin.y;
        particles->cullAabbMin.z += particles->entity->cullAabbMin.z;

        particles->cullAabbMax.x += particles->entity->cullAabbMax.x;
        particles->cullAabbMax.y += particles->entity->cullAabbMax.y;
        particles->cullAabbMax.z += particles->entity->cullAabbMax.z;
    }
    else
    {
        particles->cullAabbMin.x += particles->positionMin.x;
        particles->cullAabbMin.y += particles->positionMin.y;
        particles->cullAabbMin.z += particles->positionMin.z;

        particles->cullAabbMax.x += particles->positionMax.x;
        particles->cullAabbMax.y += particles->positionMax.y;
        particles->cullAabbMax.z += particles->positionMax.z;
    }

    if (!particles->entity)
    {
        elfGetActorPosition_((elfActor*)particles, &position.x);
    }
    else
    {
        elfGetActorPosition_((elfActor*)particles->entity, &position.x);
    }

    particles->cullAabbMin.x += position.x;
    particles->cullAabbMin.y += position.y;
    particles->cullAabbMin.z += position.z;

    particles->cullAabbMax.x += position.x;
    particles->cullAabbMax.y += position.y;
    particles->cullAabbMax.z += position.z;
}

void elfParticlesPreDraw(elfParticles* particles)
{
    elfActorPreDraw((elfActor*)particles);

    if (particles->moved)
        elfCalcParticlesAabb(particles);
}

void elfParticlesPostDraw(elfParticles* particles) { elfActorPostDraw((elfActor*)particles); }

void elfUpdateParticles(elfParticles* particles, float sync)
{
    elfParticle* particle;
    int spawnCount;
    int i;

    elfUpdateActor((elfActor*)particles);

    // update, remove and spawn particles
    particles->curTime += sync;
    if (particles->spawnCount == 0)
    {
        spawnCount = 0;
        particles->curTime = 0.0f;
    }
    else
    {
        spawnCount = (int)(particles->curTime / particles->spawnDelay);
        if (elfGetListLength(particles->particles) + spawnCount > particles->maxCount)
        {
            spawnCount -= (elfGetListLength(particles->particles) + spawnCount) - particles->maxCount;
            particles->curTime -= sync;
        }
        if (spawnCount > 0)
            particles->curTime -= particles->spawnDelay * spawnCount;
    }

    for (particle = (elfParticle*)elfBeginList(particles->particles); particle;
         particle = (elfParticle*)elfGetListNext(particles->particles))
    {
        if (particle->lifeSpan < 0.0f || particle->color.a < 0.0f)
        {
            if (spawnCount > 0 && particles->spawn)
            {
                elfInitNewParticle(particles, particle);
                spawnCount--;
            }
            else
            {
                elfRemoveListObject(particles->particles, (elfObject*)particle);
            }
            continue;
        }
        particle->size += particle->sizeGrowth * sync;
        particle->rotation += particle->rotationGrowth * sync;
        particle->position.x += particle->velocity.x * sync;
        particle->position.y += particle->velocity.y * sync;
        particle->position.z += particle->velocity.z * sync;
        particle->lifeSpan -= sync;
        particle->velocity.x += particles->gravity.x * sync;
        particle->velocity.y += particles->gravity.y * sync;
        particle->velocity.z += particles->gravity.z * sync;
        particle->color.a -= particle->fadeSpeed * sync;
    }

    // spawn particles
    if (particles->spawn)
    {
        for (i = 0; i < spawnCount; i++)
        {
            particle = elfCreateParticle();

            elfInitNewParticle(particles, particle);

            elfAppendListObject(particles->particles, (elfObject*)particle);
        }
    }
}

void elfDrawParticles(elfParticles* particles, elfCamera* camera, gfxShaderParams* shaderParams)
{
    elfParticle* particle;
    int i, j;
    float offset;
    float pos[3];
    float cameraPos[3];
    float cameraOrient[4];
    float invCameraPos[3];
    float invCameraOrient[4];
    float particleOffset[3];
    elfColor realColor;
    float sinX1;
    float cosY1;
    float sinX2;
    float cosY2;
    float sinX3;
    float cosY3;
    float sinX4;
    float cosY4;
    float radius;
    float* vertexBuffer;
    float* colorBuffer;

    vertexBuffer = (float*)gfxGetVertexDataBuffer(particles->vertices);
    colorBuffer = (float*)gfxGetVertexDataBuffer(particles->colors);

    elfGetActorPosition_((elfActor*)camera, cameraPos);
    elfGetActorOrientation_((elfActor*)camera, cameraOrient);

    invCameraPos[0] = -cameraPos[0];
    invCameraPos[1] = -cameraPos[1];
    invCameraPos[2] = -cameraPos[2];
    gfxQuaGetInverse(cameraOrient, invCameraOrient);

    // rotating the particles takes up a lot of processing power, so see if it is really
    // necessary before going ahead and doing it
    if (elfAboutZero(particles->rotationMin) && elfAboutZero(particles->rotationMax) &&
        elfAboutZero(particles->rotationGrowthMin) && elfAboutZero(particles->rotationGrowthMax))
    {
        for (i = 0, j = 0, particle = (elfParticle*)elfBeginList(particles->particles); particle;
             particle = (elfParticle*)elfGetListNext(particles->particles), i++)
        {
            particleOffset[0] = invCameraPos[0] + particle->position.x;
            particleOffset[1] = invCameraPos[1] + particle->position.y;
            particleOffset[2] = invCameraPos[2] + particle->position.z;

            gfxMulQuaVec(invCameraOrient, particleOffset, pos);

            j = i * 18;
            offset = particle->size * 0.5f;

            vertexBuffer[j] = pos[0] - offset;
            vertexBuffer[j + 1] = pos[1] + offset;
            vertexBuffer[j + 2] = pos[2];
            vertexBuffer[j + 3] = pos[0] - offset;
            vertexBuffer[j + 4] = pos[1] - offset;
            vertexBuffer[j + 5] = pos[2];
            vertexBuffer[j + 6] = pos[0] + offset;
            vertexBuffer[j + 7] = pos[1] - offset;
            vertexBuffer[j + 8] = pos[2];

            vertexBuffer[j + 9] = pos[0] - offset;
            vertexBuffer[j + 10] = pos[1] + offset;
            vertexBuffer[j + 11] = pos[2];
            vertexBuffer[j + 12] = pos[0] + offset;
            vertexBuffer[j + 13] = pos[1] - offset;
            vertexBuffer[j + 14] = pos[2];
            vertexBuffer[j + 15] = pos[0] + offset;
            vertexBuffer[j + 16] = pos[1] + offset;
            vertexBuffer[j + 17] = pos[2];

            j = i * 24;
            realColor = particle->color;
            if (particles->drawMode == ELF_ADD)
            {
                realColor.r *= realColor.a;
                realColor.g *= realColor.a;
                realColor.b *= realColor.a;
                realColor.a = 1.0f;
            }

            colorBuffer[j] = realColor.r;
            colorBuffer[j + 1] = realColor.g;
            colorBuffer[j + 2] = realColor.b;
            colorBuffer[j + 3] = realColor.a;
            colorBuffer[j + 4] = realColor.r;
            colorBuffer[j + 5] = realColor.g;
            colorBuffer[j + 6] = realColor.b;
            colorBuffer[j + 7] = realColor.a;
            colorBuffer[j + 8] = realColor.r;
            colorBuffer[j + 9] = realColor.g;
            colorBuffer[j + 10] = realColor.b;
            colorBuffer[j + 11] = realColor.a;
            colorBuffer[j + 12] = realColor.r;
            colorBuffer[j + 13] = realColor.g;
            colorBuffer[j + 14] = realColor.b;
            colorBuffer[j + 15] = realColor.a;
            colorBuffer[j + 16] = realColor.r;
            colorBuffer[j + 17] = realColor.g;
            colorBuffer[j + 18] = realColor.b;
            colorBuffer[j + 19] = particle->color.a;
            colorBuffer[j + 20] = realColor.r;
            colorBuffer[j + 21] = realColor.g;
            colorBuffer[j + 22] = realColor.b;
            colorBuffer[j + 23] = particle->color.a;
        }
    }
    else
    {
        for (i = 0, j = 0, particle = (elfParticle*)elfBeginList(particles->particles); particle;
             particle = (elfParticle*)elfGetListNext(particles->particles), i++)
        {
            particleOffset[0] = invCameraPos[0] + particle->position.x;
            particleOffset[1] = invCameraPos[1] + particle->position.y;
            particleOffset[2] = invCameraPos[2] + particle->position.z;

            gfxMulQuaVec(invCameraOrient, particleOffset, pos);

            j = i * 18;
            offset = particle->size * 0.5f;
            radius = offset / 0.707107f;
            sinX1 = sin(GFX_PI_DIV_180 * (45.0f + particle->rotation));
            cosY1 = cos(GFX_PI_DIV_180 * (45.0f + particle->rotation));
            sinX2 = sin(GFX_PI_DIV_180 * (135.0f + particle->rotation));
            cosY2 = cos(GFX_PI_DIV_180 * (135.0f + particle->rotation));
            sinX3 = sin(GFX_PI_DIV_180 * (225.0f + particle->rotation));
            cosY3 = cos(GFX_PI_DIV_180 * (225.0f + particle->rotation));
            sinX4 = sin(GFX_PI_DIV_180 * (315.0f + particle->rotation));
            cosY4 = cos(GFX_PI_DIV_180 * (315.0f + particle->rotation));

            vertexBuffer[j] = pos[0] + radius * sinX4;
            vertexBuffer[j + 1] = pos[1] + radius * cosY4;
            vertexBuffer[j + 2] = pos[2];
            vertexBuffer[j + 3] = pos[0] + radius * sinX3;
            vertexBuffer[j + 4] = pos[1] + radius * cosY3;
            vertexBuffer[j + 5] = pos[2];
            vertexBuffer[j + 6] = pos[0] + radius * sinX2;
            vertexBuffer[j + 7] = pos[1] + radius * cosY2;
            vertexBuffer[j + 8] = pos[2];

            vertexBuffer[j + 9] = pos[0] + radius * sinX4;
            vertexBuffer[j + 10] = pos[1] + radius * cosY4;
            vertexBuffer[j + 11] = pos[2];
            vertexBuffer[j + 12] = pos[0] + radius * sinX2;
            vertexBuffer[j + 13] = pos[1] + radius * cosY2;
            vertexBuffer[j + 14] = pos[2];
            vertexBuffer[j + 15] = pos[0] + radius * sinX1;
            vertexBuffer[j + 16] = pos[1] + radius * cosY1;
            vertexBuffer[j + 17] = pos[2];

            j = i * 24;
            realColor = particle->color;
            if (particles->drawMode == ELF_ADD)
            {
                realColor.r *= realColor.a;
                realColor.g *= realColor.a;
                realColor.b *= realColor.a;
                realColor.a = 1.0f;
            }

            colorBuffer[j] = realColor.r;
            colorBuffer[j + 1] = realColor.g;
            colorBuffer[j + 2] = realColor.b;
            colorBuffer[j + 3] = realColor.a;
            colorBuffer[j + 4] = realColor.r;
            colorBuffer[j + 5] = realColor.g;
            colorBuffer[j + 6] = realColor.b;
            colorBuffer[j + 7] = realColor.a;
            colorBuffer[j + 8] = realColor.r;
            colorBuffer[j + 9] = realColor.g;
            colorBuffer[j + 10] = realColor.b;
            colorBuffer[j + 11] = realColor.a;
            colorBuffer[j + 12] = realColor.r;
            colorBuffer[j + 13] = realColor.g;
            colorBuffer[j + 14] = realColor.b;
            colorBuffer[j + 15] = realColor.a;
            colorBuffer[j + 16] = realColor.r;
            colorBuffer[j + 17] = realColor.g;
            colorBuffer[j + 18] = realColor.b;
            colorBuffer[j + 19] = particle->color.a;
            colorBuffer[j + 20] = realColor.r;
            colorBuffer[j + 21] = realColor.g;
            colorBuffer[j + 22] = realColor.b;
            colorBuffer[j + 23] = particle->color.a;
        }
    }

    if (elfGetListLength(particles->particles) > 0)
    {
        shaderParams->renderParams.blendMode = particles->drawMode;
        shaderParams->renderParams.vertexColor = true;
        gfxMatrix4SetIdentity(shaderParams->modelviewMatrix);
        if (particles->texture)
            shaderParams->textureParams->texture = particles->texture->texture;
        else
            shaderParams->textureParams->texture = NULL;
        shaderParams->textureParams->type = GFX_COLOR_MAP;
        gfxSetShaderParams(shaderParams);

        gfxDrawVertexArray(particles->vertexArray, 6 * elfGetListLength(particles->particles), GFX_TRIANGLES);
    }
}

void elfDrawParticlesDebug(elfParticles* particles, gfxShaderParams* shaderParams)
{
    float min[3];
    float max[3];
    float* vertexBuffer;

    gfxMulMatrix4Matrix4(gfxGetTransformMatrix(particles->transform), shaderParams->cameraMatrix,
                         shaderParams->modelviewMatrix);

    min[0] = min[1] = min[2] = -0.5f;
    max[0] = max[1] = max[2] = 0.5f;

    vertexBuffer = (float*)gfxGetVertexDataBuffer(rnd->lines);

    vertexBuffer[0] = 0.0f;
    vertexBuffer[1] = 0.0f;
    vertexBuffer[2] = min[2];
    vertexBuffer[3] = 0.0f;
    vertexBuffer[4] = 0.0f;
    vertexBuffer[5] = max[2];

    vertexBuffer[6] = 0.0f;
    vertexBuffer[7] = min[1];
    vertexBuffer[8] = 0.0f;
    vertexBuffer[9] = 0.0f;
    vertexBuffer[10] = max[1];
    vertexBuffer[11] = 0.0f;

    vertexBuffer[12] = min[0];
    vertexBuffer[13] = 0.0f;
    vertexBuffer[14] = 0.0f;
    vertexBuffer[15] = max[0];
    vertexBuffer[16] = 0.0f;
    vertexBuffer[17] = 0.0f;

    vertexBuffer[18] = min[0];
    vertexBuffer[19] = min[1];
    vertexBuffer[20] = min[2];
    vertexBuffer[21] = max[0];
    vertexBuffer[22] = max[1];
    vertexBuffer[23] = max[2];

    vertexBuffer[24] = min[0];
    vertexBuffer[25] = max[1];
    vertexBuffer[26] = min[2];
    vertexBuffer[27] = max[0];
    vertexBuffer[28] = min[1];
    vertexBuffer[29] = max[2];

    vertexBuffer[30] = max[0];
    vertexBuffer[31] = min[1];
    vertexBuffer[32] = min[2];
    vertexBuffer[33] = min[0];
    vertexBuffer[34] = max[1];
    vertexBuffer[35] = max[2];

    vertexBuffer[36] = max[0];
    vertexBuffer[37] = max[1];
    vertexBuffer[38] = min[2];
    vertexBuffer[39] = min[0];
    vertexBuffer[40] = min[1];
    vertexBuffer[41] = max[2];

    if (!particles->selected)
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 0.35f, 0.75f, 0.75f, 1.0f);
    else
        gfxSetColor(&shaderParams->materialParams.diffuseColor, 1.0f, 0.0f, 0.0f, 1.0f);
    gfxSetShaderParams(shaderParams);
    elfDrawLines(14, rnd->lines);

    elfDrawActorDebug((elfActor*)particles, shaderParams);
}

void elfDestroyParticles(void* data)
{
    elfParticles* particles = (elfParticles*)data;

    elfCleanActor((elfActor*)particles);

    elfDecRef((elfObject*)particles->particles);
    if (particles->texture)
        elfDecRef((elfObject*)particles->texture);
    if (particles->model)
        elfDecRef((elfObject*)particles->model);
    if (particles->entity)
        elfDecRef((elfObject*)particles->entity);

    gfxDecRef((gfxObject*)particles->vertexArray);
    gfxDecRef((gfxObject*)particles->vertices);
    gfxDecRef((gfxObject*)particles->texCoords);
    gfxDecRef((gfxObject*)particles->colors);

    free(particles);

    elfDecObj(ELF_PARTICLES);
}

const char* elfGetParticlesName(elfParticles* particles) { return particles->name; }

const char* elfGetParticlesFilePath(elfParticles* particles) { return particles->filePath; }

void elfSetParticlesMaxCount(elfParticles* particles, int maxCount)
{
    int i, j, k;
    float* texCoordBuffer;
    float* colorBuffer;

    if (maxCount < 1)
        return;

    particles->maxCount = maxCount;

    elfDecRef((elfObject*)particles->particles);
    particles->particles = elfCreateList();
    elfIncRef((elfObject*)particles->particles);

    gfxDecRef((gfxObject*)particles->vertices);
    gfxDecRef((gfxObject*)particles->texCoords);
    gfxDecRef((gfxObject*)particles->colors);
    gfxDecRef((gfxObject*)particles->vertexArray);

    particles->vertices = gfxCreateVertexData(3 * 6 * maxCount, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    particles->texCoords = gfxCreateVertexData(2 * 6 * maxCount, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    particles->colors = gfxCreateVertexData(4 * 6 * maxCount, GFX_FLOAT, GFX_VERTEX_DATA_DYNAMIC);
    particles->vertexArray = gfxCreateVertexArray(GFX_FALSE);
    gfxSetVertexArrayData(particles->vertexArray, GFX_VERTEX, particles->vertices);
    gfxSetVertexArrayData(particles->vertexArray, GFX_TEX_COORD, particles->texCoords);
    gfxSetVertexArrayData(particles->vertexArray, GFX_COLOR, particles->colors);

    gfxIncRef((gfxObject*)particles->vertices);
    gfxIncRef((gfxObject*)particles->texCoords);
    gfxIncRef((gfxObject*)particles->colors);
    gfxIncRef((gfxObject*)particles->vertexArray);

    texCoordBuffer = (float*)gfxGetVertexDataBuffer(particles->texCoords);
    colorBuffer = (float*)gfxGetVertexDataBuffer(particles->colors);

    for (i = 0; i < particles->maxCount; i++)
    {
        j = i * 12;
        k = i * 24;

        texCoordBuffer[j] = 0.0f;
        texCoordBuffer[j + 1] = 1.0f;
        texCoordBuffer[j + 2] = 0.0f;
        texCoordBuffer[j + 3] = 0.0f;
        texCoordBuffer[j + 4] = 1.0f;
        texCoordBuffer[j + 5] = 0.0f;

        texCoordBuffer[j + 6] = 0.0f;
        texCoordBuffer[j + 7] = 1.0f;
        texCoordBuffer[j + 8] = 1.0f;
        texCoordBuffer[j + 9] = 0.0f;
        texCoordBuffer[j + 10] = 1.0f;
        texCoordBuffer[j + 11] = 1.0f;

        colorBuffer[k] = 1.0f;
        colorBuffer[k + 1] = 1.0f;
        colorBuffer[k + 2] = 1.0f;
        colorBuffer[k + 3] = 1.0f;
        colorBuffer[k + 4] = 1.0f;
        colorBuffer[k + 5] = 1.0f;
        colorBuffer[k + 6] = 1.0f;
        colorBuffer[k + 7] = 1.0f;
        colorBuffer[k + 8] = 1.0f;
        colorBuffer[k + 9] = 1.0f;
        colorBuffer[k + 10] = 1.0f;
        colorBuffer[k + 11] = 1.0f;
        colorBuffer[k + 12] = 1.0f;
        colorBuffer[k + 13] = 1.0f;
        colorBuffer[k + 14] = 1.0f;
        colorBuffer[k + 15] = 1.0f;
        colorBuffer[k + 16] = 1.0f;
        colorBuffer[k + 17] = 1.0f;
        colorBuffer[k + 18] = 1.0f;
        colorBuffer[k + 19] = 1.0f;
        colorBuffer[k + 20] = 1.0f;
        colorBuffer[k + 21] = 1.0f;
        colorBuffer[k + 22] = 1.0f;
        colorBuffer[k + 23] = 1.0f;
    }
}

void elfSetParticlesDrawMode(elfParticles* particles, int mode)
{
    if (particles->drawMode < 1 || particles->drawMode > 4)
        return;
    particles->drawMode = mode;
}

void elfSetParticlesTexture(elfParticles* particles, elfTexture* texture)
{
    if (particles->texture)
        elfDecRef((elfObject*)particles->texture);
    particles->texture = texture;
    if (particles->texture)
        elfIncRef((elfObject*)particles->texture);
}

void elfClearParticlesTexture(elfParticles* particles)
{
    if (particles->texture)
        elfDecRef((elfObject*)particles->texture);
    particles->texture = NULL;
}

void elfSetParticlesModel(elfParticles* particles, elfModel* model)
{
    if (particles->entity)
        elfDecRef((elfObject*)particles->entity);
    particles->entity = NULL;

    if (particles->model)
        elfDecRef((elfObject*)particles->model);
    particles->model = model;
    if (particles->model)
        elfIncRef((elfObject*)particles->model);

    elfCalcParticlesAabb(particles);
}

void elfClearParticlesModel(elfParticles* particles)
{
    if (particles->model)
        elfDecRef((elfObject*)particles->model);
    particles->model = NULL;

    elfCalcParticlesAabb(particles);
}

void elfSetParticlesEntity(elfParticles* particles, elfEntity* entity)
{
    if (particles->model)
        elfDecRef((elfObject*)particles->model);
    particles->model = NULL;

    if (particles->entity)
        elfDecRef((elfObject*)particles->entity);
    particles->entity = entity;
    if (particles->entity)
        elfIncRef((elfObject*)particles->entity);

    elfCalcParticlesAabb(particles);
}

void elfClearParticlesEntity(elfParticles* particles)
{
    if (particles->entity)
        elfDecRef((elfObject*)particles->entity);
    particles->entity = NULL;

    elfCalcParticlesAabb(particles);
}

void elfSetParticlesGravity(elfParticles* particles, float x, float y, float z)
{
    particles->gravity.x = x;
    particles->gravity.y = y;
    particles->gravity.z = z;

    elfCalcParticlesAabb(particles);
}

void elfSetParticlesSpawnCount(elfParticles* particles, int perSecond)
{
    particles->spawnCount = perSecond;
    if (particles->spawnCount <= 0)
    {
        particles->spawnCount = 0;
        particles->spawnDelay = 0.0f;
    }
    else
    {
        particles->spawnDelay = 1.0f / (float)particles->spawnCount;
    }
}

void elfSetParticlesSpawn(elfParticles* particles, unsigned char spawn) { particles->spawn = !spawn == ELF_FALSE; }

void elfSetParticlesSize(elfParticles* particles, float min, float max)
{
    particles->sizeMin = min;
    particles->sizeMax = max;
}

void elfSetParticlesSizeGrowth(elfParticles* particles, float min, float max)
{
    particles->sizeGrowthMin = min;
    particles->sizeGrowthMax = max;
}

void elfSetParticlesRotation(elfParticles* particles, float min, float max)
{
    particles->rotationMin = min;
    particles->rotationMax = max;
}

void elfSetParticlesRotationGrowth(elfParticles* particles, float min, float max)
{
    particles->rotationGrowthMin = min;
    particles->rotationGrowthMax = max;
}

void elfSetParticlesLifeSpan(elfParticles* particles, float min, float max)
{
    particles->lifeSpanMin = min;
    particles->lifeSpanMax = max;
}

void elfSetParticlesFadeSpeed(elfParticles* particles, float min, float max)
{
    particles->fadeSpeedMin = min;
    particles->fadeSpeedMax = max;
}

void elfSetParticlesVelocityMin(elfParticles* particles, float x, float y, float z)
{
    particles->velocityMin.x = x;
    particles->velocityMin.y = y;
    particles->velocityMin.z = z;

    elfCalcParticlesAabb(particles);
}

void elfSetParticlesPositionMin(elfParticles* particles, float x, float y, float z)
{
    particles->positionMin.x = x;
    particles->positionMin.y = y;
    particles->positionMin.z = z;

    elfCalcParticlesAabb(particles);
}

void elfSetParticlesPositionMax(elfParticles* particles, float x, float y, float z)
{
    particles->positionMax.x = x;
    particles->positionMax.y = y;
    particles->positionMax.z = z;

    elfCalcParticlesAabb(particles);
}

void elfSetParticlesVelocityMax(elfParticles* particles, float x, float y, float z)
{
    particles->velocityMax.x = x;
    particles->velocityMax.y = y;
    particles->velocityMax.z = z;

    elfCalcParticlesAabb(particles);
}

void elfSetParticlesColorMin(elfParticles* particles, float r, float g, float b, float a)
{
    particles->colorMin.r = r;
    particles->colorMin.g = g;
    particles->colorMin.b = b;
    particles->colorMin.a = a;
}

void elfSetParticlesColorMax(elfParticles* particles, float r, float g, float b, float a)
{
    particles->colorMax.r = r;
    particles->colorMax.g = g;
    particles->colorMax.b = b;
    particles->colorMax.a = a;
}

int elfGetParticlesMaxCount(elfParticles* particles) { return particles->maxCount; }

int elfGetParticlesCount(elfParticles* particles) { return elfGetListLength(particles->particles); }

int elfGetParticlesDrawMode(elfParticles* particles) { return particles->drawMode; }

elfTexture* elfGetParticlesTexture(elfParticles* particles) { return particles->texture; }

elfModel* elfGetParticlesModel(elfParticles* particles) { return particles->model; }

elfEntity* elfGetParticlesEntity(elfParticles* particles) { return particles->entity; }

elfVec3f elfGetParticlesGravity(elfParticles* particles) { return particles->gravity; }

int elfGetParticlesSpawnCount(elfParticles* particles) { return particles->spawnCount; }

unsigned char elfGetParticlesSpawn(elfParticles* particles) { return particles->spawn; }

float elfGetParticlesSizeMin(elfParticles* particles) { return particles->sizeMin; }

float elfGetParticlesSizeMax(elfParticles* particles) { return particles->sizeMax; }

float elfGetParticlesRotationMin(elfParticles* particles) { return particles->rotationMin; }

float elfGetParticlesRotationMax(elfParticles* particles) { return particles->rotationMax; }

float elfGetParticlesRotationGrowthMin(elfParticles* particles) { return particles->rotationGrowthMin; }

float elfGetParticlesRotationGrowthMax(elfParticles* particles) { return particles->rotationGrowthMax; }

float elfGetParticlesSizeGrowthMin(elfParticles* particles) { return particles->sizeGrowthMin; }

float elfGetParticlesSizeGrowthMax(elfParticles* particles) { return particles->sizeGrowthMax; }

float elfGetParticlesLifeSpanMin(elfParticles* particles) { return particles->lifeSpanMin; }

float elfGetParticlesLifeSpanMax(elfParticles* particles) { return particles->lifeSpanMax; }

float elfGetParticlesFadeSpeedMin(elfParticles* particles) { return particles->fadeSpeedMin; }

float elfGetParticlesFadeSpeedMax(elfParticles* particles) { return particles->fadeSpeedMax; }

elfVec3f elfGetParticlesPositionMin(elfParticles* particles) { return particles->positionMin; }

elfVec3f elfGetParticlesPositionMax(elfParticles* particles) { return particles->positionMax; }

elfVec3f elfGetParticlesVelocityMin(elfParticles* particles) { return particles->velocityMin; }

elfVec3f elfGetParticlesVelocityMax(elfParticles* particles) { return particles->velocityMax; }

elfColor elfGetParticlesColorMin(elfParticles* particles) { return particles->colorMin; }

elfColor elfGetParticlesColorMax(elfParticles* particles) { return particles->colorMax; }

unsigned char elfCullParticles(elfParticles* particles, elfCamera* camera)
{
    return !elfAabbInsideFrustum(camera, &particles->cullAabbMin.x, &particles->cullAabbMax.x);
}
