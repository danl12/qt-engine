//
// Created by danl on 19.06.2020.
//

#include <iostream>
#include "Physics.h"

btRigidBody* Physics::createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
{
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        shape->calculateLocalInertia(mass, localInertia);

    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

    btRigidBody* body = new btRigidBody(cInfo);

    body->setUserIndex(-1);
    world->addRigidBody(body);
    return body;
}

Physics::Physics() {
    auto collisionConfiguration = new btDefaultCollisionConfiguration();
    auto dispatcher = new btCollisionDispatcher(collisionConfiguration);
    auto broadphase = new btDbvtBroadphase();
    auto solver = new btSequentialImpulseConstraintSolver;
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    world->setGravity(btVector3(0, -10, 0));
}

btDiscreteDynamicsWorld *Physics::getWorld() const {
    return world;
}
