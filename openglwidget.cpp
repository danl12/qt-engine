
#include "openglwidget.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>
#include <QtMath>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

OpenGLWidget::OpenGLWidget()
        : QOpenGLWidget(), camera({0.0f, 0.0f, 55.0f}, {0.0f, 1.0f, 0.0f}), position({0, 0, 5}) {
    setMouseTracking(true);
    grabKeyboard();

    QSurfaceFormat format;
    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);
}

void OpenGLWidget::addEntity(Entity *entity) {
    entities.push_back(entity);
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
//    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    shaderProgram.create();
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "../shaders/v_shader.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "../shaders/f_shader.frag");
    shaderProgram.link();
    shaderProgram.removeAllShaders();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000 / 60);
    emit fpsUpdated(0);
}

void OpenGLWidget::resizeGL(int w, int h) {

}

float angle = 0.f;

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (fpsTimer.isValid()) {
        if (fpsTimer.elapsed() >= 1000) {
            emit fpsUpdated(fps);
            fps = 0;
            fpsTimer.restart();
        } else {
            fps++;
        }
    } else {
        fpsTimer.start();
    }
    float elapsedTime;
    if (elapsedTimer.isValid()) {
        elapsedTime = elapsedTimer.nsecsElapsed() / 1000000000.f;
        elapsedTimer.restart();
    } else {
        elapsedTime = 1;
        elapsedTimer.start();
    }
    if (keys[Qt::Key_Alt]) {
//        QCursor c = cursor();

//        camera.processMouseMovement(mapFromGlobal(c.pos()).x() - width() / 2, height() / 2 - mapFromGlobal(c.pos()).y());

//        c.setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
//        setCursor(c);

        if (keys[Qt::Key_W]) {
            camera.processKeyboard(CameraMovement::FORWARD, elapsedTime);
        }
        if (keys[Qt::Key_S]) {
            camera.processKeyboard(CameraMovement::BACKWARD, elapsedTime);
        }
        if (keys[Qt::Key_D]) {
            camera.processKeyboard(CameraMovement::RIGHT, elapsedTime);
        }
        if (keys[Qt::Key_A]) {
            camera.processKeyboard(CameraMovement::LEFT, elapsedTime);
        }
    }

    shaderProgram.bind();

    shaderProgram.setUniformValue(shaderProgram.uniformLocation("light.position"), 0.f, 30.f, 0.f);
    shaderProgram.setUniformValue(shaderProgram.uniformLocation("light.direction"), 0.f, -1.f, 0.f);
//    shaderProgram.setUniformValue(shaderProgram.uniformLocation("light.position"), camera.position);
//    shaderProgram.setUniformValue(shaderProgram.uniformLocation("light.direction"), camera.front);
    shaderProgram.setUniformValue(shaderProgram.uniformLocation("light.cutOff"),
                                  (GLfloat) qCos(qDegreesToRadians(12.5f)));
    shaderProgram.setUniformValue(shaderProgram.uniformLocation("light.outerCutOff"),
                                  (GLfloat) qCos(qDegreesToRadians(247.5f)));
    shaderProgram.setUniformValue(shaderProgram.uniformLocation("viewPos"), camera.position);

    shaderProgram.setUniformValue(shaderProgram.uniformLocation("light.ambient"), 0.1f, 0.1f, 0.1f);
    shaderProgram.setUniformValue(shaderProgram.uniformLocation("light.diffuse"), 0.8f, 0.8f, 0.8f);
    shaderProgram.setUniformValue(shaderProgram.uniformLocation("light.specular"), 1.0f, 1.0f, 1.0f);
    shaderProgram.setUniformValue(shaderProgram.uniformLocation("light.constant"), 1.0f);
    shaderProgram.setUniformValue(shaderProgram.uniformLocation("light.linear"), 0.09f);
    shaderProgram.setUniformValue(shaderProgram.uniformLocation("light.quadratic"), 0.032f);

    shaderProgram.setUniformValue(shaderProgram.uniformLocation("material.shininess"), 32.0f);

    QMatrix4x4 projection;
    projection.perspective(45.f, (float) width() / height(), 0.1f, 1000.f);
    shaderProgram.setUniformValue(shaderProgram.uniformLocation("projection"), projection);
    shaderProgram.setUniformValue(shaderProgram.uniformLocation("view"), camera.getViewMatrix());


    for (auto & entity : entities) {
        QMatrix4x4 model;
        model.translate(entity->getPosition());
        model.scale({4.0f, 4.0f, 4.0f});
//    model.rotate(angle, {0.f, 1.f, 0.f});
//    angle += 10 * elapsedTime;
        shaderProgram.setUniformValue(shaderProgram.uniformLocation("model"), model);
        QVector<Mesh *> meshes = entity->getMeshes();
        for (auto & mesh : meshes) {
            mesh->render(this, &shaderProgram);
        }
    }

    shaderProgram.release();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (keys[Qt::Key_Alt]) {
        QCursor c = cursor();

        camera.processMouseMovement(mapFromGlobal(QCursor::pos()).x() - width() / 2,
                                    height() / 2 - mapFromGlobal(QCursor::pos()).y());

        c.setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
        setCursor(c);
    }
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Alt) {
        QCursor c = cursor();
        c.setShape(Qt::BlankCursor);
        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
        setCursor(c);
    }
    keys[event->key()] = true;
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Alt) {
        QCursor c = cursor();
        c.setShape(Qt::ArrowCursor);
        setCursor(c);
    }
    keys[event->key()] = false;
}
