#include "openglwidget.h"

#include <QApplication>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <bullet/LinearMath/btTransform.h>
#include <iostream>
#include "bullet/LinearMath/btVector3.h"

QVector<Mesh *> loadModel(const QString &modelPath, QString texturePath = "", btConvexHullShape* shape = nullptr) {
    static Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(modelPath.toStdString(),
                                             aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);

    QVector<Mesh *> meshes;
    if (scene != nullptr) {
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[i];
            QVector<QVector3D> vertices, normals;
            QVector<QVector2D> uvs;
            QVector<unsigned int> indices;
            for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                aiVector3D vertex = mesh->mVertices[j];
                vertices.push_back({vertex.x, vertex.y, vertex.z});

                if (mesh->mTextureCoords[0] != nullptr) {
                    aiVector3D uv = mesh->mTextureCoords[0][j];
                    uvs.push_back({uv.x, uv.y});
                }
                if (shape != nullptr) {
                    btVector3 point(vertex.x, vertex.y,vertex.z);
                    shape->addPoint(point);
                }
                if (mesh->mNormals != nullptr) {
                    aiVector3D normal = mesh->mNormals[j];
                    normals.push_back({normal.x, normal.y, normal.z});
                }
            }
            for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
                auto face = mesh->mFaces[j];
                indices.push_back(face.mIndices[0]);
                indices.push_back(face.mIndices[1]);
                indices.push_back(face.mIndices[2]);
            }

            if (texturePath.isEmpty() || scene->mNumMeshes > 1) {
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

                if (material != nullptr) {
                    aiString aiTexturePath;
                    material->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexturePath);
                    if (aiTexturePath.length > 0) {
                        texturePath = modelPath.left(modelPath.lastIndexOf("/") + 1) + aiTexturePath.C_Str();
                    }
                }
            }
            meshes.push_back(new Mesh(vertices,
                                      uvs,
                                      normals,
                                      indices,
                                      Material(texturePath)));
        }
    }
    return meshes;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    OpenGLWidget widget;
    widget.resize(1280, 720);
    widget.show();

    auto catShape = new btConvexHullShape();
    auto catMeshes = loadModel("C:/Users/danl/Downloads/cat/cat.obj", "C:/Users/danl/Downloads/cat/cat.jpg", catShape);
    if (!catMeshes.isEmpty()) {
        Entity *cat = new Entity(catMeshes);
        cat->setRotation(QQuaternion::fromEulerAngles(50, 0, 0));
        cat->setPosition({0, 20, 0});
        widget.addEntity(cat, catShape, .5f);
    }

    auto terrainShape = new btConvexHullShape();
    auto terrainMeshes = loadModel("C:/Users/danl/Downloads/terrain.obj", "", terrainShape);
    if (!terrainMeshes.isEmpty()) {
        Entity *terrain = new Entity(terrainMeshes);
        terrain->setScale(100);
        widget.addEntity(terrain, terrainShape);
    }

    auto shape = new btConvexHullShape();
    auto model = loadModel("C:/Users/danl/Downloads/cube.obj", "", shape);
    if (!model.isEmpty()) {
        for (int k = 0; k < 5; k++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    Entity *entity = new Entity(model, {1.f * i, 48 + 1.f * k, 1.f * j});
                    widget.addEntity(entity, shape, .1f);
                }
            }
        }
    }

    return a.exec();
}
