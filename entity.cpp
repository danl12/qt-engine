#include <iostream>
#include "entity.h"



Entity::Entity(QVector<Mesh *> meshes, const QVector3D &position, btRigidBody* rigidBody) :
        meshes(std::move(meshes)),
        position(position),
        rigidBody(rigidBody) {

}

QVector<Mesh *> Entity::getMeshes() const {
    return meshes;
}

QVector3D Entity::getPosition() const {
    return position;
}

QMatrix4x4 Entity::getModelMatrix() {
    QMatrix4x4 modelMatrix = QMatrix4x4();
        btVector3 rbPosition = rigidBody->getWorldTransform().getOrigin();
        btQuaternion rbRotation = rigidBody->getWorldTransform().getRotation();
        QVector3D position(rbPosition.x(), rbPosition.y(), rbPosition.z());
        QQuaternion rotation(rbRotation.w(), rbRotation.x(), rbRotation.y(), rbRotation.z());
        modelMatrix.translate(position);
        modelMatrix.rotate(rotation);
    modelMatrix.scale(scale);
    return modelMatrix;
}

QQuaternion Entity::getRotation() const {
    return rotation;
}

void Entity::setRotation(const QQuaternion &rotation) {
    this->rotation = rotation;
}

void Entity::setPosition(const QVector3D &position) {
    this->position = position;
}

void Entity::setScale(const float &scale) {
    this->scale = scale;
}

float Entity::getScale() const {
    return scale;
}

btRigidBody *Entity::getRigidBody() const {
    return rigidBody;
}

void Entity::setRigidBody(btRigidBody *rigidBody) {
    Entity::rigidBody = rigidBody;
}

btVector3 Entity::getBtPosition() {
    return btVector3({position.x()}, {position.y()}, {position.z()});
}

btQuaternion Entity::getBtRotation() {
    return {rotation.x(), rotation.y(), rotation.z(), rotation.scalar()};
}
