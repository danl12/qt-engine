#include "material.h"

Material::Material(QOpenGLTexture *texture, QOpenGLTexture *normalMap, const QVector4D &diffuseColor, const QVector4D &specularColor)
{
    this->diffuseColor = diffuseColor;
    this->specularColor = specularColor;
    this->texture = texture;
    this->normalMap = normalMap;
    if (texture != nullptr) {
        texture->create();
        texture->bind();
    }
    if (normalMap != nullptr) {
        normalMap->create();
        normalMap->bind();
    }
}

float Material::getReflectance() const
{
    return reflectance;
}

QVector4D Material::getDiffuseColor() const
{
    return diffuseColor;
}

QVector4D Material::getSpecularColor() const
{
    return specularColor;
}

QOpenGLTexture *Material::getTexture() const
{
    return texture;
}

QOpenGLTexture *Material::getNormalMap() const
{
    return normalMap;
}
