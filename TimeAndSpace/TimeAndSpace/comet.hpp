//
//  comet.hpp
//  TimeAndSpace
//
//  Created by Carry on 2018/8/6.
//  Copyright © 2018 Carry. All rights reserved.
//

#ifndef comet_hpp
#define comet_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

#include "camera.hpp"
#include "shader.hpp"

class Comet
{
public:
    Comet(Camera*);
    void update(Shader*, float);
    
    glm::vec3 getPos()
    {
        return position;
    }
    
    float getLifeTime()
    {
        return lifetime;
    }
private:
    glm::vec3 position;
    glm::vec3 bornPoint;
    glm::vec3 direction;
    float lifetime;
    
    int randomAngle;
    float cometRotateAngle;
    float rightAngle;
    glm::vec3 yawAxial;
    glm::vec3 pitchAxial;
    glm::vec3 rightAxial;
};

#endif /* comet_hpp */
