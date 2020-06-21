#include "material.h"

#include <iostream>

Material::Material(const QString &texturePath,
                   const QString &normalMapPath, const QVector4D &diffuseColor, const QVector4D &specularColor)
{
    this->diffuseColor = diffuseColor;
    this->specularColor = specularColor;
    if (!texturePath.isEmpty()) {
//        std::cout << texturePath->toStdString() << std::endl;
        texture = new QOpenGLTexture(QImage(texturePath));
        texture->create();
        texture->bind();
    } else {
        texture = nullptr;
    }
    if (!normalMapPath.isEmpty()) {
        normalMap = new QOpenGLTexture(QImage(normalMapPath));
        normalMap->create();
        normalMap->bind();
    } else {
        normalMap = nullptr;
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
