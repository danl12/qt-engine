//
// Created by danl on 19.06.2020.
//

#ifndef QT_ENGINE_PHYSICS_H
#define QT_ENGINE_PHYSICS_H

#include <bullet/btBulletDynamicsCommon.h>
#include <QVector>

class Physics {
public:
    Physics();

    btDiscreteDynamicsWorld *getWorld() const;

    btRigidBody *createRigidBody(float mass, const btTransform &startTransform, btCollisionShape *shape);

private:
    btDiscreteDynamicsWorld* world;
};


#endif //QT_ENGINE_PHYSICS_H
