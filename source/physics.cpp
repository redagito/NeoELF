
#include <btBulletDynamicsCommon.h>
#include <stdio.h>
#include <stdlib.h>

#include "blendelf.h"
#include "gfx.h"
#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    struct elfCollision
    {
        ELF_OBJECT_HEADER;
        elfActor* actor;
        elfVec3f position;
        elfVec3f normal;
        float depth;
    };

    struct elfJoint
    {
        ELF_OBJECT_HEADER;
        char* name;
        unsigned char jointType;
        elfActor* actor1;
        elfActor* actor2;
        btTypedConstraint* constraint;
        elfPhysicsWorld* world;
        float pivot[3];
        float axis[3];
    };

    struct elfPhysicsTriMesh
    {
        ELF_OBJECT_HEADER;
        btTriangleMesh* triMesh;
    };

    struct elfPhysicsObject
    {
        ELF_OBJECT_HEADER;
        int shapeType;
        float mass;
        elfVec3f offset;
        btCollisionShape* shape;
        btRigidBody* body;
        btDefaultMotionState* motionState;
        elfPhysicsTriMesh* triMesh;
        elfPhysicsWorld* world;
        elfList* collisions;
        int collisionCount;
        elfActor* actor;
    };

    struct elfPhysicsWorld
    {
        ELF_OBJECT_HEADER;
        btCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        btBroadphaseInterface* broadphase;
        btConstraintSolver* solver;
        btDiscreteDynamicsWorld* world;
        float sync;
    };

    class MultipleRayResultCallback : public btCollisionWorld::RayResultCallback
    {
       public:
        MultipleRayResultCallback(const btVector3& rayFromWorld, const btVector3& rayToWorld, elfList* list)
            : m_rayFromWorld(rayFromWorld), m_rayToWorld(rayToWorld), m_list(list)
        {
        }

        btVector3 m_rayFromWorld;
        btVector3 m_rayToWorld;
        btVector3 m_hitNormalWorld;
        btVector3 m_hitPointWorld;
        elfList* m_list;

        virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
        {
            elfCollision* collision;

            m_collisionObject = rayResult.m_collisionObject;
            if (normalInWorldSpace)
            {
                m_hitNormalWorld = rayResult.m_hitNormalLocal;
            }
            else
            {
                m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis() * rayResult.m_hitNormalLocal;
            }
            m_hitPointWorld.setInterpolate3(m_rayFromWorld, m_rayToWorld, rayResult.m_hitFraction);

            collision = elfCreateCollision();

            collision->position.x = m_hitPointWorld.x();
            collision->position.y = m_hitPointWorld.y();
            collision->position.z = m_hitPointWorld.z();
            collision->normal.x = m_hitNormalWorld.x();
            collision->normal.y = m_hitNormalWorld.y();
            collision->normal.z = m_hitNormalWorld.z();
            collision->actor = ((elfPhysicsObject*)((btRigidBody*)m_collisionObject)->getUserPointer())->actor;
            elfIncRef((elfObject*)collision->actor);

            elfAppendListObject(m_list, (elfObject*)collision);

            return rayResult.m_hitFraction;
        }
    };

    elfPhysicsWorld* elfCreatePhysicsWorld()
    {
        elfPhysicsWorld* world;

        world = (elfPhysicsWorld*)malloc(sizeof(elfPhysicsWorld));
        memset(world, 0x0, sizeof(elfPhysicsWorld));
        world->objType = ELF_PHYSICS_WORLD;
        world->objDestr = elfDestroyPhysicsWorld;

        world->collisionConfiguration = new btDefaultCollisionConfiguration();
        world->dispatcher = new btCollisionDispatcher(world->collisionConfiguration);
        world->broadphase = new btDbvtBroadphase();
        world->solver = new btSequentialImpulseConstraintSolver();
        world->world = new btDiscreteDynamicsWorld(world->dispatcher, world->broadphase, world->solver,
                                                   world->collisionConfiguration);
        world->world->getDispatchInfo().m_enableSPU = true;
        world->world->setGravity(btVector3(0.0f, 0.0f, -9.81f));

        elfIncObj(ELF_PHYSICS_WORLD);

        return world;
    }

    void elfDestroyPhysicsWorld(void* data)
    {
        elfPhysicsWorld* world = (elfPhysicsWorld*)data;

        delete world->world;
        delete world->solver;
        delete world->broadphase;
        delete world->dispatcher;
        delete world->collisionConfiguration;

        free(world);

        elfDecObj(ELF_PHYSICS_WORLD);
    }

    void elfUpdatePhysicsWorld(elfPhysicsWorld* world, float time)
    {
        int manifoldCount;
        int contactCount;
        btPersistentManifold* manifold = 0;
        btManifoldPoint* point = 0;
        elfPhysicsObject* obj0;
        elfPhysicsObject* obj1;
        elfCollision* col0;
        elfCollision* col1;
        int i, j;

        world->world->stepSimulation(time, 4);

        manifoldCount = world->dispatcher->getNumManifolds();
        contactCount = 0;

        for (i = 0; i < manifoldCount; i++)
        {
            manifold = world->dispatcher->getManifoldByIndexInternal(i);
            contactCount = manifold->getNumContacts();
            if (!contactCount)
                continue;

            obj0 = (elfPhysicsObject*)((btRigidBody*)manifold->getBody0())->getUserPointer();
            obj1 = (elfPhysicsObject*)((btRigidBody*)manifold->getBody1())->getUserPointer();
            if (obj0 == obj1)
                continue;

            // add elfClearPhysicObjectCollisions somewhere...

            while (elfGetListLength(obj0->collisions) < obj0->collisionCount + contactCount)
            {
                col0 = elfCreateCollision();
                elfAppendListObject(obj0->collisions, (elfObject*)col0);
            }
            while (elfGetListLength(obj1->collisions) < obj1->collisionCount + contactCount)
            {
                col1 = elfCreateCollision();
                elfAppendListObject(obj1->collisions, (elfObject*)col1);
            }

            obj0->collisionCount += contactCount;
            obj1->collisionCount += contactCount;

            for (j = 0, col0 = (elfCollision*)elfBeginList(obj0->collisions),
                col1 = (elfCollision*)elfBeginList(obj1->collisions);
                 j < contactCount; col0 = (elfCollision*)elfGetListNext(obj0->collisions),
                col1 = (elfCollision*)elfGetListNext(obj1->collisions), j++)
            {
                point = &manifold->getContactPoint(j);

                if (col0->actor)
                    elfDecRef((elfObject*)col0->actor);
                col0->actor = obj1->actor;
                elfIncRef((elfObject*)col0->actor);
                col0->position.x = point->m_positionWorldOnB.getX();
                col0->position.y = point->m_positionWorldOnB.getY();
                col0->position.z = point->m_positionWorldOnB.getZ();
                col0->normal.x = point->m_normalWorldOnB.getX();
                col0->normal.y = point->m_normalWorldOnB.getY();
                col0->normal.z = point->m_normalWorldOnB.getZ();
                col0->depth = point->m_distance1;

                if (col1->actor)
                    elfDecRef((elfObject*)col1->actor);
                col1->actor = obj0->actor;
                elfIncRef((elfObject*)col1->actor);
                col1->position.x = point->m_positionWorldOnB.getX();
                col1->position.y = point->m_positionWorldOnB.getY();
                col1->position.z = point->m_positionWorldOnB.getZ();
                col1->normal.x = point->m_normalWorldOnB.getX();
                col1->normal.y = point->m_normalWorldOnB.getY();
                col1->normal.z = point->m_normalWorldOnB.getZ();
                col1->depth = point->m_distance1;
            }
        }
    }

    void elfSetPhysicsWorldGravity(elfPhysicsWorld* world, float x, float y, float z)
    {
        world->world->setGravity(btVector3(x, y, z));
    }

    elfVec3f elfGetPhysicsWorldGravity(elfPhysicsWorld* world)
    {
        btVector3 grav;
        elfVec3f result;

        grav = world->world->getGravity();

        result.x = grav.x();
        result.y = grav.y();
        result.z = grav.z();

        return result;
    }

    elfCollision* elfGetRayCastResult(elfPhysicsWorld* world, float x, float y, float z, float dx, float dy, float dz)
    {
        elfCollision* collision;

        btCollisionWorld::ClosestRayResultCallback rayResult(btVector3(x, y, z), btVector3(dx, dy, dz));

        world->world->getCollisionWorld()->rayTest(btVector3(x, y, z), btVector3(dx, dy, dz), rayResult);

        if (!rayResult.hasHit())
            return NULL;

        collision = elfCreateCollision();

        collision->position.x = rayResult.m_hitPointWorld.x();
        collision->position.y = rayResult.m_hitPointWorld.y();
        collision->position.z = rayResult.m_hitPointWorld.z();
        collision->normal.x = rayResult.m_hitNormalWorld.x();
        collision->normal.y = rayResult.m_hitNormalWorld.y();
        collision->normal.z = rayResult.m_hitNormalWorld.z();
        collision->actor = ((elfPhysicsObject*)((btRigidBody*)rayResult.m_collisionObject)->getUserPointer())->actor;
        elfIncRef((elfObject*)collision->actor);

        return collision;
    }

    elfList* elfGetRayCastResults(elfPhysicsWorld* world, float x, float y, float z, float dx, float dy, float dz)
    {
        elfList* list;

        list = elfCreateList();

        MultipleRayResultCallback rayResult(btVector3(x, y, z), btVector3(dx, dy, dz), list);

        world->world->getCollisionWorld()->rayTest(btVector3(x, y, z), btVector3(dx, dy, dz), rayResult);

        if (!rayResult.hasHit())
        {
            elfDestroyList(list);
            return NULL;
        }

        return list;
    }

    elfCollision* elfCreateCollision()
    {
        elfCollision* collision;

        collision = (elfCollision*)malloc(sizeof(elfCollision));
        memset(collision, 0x0, sizeof(elfCollision));
        collision->objType = ELF_COLLISION;
        collision->objDestr = elfDestroyCollision;

        elfIncObj(ELF_COLLISION);

        return collision;
    }

    void elfDestroyCollision(void* data)
    {
        elfCollision* collision = (elfCollision*)data;

        if (collision->actor)
            elfDecRef((elfObject*)collision->actor);

        free(collision);

        elfDecObj(ELF_COLLISION);
    }

    elfActor* elfGetCollisionActor(elfCollision* collision) { return collision->actor; }

    elfVec3f elfGetCollisionPosition(elfCollision* collision) { return collision->position; }

    elfVec3f elfGetCollisionNormal(elfCollision* collision) { return collision->normal; }

    float elfGetCollisionDepth(elfCollision* collision) { return collision->depth; }

    elfJoint* elfCreateJoint()
    {
        elfJoint* joint;

        joint = (elfJoint*)malloc(sizeof(elfJoint));
        memset(joint, 0x0, sizeof(elfJoint));
        joint->objType = ELF_JOINT;
        joint->objDestr = elfDestroyJoint;

        elfIncObj(ELF_JOINT);

        return joint;
    }

    void elfRecalcJoint(elfJoint* joint)
    {
        elfVec3f pos1;
        elfVec3f pos2;
        elfVec4f qua1;
        elfVec4f qua2;
        elfVec4f aqua1;
        elfVec4f aqua2;
        elfVec3f worldCoordPivot1;
        elfVec3f localCoordPivot2;
        elfVec3f localAxis1;
        elfVec3f localAxis2;
        float matrix1[16];
        float matrix2[16];
        float matrix3[16];

        pos1 = elfGetActorPosition(joint->actor1);
        pos2 = elfGetActorPosition(joint->actor2);
        qua1 = elfGetActorOrientation(joint->actor1);
        qua2 = elfGetActorOrientation(joint->actor2);

        worldCoordPivot1.x = joint->pivot[0];
        worldCoordPivot1.y = joint->pivot[1];
        worldCoordPivot1.z = joint->pivot[2];

        worldCoordPivot1 = elfAddVec3fVec3f(elfMulQuaVec3f(qua1, worldCoordPivot1), pos1);
        localCoordPivot2 = elfMulQuaVec3f(elfGetQuaInverted(qua2), elfSubVec3fVec3f(worldCoordPivot1, pos2));

        btVector3 pivotInA(joint->pivot[0], joint->pivot[1], joint->pivot[2]);
        btVector3 pivotInB(localCoordPivot2.x, localCoordPivot2.y, localCoordPivot2.z);

        aqua1 = elfCreateQuaFromEuler(joint->axis[0], joint->axis[1], joint->axis[2]);
        localAxis1 = elfMulQuaVec3f(aqua1, elfCreateVec3f(0.0f, 0.0f, 1.0f));

        aqua2 = elfMulQuaQua(aqua1, elfMulQuaQua(qua1, elfGetQuaInverted(qua2)));
        localAxis2 = elfMulQuaVec3f(aqua2, elfCreateVec3f(0.0f, 0.0f, 1.0f));

        btVector3 axisInA(localAxis1.x, localAxis1.y, localAxis1.z);
        btVector3 axisInB(localAxis2.x, localAxis2.y, localAxis2.z);

        if (joint->jointType == ELF_HINGE)
        {
            joint->constraint = new btHingeConstraint(*joint->actor1->object->body, *joint->actor2->object->body,
                                                      pivotInA, pivotInB, axisInA, axisInB);
            joint->actor1->scene->world->world->addConstraint(joint->constraint);
        }
        else if (joint->jointType == ELF_BALL)
        {
            joint->constraint = new btPoint2PointConstraint(*joint->actor1->object->body, *joint->actor2->object->body,
                                                            pivotInA, pivotInB);
            joint->actor1->scene->world->world->addConstraint(joint->constraint);
        }
        else if (joint->jointType == ELF_CONE_TWIST)
        {
            gfxMatrix4SetIdentity(matrix1);
            matrix1[12] = joint->pivot[0];
            matrix1[13] = joint->pivot[1];
            matrix1[14] = joint->pivot[2];
            gfxQuaToMatrix4(&aqua1.x, matrix2);
            gfxMulMatrix4Matrix4(matrix1, matrix2, matrix3);
            btTransform frameInA;
            frameInA.setFromOpenGLMatrix(matrix3);

            gfxMatrix4SetIdentity(matrix1);
            matrix1[12] = localCoordPivot2.x;
            matrix1[13] = localCoordPivot2.y;
            matrix1[14] = localCoordPivot2.z;
            gfxQuaToMatrix4(&aqua2.x, matrix2);
            gfxMulMatrix4Matrix4(matrix1, matrix2, matrix3);
            btTransform frameInB;
            frameInB.setFromOpenGLMatrix(matrix3);

            joint->constraint = new btConeTwistConstraint(*joint->actor1->object->body, *joint->actor2->object->body,
                                                          frameInA, frameInB);
            joint->actor1->scene->world->world->addConstraint(joint->constraint);
        }
    }

    void elfSetJointWorld(elfJoint* joint, elfPhysicsWorld* world)
    {
        if (joint->world && joint->constraint)
        {
            joint->world->world->removeConstraint(joint->constraint);
            delete joint->constraint;
            joint->constraint = NULL;
        }
        joint->world = world;
        if (joint->world)
        {
            if (joint->world != joint->actor1->scene->world || joint->world != joint->actor2->scene->world)
            {
                joint->world = NULL;
                return;
            }
            elfRecalcJoint(joint);
        }
    }

    elfJoint* elfCreateHingeJoint(elfActor* actor1, elfActor* actor2, const char* name, float px, float py, float pz,
                                  float ax, float ay, float az)
    {
        elfJoint* joint;

        joint = elfCreateJoint();
        joint->jointType = ELF_HINGE;
        if (name)
            joint->name = elfCreateString(name);

        joint->actor1 = actor1;
        joint->actor2 = actor2;

        joint->pivot[0] = px;
        joint->pivot[1] = py;
        joint->pivot[2] = pz;

        joint->axis[0] = ax;
        joint->axis[1] = ay;
        joint->axis[2] = az;

        if (actor1->scene)
            elfSetJointWorld(joint, actor1->scene->world);

        return joint;
    }

    elfJoint* elfCreateBallJoint(elfActor* actor1, elfActor* actor2, const char* name, float px, float py, float pz)
    {
        elfJoint* joint;

        joint = elfCreateJoint();
        joint->jointType = ELF_BALL;
        if (name)
            joint->name = elfCreateString(name);

        joint->actor1 = actor1;
        joint->actor2 = actor2;

        joint->pivot[0] = px;
        joint->pivot[1] = py;
        joint->pivot[2] = pz;

        if (actor1->scene)
            elfSetJointWorld(joint, actor1->scene->world);

        return joint;
    }

    elfJoint* elfCreateConeTwistJoint(elfActor* actor1, elfActor* actor2, const char* name, float px, float py,
                                      float pz, float ax, float ay, float az)
    {
        elfJoint* joint;

        joint = elfCreateJoint();
        joint->jointType = ELF_BALL;
        if (name)
            joint->name = elfCreateString(name);

        joint->actor1 = actor1;
        joint->actor2 = actor2;

        joint->pivot[0] = px;
        joint->pivot[1] = py;
        joint->pivot[2] = pz;

        if (actor1->scene)
            elfSetJointWorld(joint, actor1->scene->world);

        return joint;
    }

    void elfClearJoint(elfJoint* joint)
    {
        if (joint->constraint)
        {
            if (joint->world)
                joint->world->world->removeConstraint(joint->constraint);
            delete joint->constraint;
            joint->constraint = NULL;
        }
        joint->actor1 = NULL;
        joint->actor2 = NULL;
        joint->world = NULL;
    }

    void elfDestroyJoint(void* data)
    {
        elfJoint* joint = (elfJoint*)data;

        if (joint->name)
            elfDestroyString(joint->name);
        if (joint->constraint)
        {
            if (joint->world)
                joint->world->world->removeConstraint(joint->constraint);
            delete joint->constraint;
        }
        free(joint);

        elfDecObj(ELF_JOINT);
    }

    const char* elfGetJointName(elfJoint* joint) { return joint->name; }

    int elfGetJointType(elfJoint* joint) { return joint->jointType; }

    elfActor* elfGetJointActorA(elfJoint* joint) { return joint->actor1; }

    elfActor* elfGetJointActorB(elfJoint* joint) { return joint->actor2; }

    elfVec3f elfGetJointPivot(elfJoint* joint)
    {
        elfVec3f result;

        result.x = joint->pivot[0];
        result.y = joint->pivot[1];
        result.z = joint->pivot[2];

        return result;
    }

    elfVec3f elfGetJointAxis(elfJoint* joint)
    {
        elfVec3f result;

        result.x = joint->axis[0];
        result.y = joint->axis[1];
        result.z = joint->axis[2];

        return result;
    }

    elfPhysicsTriMesh* elfCreatePhysicsTriMesh(float* verts, unsigned int* idx, int indiceCount)
    {
        elfPhysicsTriMesh* triMesh;
        int i;

        if (indiceCount < 3)
            return NULL;

        triMesh = (elfPhysicsTriMesh*)malloc(sizeof(elfPhysicsTriMesh));
        memset(triMesh, 0x0, sizeof(elfPhysicsTriMesh));
        triMesh->objType = ELF_PHYSICS_TRI_MESH;
        triMesh->objDestr = elfDestroyPhysicsTriMesh;

        triMesh->triMesh = new btTriangleMesh();

        for (i = 0; i < indiceCount; i += 3)
        {
            triMesh->triMesh->addTriangle(
                btVector3(verts[idx[i] * 3], verts[idx[i] * 3 + 1], verts[idx[i] * 3 + 2]),
                btVector3(verts[idx[i + 1] * 3], verts[idx[i + 1] * 3 + 1], verts[idx[i + 1] * 3 + 2]),
                btVector3(verts[idx[i + 2] * 3], verts[idx[i + 2] * 3 + 1], verts[idx[i + 2] * 3 + 2]));
        }

        elfIncObj(ELF_PHYSICS_TRI_MESH);

        return triMesh;
    }

    void elfDestroyPhysicsTriMesh(void* data)
    {
        elfPhysicsTriMesh* triMesh = (elfPhysicsTriMesh*)data;

        delete triMesh->triMesh;

        free(triMesh);

        elfDecObj(ELF_PHYSICS_TRI_MESH);
    }

    elfPhysicsObject* elfCreatePhysicsObject()
    {
        elfPhysicsObject* object;

        object = (elfPhysicsObject*)malloc(sizeof(elfPhysicsObject));
        memset(object, 0x0, sizeof(elfPhysicsObject));
        object->objType = ELF_PHYSICS_OBJECT;
        object->objDestr = elfDestroyPhysicsObject;

        object->collisions = elfCreateList();
        elfIncRef((elfObject*)object->collisions);

        elfIncObj(ELF_PHYSICS_OBJECT);

        return object;
    }

    elfPhysicsObject* elfCreatePhysicsObjectMesh(elfPhysicsTriMesh* triMesh, float mass)
    {
        elfPhysicsObject* object;

        object = elfCreatePhysicsObject();

        object->shape = new btBvhTriangleMeshShape(triMesh->triMesh, true);

        object->shapeType = ELF_MESH;
        object->mass = mass;

        object->triMesh = triMesh;
        elfIncRef((elfObject*)triMesh);

        btScalar bodyMass(mass);
        btVector3 localInertia(0.0f, 0.0f, 0.0f);

        if (!elfAboutZero(mass))
            object->shape->calculateLocalInertia(mass, localInertia);

        btTransform startTransform;
        startTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
        startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f));

        object->motionState = new btDefaultMotionState(startTransform);
        object->body = new btRigidBody(bodyMass, object->motionState, object->shape, localInertia);

        object->body->setUserPointer(object);

        return object;
    }

    elfPhysicsObject* elfCreatePhysicsObjectSphere(float radius, float mass, float ox, float oy, float oz)
    {
        elfPhysicsObject* object;

        object = elfCreatePhysicsObject();

        object->shape = new btSphereShape(btScalar(radius));
        object->shapeType = ELF_SPHERE;
        object->mass = mass;
        object->offset.x = ox;
        object->offset.y = oy;
        object->offset.z = oz;

        btScalar bodyMass(mass);
        btVector3 localInertia(0.0f, 0.0f, 0.0f);

        if (!elfAboutZero(mass))
            object->shape->calculateLocalInertia(mass, localInertia);

        btTransform startTransform;
        startTransform.setOrigin(btVector3(object->offset.x, object->offset.y, object->offset.z));
        startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f));

        object->motionState = new btDefaultMotionState(startTransform);
        object->body = new btRigidBody(bodyMass, object->motionState, object->shape, localInertia);

        object->body->setUserPointer(object);

        return object;
    }

    elfPhysicsObject* elfCreatePhysicsObjectBox(float hx, float hy, float hz, float mass, float ox, float oy, float oz)
    {
        elfPhysicsObject* object;

        object = elfCreatePhysicsObject();

        object->shape = new btBoxShape(btVector3(hx + 0.001f, hy + 0.001f, hz + 0.001f));
        object->shapeType = ELF_BOX;
        object->mass = mass;
        object->offset.x = ox;
        object->offset.y = oy;
        object->offset.z = oz;

        btScalar bodyMass(mass);
        btVector3 localInertia(0.0f, 0.0f, 0.0f);

        if (!elfAboutZero(mass))
            object->shape->calculateLocalInertia(mass, localInertia);

        btTransform startTransform;
        startTransform.setOrigin(btVector3(object->offset.x, object->offset.y, object->offset.z));
        startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f));

        object->motionState = new btDefaultMotionState(startTransform);
        object->body = new btRigidBody(bodyMass, object->motionState, object->shape, localInertia);

        object->body->setUserPointer(object);

        return object;
    }

    elfPhysicsObject* elfCreatePhysicsObjectCapsule(unsigned char type, float length, float radius, float mass,
                                                    float ox, float oy, float oz)
    {
        elfPhysicsObject* object;

        object = elfCreatePhysicsObject();

        if (type == ELF_CAPSULE_X)
            object->shape = new btCapsuleShapeX(radius, length);
        else if (type == ELF_CAPSULE_Y)
            object->shape = new btCapsuleShape(radius, length);
        else if (type == ELF_CAPSULE_Z)
            object->shape = new btCapsuleShapeZ(radius, length);

        object->shapeType = type;
        object->mass = mass;
        object->offset.x = ox;
        object->offset.y = oy;
        object->offset.z = oz;

        btScalar bodyMass(mass);
        btVector3 localInertia(0.0f, 0.0f, 0.0f);

        if (!elfAboutZero(mass))
            object->shape->calculateLocalInertia(mass, localInertia);

        btTransform startTransform;
        startTransform.setOrigin(btVector3(object->offset.x, object->offset.y, object->offset.z));
        startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f));

        object->motionState = new btDefaultMotionState(startTransform);
        object->body = new btRigidBody(bodyMass, object->motionState, object->shape, localInertia);

        object->body->setUserPointer(object);

        return object;
    }

    elfPhysicsObject* elfCreatePhysicsObjectCone(unsigned char type, float length, float radius, float mass, float ox,
                                                 float oy, float oz)
    {
        elfPhysicsObject* object;

        object = elfCreatePhysicsObject();

        if (type == ELF_CONE_X)
            object->shape = new btConeShapeX(radius, length);
        else if (type == ELF_CONE_Y)
            object->shape = new btConeShape(radius, length);
        else if (type == ELF_CONE_Z)
            object->shape = new btConeShapeZ(radius, length);

        object->shapeType = type;
        object->mass = mass;
        object->offset.x = ox;
        object->offset.y = oy;
        object->offset.z = oz;

        btScalar bodyMass(mass);
        btVector3 localInertia(0.0f, 0.0f, 0.0f);

        if (!elfAboutZero(mass))
            object->shape->calculateLocalInertia(mass, localInertia);

        btTransform startTransform;
        startTransform.setOrigin(btVector3(object->offset.x, object->offset.y, object->offset.z));
        startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f));

        object->motionState = new btDefaultMotionState(startTransform);
        object->body = new btRigidBody(bodyMass, object->motionState, object->shape, localInertia);

        object->body->setUserPointer(object);

        return object;
    }

    void elfSetPhysicsObjectWorld(elfPhysicsObject* object, elfPhysicsWorld* world)
    {
        if (object->world)
        {
            object->world->world->removeRigidBody(object->body);
            object->world = NULL;
        }
        object->world = world;
        if (object->world)
        {
            object->world->world->addRigidBody(object->body);
            object->body->activate(true);
        }
    }

    void elfDestroyPhysicsObject(void* data)
    {
        elfPhysicsObject* object = (elfPhysicsObject*)data;

        if (object->body)
        {
            if (object->world)
                object->world->world->removeRigidBody(object->body);
            delete object->body;
        }
        if (object->shape)
            delete object->shape;
        if (object->motionState)
            delete object->motionState;
        if (object->triMesh)
            elfDecRef((elfObject*)object->triMesh);
        elfDecRef((elfObject*)object->collisions);

        free(object);

        elfDecObj(ELF_PHYSICS_OBJECT);
    }

    void elfSetPhysicsObjectActor(elfPhysicsObject* object, elfActor* actor) { object->actor = actor; }

    void elfRemovePhysicsObjectCollisions(elfPhysicsObject* object)
    {
        if (elfGetListLength(object->collisions) > 0)
        {
            elfDecRef((elfObject*)object->collisions);
            object->collisions = elfCreateList();
            elfIncRef((elfObject*)object->collisions);
        }
        object->collisionCount = 0;
    }

    void elfClearPhysicsObjectCollisions(elfPhysicsObject* object)
    {
        if (elfGetListLength(object->collisions) > 0)
            elfRemoveListObject(object->collisions, elfBeginList(object->collisions));
        object->collisionCount = 0;
    }

    int elfGetPhysicsObjectCollisionCount(elfPhysicsObject* object) { return elfGetListLength(object->collisions); }

    elfCollision* elfGetPhysicsObjectCollision(elfPhysicsObject* object, int idx)
    {
        if (idx < 0 || idx > elfGetListLength(object->collisions) - 1)
            return NULL;

        return (elfCollision*)elfGetListObject(object->collisions, idx);
    }

    void elfSetPhysicsObjectPosition(elfPhysicsObject* object, float x, float y, float z)
    {
        float orient[4];
        float offset[3];

        elfGetPhysicsObjectOrientation(object, orient);

        gfxMulQuaVec(orient, &object->offset.x, offset);

        object->body->activate(true);
        if (object->body->isStaticObject())
        {
            object->body->setCollisionFlags(object->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        }
        btTransform xform = object->body->getCenterOfMassTransform();
        xform.setOrigin(btVector3(x + offset[0], y + offset[1], z + offset[2]));
        object->body->setCenterOfMassTransform(xform);
        object->motionState->setWorldTransform(xform);
    }

    void elfSetPhysicsObjectOrientation(elfPhysicsObject* object, float x, float y, float z, float w)
    {
        float orient[4];
        float offset[3];
        float newPos[3];
        btVector3 origin;

        elfGetPhysicsObjectOrientation(object, orient);

        gfxMulQuaVec(orient, &object->offset.x, offset);

        origin = object->motionState->m_graphicsWorldTrans.getOrigin();

        newPos[0] = origin.x() - offset[0];
        newPos[1] = origin.y() - offset[1];
        newPos[2] = origin.z() - offset[2];

        orient[0] = x;
        orient[1] = y;
        orient[2] = z;
        orient[3] = w;

        gfxMulQuaVec(orient, &object->offset.x, offset);

        newPos[0] += offset[0];
        newPos[1] += offset[1];
        newPos[2] += offset[2];

        object->body->activate(true);
        if (object->body->isStaticObject())
        {
            object->body->setCollisionFlags(object->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        }
        btTransform xform = object->body->getCenterOfMassTransform();
        xform.setOrigin(btVector3(newPos[0], newPos[1], newPos[2]));
        xform.setRotation(btQuaternion(x, y, z, w));
        object->body->setCenterOfMassTransform(xform);
        object->motionState->setWorldTransform(xform);
    }

    void elfSetPhysicsObjectScale(elfPhysicsObject* object, float x, float y, float z)
    {
        object->body->activate(true);
        object->shape->setLocalScaling(btVector3(x, y, z));
    }

    int elfGetPhysicsObjectShape(elfPhysicsObject* object) { return object->shapeType; }

    float elfGetPhysicsObjectMass(elfPhysicsObject* object) { return object->mass; }

    void elfGetPhysicsObjectPosition(elfPhysicsObject* object, float* params)
    {
        float orient[4];
        float offset[3];
        btVector3 origin;

        elfGetPhysicsObjectOrientation(object, orient);

        gfxMulQuaVec(orient, &object->offset.x, offset);

        origin = object->motionState->m_graphicsWorldTrans.getOrigin();
        params[0] = origin.x() - offset[0];
        params[1] = origin.y() - offset[1];
        params[2] = origin.z() - offset[2];
    }

    void elfGetPhysicsObjectOrientation(elfPhysicsObject* object, float* params)
    {
        btQuaternion orient = object->motionState->m_graphicsWorldTrans.getRotation();
        params[0] = orient.x();
        params[1] = orient.y();
        params[2] = orient.z();
        params[3] = orient.w();
    }

    void elfGetPhysicsObjectScale(elfPhysicsObject* object, float* params)
    {
        btVector3 origin = object->shape->getLocalScaling();
        params[0] = origin.x();
        params[1] = origin.y();
        params[2] = origin.z();
    }

    unsigned char elfIsPhysicsObjectStatic(elfPhysicsObject* object) { return object->body->isStaticObject(); }

    void elfSetPhysicsObjectAnisotropicFriction(elfPhysicsObject* object, float x, float y, float z)
    {
        object->body->setAnisotropicFriction(btVector3(x, y, z));
    }

    void elfSetPhysicsObjectDamping(elfPhysicsObject* object, float linDamp, float angDamp)
    {
        object->body->setDamping(linDamp, angDamp);
    }

    void elfSetPhysicsObjectSleepThresholds(elfPhysicsObject* object, float linThrs, float angThrs)
    {
        object->body->setSleepingThresholds(linThrs, angThrs);
    }

    void elfSetPhysicsObjectRestitution(elfPhysicsObject* object, float restitution)
    {
        object->body->setRestitution(restitution);
    }

    void elfAddPhysicsObjectForce(elfPhysicsObject* object, float x, float y, float z)
    {
        object->body->activate(true);
        object->body->applyCentralForce(btVector3(x, y, z));
    }

    void elfAddPhysicsObjectTorque(elfPhysicsObject* object, float x, float y, float z)
    {
        object->body->activate(true);
        object->body->applyTorque(btVector3(x, y, z));
    }

    void elfSetPhysicsObjectLinearVelocity(elfPhysicsObject* object, float x, float y, float z)
    {
        object->body->activate(true);
        object->body->setLinearVelocity(btVector3(x, y, z));
    }

    void elfSetPhysicsObjectAngularVelocity(elfPhysicsObject* object, float x, float y, float z)
    {
        object->body->activate(true);
        object->body->setAngularVelocity(btVector3(x, y, z));
    }

    void elfSetPhysicsObjectLinearFactor(elfPhysicsObject* object, float x, float y, float z)
    {
        object->body->activate(true);
        object->body->setLinearFactor(btVector3(x, y, z));
    }

    void elfSetPhysicsObjectAngularFactor(elfPhysicsObject* object, float x, float y, float z)
    {
        object->body->activate(true);
        object->body->setAngularFactor(btVector3(x, y, z));
    }

    void elfGetPhysicsObjectLinearVelocity(elfPhysicsObject* object, float* params)
    {
        btVector3 vec;
        vec = object->body->getLinearVelocity();
        params[0] = vec.x();
        params[1] = vec.y();
        params[2] = vec.z();
    }

    void elfGetPhysicsObjectAngularVelocity(elfPhysicsObject* object, float* params)
    {
        btVector3 vec;
        vec = object->body->getAngularVelocity();
        params[0] = vec.x();
        params[1] = vec.y();
        params[2] = vec.z();
    }

    void elfGetPhysicsObjectLinearFactor(elfPhysicsObject* object, float* params)
    {
        btVector3 vec;
        vec = object->body->getLinearFactor();
        params[0] = vec.x();
        params[1] = vec.y();
        params[2] = vec.z();
    }

    void elfGetPhysicsObjectAngularFactor(elfPhysicsObject* object, float* params)
    {
        btVector3 vec;
        vec = object->body->getAngularFactor();
        params[0] = vec.x();
        params[1] = vec.y();
        params[2] = vec.z();
    }

    void elfGetPhysicsObjectAnisotropicFriction(elfPhysicsObject* object, float* params)
    {
        btVector3 vec;
        vec = object->body->getAnisotropicFriction();
        params[0] = vec.x();
        params[1] = vec.y();
        params[2] = vec.z();
    }

    float elfGetPhysicsObjectLinearDamping(elfPhysicsObject* object) { return object->body->getLinearDamping(); }

    float elfGetPhysicsObjectAngularDamping(elfPhysicsObject* object) { return object->body->getAngularDamping(); }

    float elfGetPhysicsObjectLinearSleepThreshold(elfPhysicsObject* object)
    {
        return object->body->getLinearSleepingThreshold();
    }

    float elfGetPhysicsObjectAngularSleepThreshold(elfPhysicsObject* object)
    {
        return object->body->getAngularSleepingThreshold();
    }

    float elfGetPhysicsObjectRestitution(elfPhysicsObject* object) { return object->body->getRestitution(); }

#ifdef __cplusplus
}
#endif
