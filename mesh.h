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

class Mesh
{
public:
    Mesh(const QVector<QVector3D> &positions,
         const QVector<QVector2D> &uvs,
         const QVector<QVector3D> &normals,
         const QVector<unsigned int> &indices,
         const Material &material = Material());

    void render(QOpenGLFunctions* f, QOpenGLShaderProgram *shaderProgram);

private:
    int vertexCount;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer positionBuffer, uvBuffer, normalBuffer, elementBuffer;
    Material material;
};

#endif // MESH_H
