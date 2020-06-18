#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "camera.h"
#include "entity.h"
#include "mesh.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QElapsedTimer>

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    OpenGLWidget();

    QVector<Entity*> entities;

    void addEntity(Entity* entity);
    QOpenGLShaderProgram shaderProgram;

protected:
    // QWidget interface
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    // QOpenGLWidget interface
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    Camera camera;
    QMap<int, bool> keys;
    QElapsedTimer elapsedTimer, fpsTimer;
    QVector3D position;
    int fps = 0;

signals:
    void fpsUpdated(int fps);

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // OPENGLWIDGET_H
