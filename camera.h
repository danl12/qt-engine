#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    QVector3D position, front, right, up, worldUp;

    Camera(QVector3D position, QVector3D up, float yaw = -90.f, float pitch = 0.f);

    QMatrix4x4 getViewMatrix() const;

    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    void processKeyboard(CameraMovement direction, float deltaTime);
private:
    float yaw, pitch, movementSpeed, mouseSensitivity;

    void updateVectors();
};

#endif // CAMERA_H
