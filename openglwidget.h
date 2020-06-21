#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "camera.h"
#include "entity.h"
#include "mesh.h"
#include "Physics.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QElapsedTimer>
#include <QOpenGLFunctions_2_0>

class BulletDebugDrawer_DeprecatedOpenGL : public btIDebugDraw{
public:
    QOpenGLFunctions_2_0* f;

    BulletDebugDrawer_DeprecatedOpenGL(QOpenGLFunctions_2_0* f) {
        this->f = f;
    }

    void SetMatrices(QMatrix4x4 pViewMatrix, QMatrix4x4 pProjectionMatrix){
        f->glUseProgram(0); // Use Fixed-function pipeline (no shaders)
        f->glMatrixMode(GL_MODELVIEW);
        f->glLoadMatrixf(pViewMatrix.constData());
        f->glMatrixMode(GL_PROJECTION);
        f->glLoadMatrixf(pProjectionMatrix.constData());
    }
    virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color){
        f->glColor3f(color.x(), color.y(), color.z());
        f->glBegin(GL_LINES);
        f->glVertex3f(from.x(), from.y(), from.z());
        f->glVertex3f(to.x(), to.y(), to.z());
        f->glEnd();
    }
    virtual void drawContactPoint(const btVector3 &,const btVector3 &,btScalar,int,const btVector3 &){}
    virtual void reportErrorWarning(const char *){}
    virtual void draw3dText(const btVector3 &,const char *){}
    virtual void setDebugMode(int p){
        m = p;
    }
    int getDebugMode(void) const {return 3;}
    int m;
};

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    OpenGLWidget();

    QVector<Entity*> entities;

    void addEntity(Entity* entity, btCollisionShape* shape, float mass = 0.f);
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
    BulletDebugDrawer_DeprecatedOpenGL* debugDrawer;
    Physics physics;
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
