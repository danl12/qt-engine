#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLTexture>
#include <QVector4D>

class Material
{
public:
    explicit Material(QOpenGLTexture *texture = nullptr,
             QOpenGLTexture *normalMap = nullptr,
             const QVector4D &diffuseColor = {1.f, 1.f, 1.f, 1.f},
             const QVector4D &specularColor = {1.f, 1.f, 1.f, 1.f});

    float getReflectance() const;

    QVector4D getDiffuseColor() const;

    QVector4D getSpecularColor() const;

    QOpenGLTexture *getTexture() const;

    QOpenGLTexture *getNormalMap() const;

    bool hasTexture() {
        return texture != nullptr;
    }

private:
    QVector4D diffuseColor, specularColor;

    float reflectance{};

    QOpenGLTexture* texture;
    QOpenGLTexture* normalMap;

};

#endif // MATERIAL_H
