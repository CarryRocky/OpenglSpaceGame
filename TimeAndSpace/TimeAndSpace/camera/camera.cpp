//
//  camera.cpp
//  TimeAndSpace
//
//  Created by Carry on 2018/7/11.
//  Copyright © 2018 Carry. All rights reserved.
//

#include "camera.hpp"

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;
    
    Yaw   += xoffset;
    Pitch += yoffset;
    
    if (constrainPitch)
    {
        adjustPitch();
    }
    
    updateCameraVectors();
}

void Camera::adjustPitch()
{
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;
}

void Camera::processMouseScroll(float yoffset)
{
    if (Zoom >= 1.0f && Zoom <= 45.0f)
        Zoom -= yoffset;
    if (Zoom <= 1.0f)
        Zoom = 1.0f;
    if (Zoom >= 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::dizzying(float dt)
{
    int shakeInt = -1;
    if (shakeRight)
        shakeInt = 1;
    
    Yaw += float(shakeInt) * dizzyVelocity;
    updateCameraVectors();
    
    shakeRight = !shakeRight;
    
    Position += dizzyVelocity * dt * dizzyDir;
    dizzyVelocity += dizzyAcc * dizzyTime;
    dizzyTime += dt;
}
