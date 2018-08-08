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
    Star(glm::vec3 position, int sid, string imgPath, float pScale = 0.3f, float ss = 20.0f)
    {
        pos = position;
        starId = sid;
        selfSpeed = ss;
        scale = pScale;
        img = imgPath;
    }
    
    glm::vec3 pos;
    int starId;
    float selfSpeed;
    float scale;
    string img;
};

struct Planet
{
    Planet(float pRadius, int pid, int fid, string imgPath, float pScale = 0.2f, float rs = 1.0f, float ss = 50.0f)
    {
        radius = pRadius;
        planetId = pid;
        fatherId = fid;
        selfSpeed = ss;
        rotateSpeed = rs;
        scale = pScale;
        img = imgPath;
    }
    
    float radius;
    int planetId;
    int fatherId;
    float selfSpeed;
    float rotateSpeed;
    float scale;
    string img;
};

struct Moon
{
    Moon(float pRadius, int mid, int fid, string imgPath, float pScale = 0.1f, float rs = 4.0f, float ss = 100.0f)
    {
        radius = pRadius;
        moonId = mid;
        fatherId = fid;
        selfSpeed = ss;
        rotateSpeed = rs;
        scale = pScale;
        img = imgPath;
    }
    
    float radius;
    int moonId;
    int fatherId;
    float selfSpeed;
    float rotateSpeed;
    float scale;
    string img;
};

const vector<Star> sArray{
    Star(glm::vec3(0.0f, 0.0f, 18.0f), 1, "imgs/star_1.jpg"),
    Star(glm::vec3(15.0f, 0.0f, -15.0f), 2, "imgs/star_2.jpg"),
    Star(glm::vec3(-30.0f, 0.0f, -30.0f), 3, "imgs/star_3.jpg"),
};

const vector<Planet> pArray{
    Planet(2.4f, 1, 1, "imgs/planet_1.jpg", 0.2f, 0.6),
    Planet(5.0f, 2, 1, "imgs/planet_2.jpg", 0.2f, 0.4),
    Planet(11.6f, 3, 1, "imgs/planet_3.jpg", 0.2f, 0.2),
    
    Planet(3.8f, 4, 2, "imgs/planet_4.jpg", 0.2f, 0.1f),
    Planet(8.8f, 5, 2, "imgs/planet_5.jpg", 0.2f, 0.5f),
    
    Planet(2.0f, 6, 3, "imgs/planet_6.jpg", 0.2f, 0.4f),
    Planet(7.4f, 7, 3, "imgs/planet_7.jpg", 0.2f, 0.3f),
    Planet(11.0f, 8, 3, "imgs/planet_8.jpg", 0.2f, 0.2f),
    Planet(20.1f, 9, 3, "imgs/planet_9.jpg", 0.2f, 0.06f),
};

const vector<Moon> mArray{
    Moon(0.8f, 1, 3, "imgs/moon_1.jpg"),
    Moon(0.4f, 2, 5, "imgs/moon_2.jpg"),
    Moon(0.6f, 3, 6, "imgs/moon_3.jpg"),
    Moon(1.2f, 4, 8, "imgs/moon_4.jpg"),
};

struct SolarInfo
{
    float range;
    vector<int> planets;
    vector<int> moons;
    bool isCheck;
};

#endif /* planetConfig_h */
