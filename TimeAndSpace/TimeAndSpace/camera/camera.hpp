//
//  camera.hpp
//  TimeAndSpace
//
//  Created by Carry on 2018/7/11.
//  Copyright © 2018 Carry. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

const float DIZZY_ACC   = -0.1f;

const float SHAKE_TIME  = 0.1f;

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), float yaw = YAW, float pitch = PITCH)
    {
        MovementSpeed = SPEED;
        MouseSensitivity = SENSITIVITY;
        Zoom = ZOOM;
        
        Position = position;
        WorldUp = up;
        Front = front;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
        
        dizzyAcc = DIZZY_ACC;
        shakeRight = true;
        resetDizzyParameter();
    }
    
    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    
    void processKeyboard(Camera_Movement, float);
    
    void processMouseMovement(float, float, GLboolean constrainPitch = true);
    
    void processMouseScroll(float);
    
    void setPosition(glm::vec3 newPos)
    {
        Position = newPos;
    }
    
    void setFront(glm::vec3 newFront)
    {
        Front = newFront;
    }
    
    void setPitch(float newPitch)
    {
        Pitch = newPitch;
    }
    
    float getZoom()
    {
        return Zoom;
    }
    
    float getYaw()
    {
        return Yaw;
    }
    
    float getPitch()
    {
        return Pitch;
    }
    
    glm::vec3 getPosition()
    {
        return Position;
    }
    
    glm::vec3 getWorldCoByCameraCo(float r, float u, float f)
    {
        return Position + r * Right + u * Up + f * Front;
    }
    
    glm::vec3 getDirctionByCameraCo(float r, float u, float f)
    {
        return r * Right + u * Up + f * Front;
    }
    
    void resetDizzyParameter()
    {
        dizzyTime = 0.0f;
        dizzyVelocity = 6.0f;
        dizzyDir = glm::vec3(0,0,0);
        
        shakeTime = SHAKE_TIME;
    }
    
    void startDizzy(glm::vec3 dir)
    {
        dizzyDir = dir;
    }
    
    void dizzying(float);
    
    float getDizzyVelocity()
    {
        return dizzyVelocity;
    }
    
    void adjustPitch();
private:
    void updateCameraVectors();
    
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    
    float dizzyTime;
    float dizzyVelocity;
    glm::vec3 dizzyDir;
    float dizzyAcc;
    bool shakeRight;
    
    float shakeTime;
};

#endif
