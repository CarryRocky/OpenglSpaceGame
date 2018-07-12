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

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
};

#endif
