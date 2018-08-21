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
    Star(glm::vec3(0.0f, 10.0f, 18.0f), 1, "imgs/star_1.jpg"),
    Star(glm::vec3(15.0f, -5.0f, -15.0f), 2, "imgs/star_2.jpg"),
    Star(glm::vec3(-30.0f, 3.0f, -30.0f), 3, "imgs/star_3.jpg"),
    Star(glm::vec3(25.0f, -20.0f, 33.0f), 4, "imgs/star_4.jpg"),
    Star(glm::vec3(-31.0f, -15.0f, 28.0f), 5, "imgs/star_5.jpg"),
    Star(glm::vec3(4.0f, 25.0f, -32.0f), 6, "imgs/star_6.jpg"),
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
    
    Planet(3.4f, 10, 4, "imgs/planet_10.jpg", 0.2f, 0.4),
    Planet(6.0f, 11, 4, "imgs/planet_11.jpg", 0.2f, 0.6),
    Planet(12.6f, 12, 4, "imgs/planet_12.jpg", 0.2f, 0.1),

    Planet(2.7f, 13, 5, "imgs/planet_13.jpg", 0.2f, 0.1),
    Planet(5.9f, 14, 5, "imgs/planet_14.jpg", 0.2f, 0.2),
    Planet(19.6f, 15, 5, "imgs/planet_15.jpg", 0.2f, 0.03),

    Planet(4.0f, 16, 6, "imgs/planet_16.jpg", 0.2f, 0.7f),
    Planet(7.4f, 17, 6, "imgs/planet_17.jpg", 0.2f, 0.5f),
    Planet(12.0f, 18, 6, "imgs/planet_18.jpg", 0.2f, 0.3f),
    Planet(23.1f, 19, 6, "imgs/planet_19.jpg", 0.2f, 0.4f),
};

const vector<Moon> mArray{
    Moon(1.3f, 1, 3, "imgs/moon_1.jpg"),

    Moon(1.0f, 2, 5, "imgs/moon_2.jpg"),
    Moon(1.1f, 3, 6, "imgs/moon_3.jpg"),
    Moon(1.7f, 4, 10, "imgs/moon_4.jpg"),
    
    Moon(1.2f, 5, 10, "imgs/moon_5.jpg"),
    Moon(1.0f, 6, 11, "imgs/moon_6.jpg"),
    Moon(1.1f, 7, 12, "imgs/moon_7.jpg"),

    Moon(1.5f, 8, 15, "imgs/moon_8.jpg"),

    Moon(1.1f, 9, 16, "imgs/moon_9.jpg"),
};

struct SolarInfo
{
    float range;
    vector<int> planets;
    vector<int> moons;
    bool isCheck;
};

#endif /* planetConfig_h */
