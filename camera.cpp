#include "camera.h"

Camera::Camera()
{
    tolerance = 0.025;

    lookAt = QVector3D(0,0,0);
    distance = 20;

    //clear out existing camera rotations
    rotationX = QQuaternion();
    rotationY = QQuaternion();

    //set default camera rotation
    rotate(45, 15);


    saveStateAsDefaults();
}

void Camera::setViewVectors()
{
    //rotate the up vector opposite the camera rotation to keep Y
    //UP on the screen
    Up = rotationY.conjugate().rotatedVector(QVector3D(0, 1, 0));
    Up = rotationX.conjugate().rotatedVector(Up);

    //rotate the right vector opposite the camera rotation to keep X
    //RIGHT on the screen
    Right = rotationY.conjugate().rotatedVector(QVector3D(1, 0, 0));
    Right = rotationX.conjugate().rotatedVector(Right);
}

void Camera::reset()
{
    lookAt = default_lookAt;
    distance = default_distance;
    rotationX = default_rotationX;
    rotationY = default_rotationY;
}

void Camera::saveStateAsDefaults()
{
    default_lookAt = lookAt;
    default_distance = distance;
    default_rotationX = rotationX;
    default_rotationY = rotationY;
}

void Camera::rotate(float xdelta, float ydelta)
{
    //determine whether or not the Y axis rotation needs to be flipped
    //so that moving the mouse right does not rotate the grid left.
    //an abs value < .7 means that the Y axis is pointing down.
    int Y = 1;
    if (fabs(rotationY.scalar()) <= .7)
        Y = -1;

    //rotate Y first so that we can rotate the XZ plane along the camera's
    //perpendicular vector
    QQuaternion q = QQuaternion::fromAxisAndAngle(0, Y, 0, xdelta);
    rotationX *= q;
    rotationX.normalize();

    //now rotate around the xz (when rotation is applied) plane
    q = QQuaternion::fromAxisAndAngle(1, 0, 0, ydelta);
    rotationY *= q;
    rotationY.normalize();

    //now that the camera rotations have been updated, we need to make sure
    //that the UP and RIGHT view vectors are also updated
    setViewVectors();
}

void Camera::pan(float xdelta, float ydelta)
{
    //move the lookat vector based on the current UP and RIGHT vectors
    lookAt += Up * ydelta * tolerance * distance * tolerance;
    lookAt += Right * -xdelta * tolerance * distance * tolerance;
}

void Camera::zoom(float xdelta)
{
    distance -= xdelta * tolerance * distance * tolerance;
}

void Camera::dolly(float delta)
{
    QVector3D forward = QVector3D::crossProduct(Right, Up);
    forward.normalize();

    distance -= delta;
    lookAt -= forward * delta;
}
