#include "entity.h"

#include <utility>

Entity::Entity(QVector<Mesh *> meshes, const QVector3D &position) :
        meshes(std::move(meshes)),
        position(position) {

}

QVector<Mesh *> Entity::getMeshes() const {
    return meshes;
}

QVector3D Entity::getPosition() const {
    return position;
}
