#ifndef MESH_H
#define MESH_H

#include "material.h"

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <bullet/btBulletCollisionCommon.h>

class Mesh
{
public:
    Mesh(const QVector<QVector3D> &positions,
         const QVector<QVector2D> &uvs = QVector<QVector2D>(),
         const QVector<QVector3D> &normals = QVector<QVector3D>(),
         const QVector<unsigned int> &indices = QVector<unsigned int>(),
         const Material &material = Material());

    void render(QOpenGLFunctions* f, QOpenGLShaderProgram *shaderProgram);

    const QVector<QVector3D> &getPositions() const;

    const QVector<unsigned int> & getIndices() const;

    int getVertexCount() const;

    const Material &getMaterial() const;

    void setMaterial(const Material &material);

private:
    int vertexCount;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer positionBuffer, uvBuffer, normalBuffer, elementBuffer;
    Material material;
    QVector<QVector3D> positions;
    QVector<unsigned int> indices;
};

#endif // MESH_H
