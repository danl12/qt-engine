#include "camera.h"

#include <QtMath>

Camera::Camera(QVector3D position, QVector3D up, float yaw, float pitch):
    position(position),
    front(0.f, 0.f, -1.f),
    worldUp(up),
    yaw(yaw),
    pitch(pitch),
    movementSpeed(22.5f),
    mouseSensitivity(0.1f)
{
    updateVectors();
}

QMatrix4x4 Camera::getViewMatrix() const
{
    QMatrix4x4 m;
    m.lookAt(position, position + front, up);
    return m;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw   += xOffset;
    pitch += yOffset;

    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    updateVectors();
}

void Camera::updateVectors()
{
    front = QVector3D(
                qCos(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)),
                qSin(qDegreesToRadians(pitch)),
                qSin(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch))
            );
    front.normalize();

    right = QVector3D::crossProduct(front, worldUp);
    right.normalize();

    up = QVector3D::crossProduct(right, front);
    up.normalize();
}
