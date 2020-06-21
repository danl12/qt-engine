#ifndef ENTITY_H
#define ENTITY_H

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include "mesh.h"

class Entity {
public:
    Entity(QVector<Mesh *> meshes, const QVector3D &position = {0, 0, 0}, btRigidBody* rigidBody = nullptr);

    QVector<Mesh *> getMeshes() const;

    QVector3D getPosition() const;

    void setPosition(const QVector3D &position);

    QQuaternion getRotation() const;

    void setRotation(const QQuaternion &rotation);

    QMatrix4x4 getModelMatrix();

    float getScale() const;

    void setScale(const float &scale);

    btRigidBody *getRigidBody() const;

    void setRigidBody(btRigidBody *rigidBody);

    btVector3 getBtPosition();

    btQuaternion getBtRotation();

private:
    QVector<Mesh *> meshes;
    QVector3D position;
    QQuaternion rotation;
    btRigidBody* rigidBody = nullptr;
    float scale = 1;
};

#endif // ENTITY_H
