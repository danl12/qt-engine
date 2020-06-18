#include "mesh.h"

#include <QOpenGLFunctions>

Mesh::Mesh(const QVector<QVector3D> &positions,
           const QVector<QVector2D> &uvs,
           const QVector<QVector3D> &normals,
           const QVector<unsigned int> &indices,
           const Material &material)
    : vao()
    , positionBuffer(QOpenGLBuffer::Type::VertexBuffer)
    , uvBuffer(QOpenGLBuffer::Type::VertexBuffer)
    , normalBuffer(QOpenGLBuffer::Type::VertexBuffer)
    , elementBuffer(QOpenGLBuffer::Type::IndexBuffer)
    , material(material)
{
    vertexCount = indices.size();
    vao.create();
    vao.bind();

    positionBuffer.create();
    positionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    positionBuffer.bind();
    positionBuffer.allocate(positions.constData(), positions.size() * sizeof(QVector3D));

    uvBuffer.create();
    uvBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    uvBuffer.bind();
    uvBuffer.allocate(uvs.constData(), uvs.size() * sizeof(QVector2D));

    normalBuffer.create();
    normalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    normalBuffer.bind();
    normalBuffer.allocate(normals.constData(), normals.size() * sizeof(QVector3D));

    elementBuffer.create();
    elementBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    elementBuffer.bind();
    elementBuffer.allocate(indices.constData(), indices.size() * sizeof(unsigned int));

    positionBuffer.release();
    uvBuffer.release();
    normalBuffer.release();
    elementBuffer.release();

    vao.release();
}

void Mesh::render(QOpenGLFunctions* functions, QOpenGLShaderProgram* shaderProgram)
{
    if (material.hasTexture()) {
        material.getTexture()->bind(GL_TEXTURE0);
    }
    shaderProgram->setUniformValue(shaderProgram->uniformLocation("myTextureSampler"), 0);

    shaderProgram->enableAttributeArray(shaderProgram->attributeLocation("aPos"));
    positionBuffer.bind();
    shaderProgram->setAttributeBuffer(shaderProgram->attributeLocation("aPos"), GL_FLOAT, 0, 3);

    shaderProgram->enableAttributeArray(shaderProgram->attributeLocation("aTexCoords"));
    uvBuffer.bind();
    shaderProgram->setAttributeBuffer(shaderProgram->attributeLocation("aTexCoords"), GL_FLOAT, 0, 2);

    shaderProgram->enableAttributeArray(shaderProgram->attributeLocation("aNormal"));
    normalBuffer.bind();
    shaderProgram->setAttributeBuffer(shaderProgram->attributeLocation("aNormal"), GL_FLOAT, 0, 3);

    elementBuffer.bind();

    functions->glDrawElements(
                GL_TRIANGLES,
                vertexCount,
                GL_UNSIGNED_INT,
                (void*)0
                );

    shaderProgram->disableAttributeArray(shaderProgram->uniformLocation("vertexPosition_modelspace"));
    shaderProgram->disableAttributeArray(shaderProgram->uniformLocation("vertexUV"));
    shaderProgram->disableAttributeArray(shaderProgram->uniformLocation("vertexNormal_modelspace"));
}


