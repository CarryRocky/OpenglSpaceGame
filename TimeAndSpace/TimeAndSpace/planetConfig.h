//
//  planetConfig.h
//  TimeAndSpace
//
//  Created by Carry on 2018/7/26.
//  Copyright © 2018 Carry. All rights reserved.
//

#ifndef planetConfig_h
#define planetConfig_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
using namespace std;

struct Star
{
    Star(glm::vec3 position, int sid, float pScale = 0.3f, float ss = 20.0f)
    {
        pos = position;
        starId = sid;
        selfSpeed = ss;
        scale = pScale;
    }
    
    glm::vec3 pos;
    int starId;
    float selfSpeed;
    float scale;
};

struct Planet
{
    Planet(float pRadius, int pid, int fid, float pScale = 0.2f, float rs = 1.0f, float ss = 50.0f)
    {
        radius = pRadius;
        planetId = pid;
        fatherId = fid;
        selfSpeed = ss;
        rotateSpeed = rs;
        scale = pScale;
    }
    
    float radius;
    int planetId;
    int fatherId;
    float selfSpeed;
    float rotateSpeed;
    float scale;
};

struct Moon
{
    Moon(float pRadius, int mid, int fid, float pScale = 0.1f, float rs = 4.0f, float ss = 100.0f)
    {
        radius = pRadius;
        moonId = mid;
        fatherId = fid;
        selfSpeed = ss;
        rotateSpeed = rs;
        scale = pScale;
    }
    
    float radius;
    int moonId;
    int fatherId;
    float selfSpeed;
    float rotateSpeed;
    float scale;
};

const vector<Star> sArray{
    Star(glm::vec3(0.0f, 0.0f, 15.0f), 1),
    Star(glm::vec3(15.0f, 0.0f, -15.0f), 2),
    Star(glm::vec3(-30.0f, 0.0f, -30.0f), 3),
};

const vector<Planet> pArray{
    Planet(1.2f, 1, 1, 0.2f, 3),
    Planet(2.5f, 2, 1, 0.2f, 2),
    Planet(5.8f, 3, 1, 0.2f, 1),
    Planet(3.8f, 4, 2, 0.2f, 0.5f),
    Planet(8.8f, 5, 2, 0.2f, 2.5f),
    Planet(1.8f, 6, 3, 0.2f, 2),
    Planet(3.7f, 7, 3, 0.2f, 1.5f),
    Planet(5.8f, 8, 3, 0.2f, 1.0f),
    Planet(9.8f, 9, 3, 0.2f, 0.3f),
};

const vector<Moon> mArray{
    Moon(0.8f, 1, 3),
    Moon(0.4f, 2, 5),
    Moon(0.6f, 3, 6),
    Moon(1.2f, 4, 8),
};

#endif /* planetConfig_h */
