#ifndef ENTITY_H
#define ENTITY_H

#include "mesh.h"

class Entity
{
public:
    Entity(QVector<Mesh*> meshes, const QVector3D &position = {0, 0, 0});

    QVector<Mesh *> getMeshes() const;

    QVector3D getPosition() const;

private:
    QVector<Mesh*> meshes;
    QVector3D position;
};

#endif // ENTITY_H
