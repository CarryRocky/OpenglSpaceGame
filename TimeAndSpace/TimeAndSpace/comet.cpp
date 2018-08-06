//
//  comet.cpp
//  TimeAndSpace
//
//  Created by Carry on 2018/8/6.
//  Copyright © 2018 Carry. All rights reserved.
//

#include "comet.hpp"

Comet::Comet(Camera *pCamera)
{
    lifetime = 0.0f;
    if (rand() % 2 == 0)
    {
        position = pCamera->getWorldCoByCameraCo(10, 10, 20);
        randomAngle = rand() % 70 + 10;
    }
    else
    {
        position = pCamera->getWorldCoByCameraCo(-10, 10, 20);
        randomAngle = rand() % 70 + 100;
    }
    direction = pCamera->getDirctionByCameraCo(-cos(glm::radians(float(randomAngle))), -sin(glm::radians(float(randomAngle))), 0);
    yawAxial = glm::normalize(pCamera->getDirctionByCameraCo(0.0f, 1.0f, 0.0f));
    pitchAxial = glm::normalize(pCamera->getDirctionByCameraCo(0.0f, 0.0f, -1.0f));
    rightAxial = glm::normalize(pCamera->getDirctionByCameraCo(1.0f, 0.0f, 0.0f));
    cometRotateAngle = -pCamera->getYaw() - 180.0f;
    rightAngle = pCamera->getPitch();
}

void Comet::update(Shader *pShader, float dt)
{
    glm::mat4 cometModel;
    glm::mat4 cometNormal;
    position += dt * 6.0f * direction;
    
    cometModel = glm::translate(cometModel, position);
    cometNormal = glm::rotate(cometNormal, glm::radians(float(randomAngle)), pitchAxial);
    cometNormal = glm::rotate(cometNormal, glm::radians(float(cometRotateAngle)), yawAxial);
    cometNormal = glm::rotate(cometNormal, glm::radians(float(rightAngle)), rightAxial);
    cometModel *= cometNormal;
    cometModel = glm::scale(cometModel, glm::vec3(0.01f, 0.01f, 0.01f));
    pShader->setMatrix4("model", glm::value_ptr(cometModel));
    pShader->setMatrix4("rotation", glm::value_ptr(cometNormal));
    
    lifetime += dt;
}

