#include "openglwidget.h"

#include <QApplication>
#include <includes/assimp/scene.h>
#include <includes/assimp/Importer.hpp>
#include <includes/assimp/postprocess.h>

QVector<Mesh *> loadModel(const QString &modelPath, const QString &modelTexturePath = nullptr) {
    static Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(modelPath.toStdString(),
                                             aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);
    QVector<Mesh *> meshes;
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
        aiString texturePath;
        scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);

        QString modelFolder = modelPath.left(modelPath.lastIndexOf("/"));
        meshes.push_back(new Mesh(vertices,
                                  uvs,
                                  normals,
                                  indices,
                                  Material(
                                          new QOpenGLTexture(
                                                  QImage(
                                                          modelTexturePath == nullptr ? modelFolder + "/" +
                                                                                                   QString(texturePath.C_Str())
                                                                                                 : modelTexturePath)))));
    }
    return meshes;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    OpenGLWidget widget;
    widget.resize(1280, 720);
    widget.show();

    widget.addEntity(new Entity(
            loadModel("C:/Users/danl/Downloads/carriage/carriage.obj"),
            {-1, 0, 50}));

//    OpenGLWidget widget;

//    JsonObject obj = parseJson("conf.json");

//    for (Entity entity : obj.get("entities").toVector())  {
//        widget.add(entity);
//    }

//    widget.show();

    return a.exec();
}
