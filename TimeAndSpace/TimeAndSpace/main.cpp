//
//  main.cpp
//  TimeAndSpace
//
//  Created by Carry on 2018/6/29.
//  Copyright © 2018 Carry. All rights reserved.
//

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "planetConfig.h"

#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
// by defining STB_IMAGE_IMPLEMENTATION the preprocessor modifies the header file such that it only contains the relevant definition source code, effectively turning the header file into a .cpp file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WIN_WIDTH 750
#define WIN_HEIGHT 750

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

float deltaTime = 0.0f;     // time between current frame and last frame
float lastFrame = 0.0f;     // time of last frame

float lastX = float(WIN_WIDTH/2), lastY = float(WIN_HEIGHT/2);
bool firstMouse = true;

// keep all input code organized
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        camera.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
        camera.setFront(glm::vec3(0.0f, 0.0f, -1.0f));
        camera.setPitch(0.0f);
    }
    
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        camera.setPosition(glm::vec3(0.0f, 3.0f, 3.0f));
        camera.setFront(glm::vec3(0.0f, -1.0f, -1.0f));
        camera.setPitch(-45.0f);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;   // reversed since y-coordinates range from bottom to top
    
    lastX = xpos;
    lastY = ypos;
    
    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(yoffset);
}

// adjust the viewport when user resizes the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void loadTextureFile(unsigned int &texture, const char *fileName, GLenum imgType, bool needFlip)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_set_flip_vertically_on_load(needFlip);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
    if (data)
    {
        // the second argument specifies the mipmap level
        // the third argument tells OpenGL the format of the texture to store
        // the sixth argument should always be zero (some legacy stuff)
        // the seventh and eighth argument specify the format and datatype of the source image
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imgType, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture: " << fileName << endl;
    }
    stbi_image_free(data);
}

void sphereVecPushBack(vector<float> &newVec, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    newVec.push_back(p1.x);
    newVec.push_back(p1.y);
    newVec.push_back(p1.z);
    newVec.push_back(0.5 * p1.x + 0.5);
    newVec.push_back(0.5 * p1.y + 0.5);
    
    newVec.push_back(p2.x);
    newVec.push_back(p2.y);
    newVec.push_back(p2.z);
    newVec.push_back(0.5 * p2.x + 0.5);
    newVec.push_back(0.5 * p2.y + 0.5);
    
    newVec.push_back(p3.x);
    newVec.push_back(p3.y);
    newVec.push_back(p3.z);
    newVec.push_back(0.5 * p3.x + 0.5);
    newVec.push_back(0.5 * p3.y + 0.5);
}

void buildSphere(vector<float> &sphereVec)
{
    for (int j = 0;j < 8;j++)
    {
        vector<float> newVec;
        for (int i = 0;i < sphereVec.size();i = i + 15)
        {
            glm::vec3 point_1(sphereVec[i], sphereVec[i + 1], sphereVec[i + 2]);
            glm::vec3 point_2(sphereVec[i + 5], sphereVec[i + 6], sphereVec[i + 7]);
            glm::vec3 point_3(sphereVec[i + 10], sphereVec[i + 11], sphereVec[i + 12]);
            
            glm::vec3 mid_1 = glm::normalize((point_1 + point_2)/2.0f);
            glm::vec3 mid_2 = glm::normalize((point_1 + point_3)/2.0f);
            glm::vec3 mid_3 = glm::normalize((point_2 + point_3)/2.0f);
            
            sphereVecPushBack(newVec, point_1, mid_1, mid_2);
            sphereVecPushBack(newVec, mid_1, point_2, mid_3);
            sphereVecPushBack(newVec, mid_3, mid_2, mid_1);
            sphereVecPushBack(newVec, mid_2, mid_3, point_3);
        }
        
        sphereVec = newVec;
    }
}

void bindSphereVBO(unsigned int VBO, vector<float> &sphereVec, float *vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // the paramater GL_STATIC_DEAW means: the data will most likely not change at all or very rarely
    glBufferData(GL_ARRAY_BUFFER, sphereVec.size()*sizeof(float), vertices, GL_STATIC_DRAW);
    // the first parameter specifies which vertex attribute to configure
    // pass 0 because has specified the location of the position vertex attribute in the vertex shader with layout (location = 0)
    // because vertex attribute is a vec3 so the second parameter is composed of 3 values
    // the forth parameter specifies if the data wanted to be normalized
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // parameter is the vertex attribute location as its argument
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

unsigned int loadCubemap(vector<string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                         );
            stbi_image_free(data);
        }
        else
        {
            cout << "Cubemap texture failed to load at path: " << faces[i] << endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}

unsigned int loadCubemap(string imgName)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char *data = stbi_load(imgName.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                         );
            stbi_image_free(data);
        }
        else
        {
            cout << "Cubemap texture failed to load at path: " << imgName << endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}

void createStars(Shader *curShader, int sid)
{
    Star curStar = sArray[sid];
    glm::mat4 model;
    model = glm::translate(model, curStar.pos);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(curStar.selfSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
    float scale = curStar.scale;
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    curShader->setMatrix4("model", glm::value_ptr(model));
}

void createPlanets(Shader *curShader, int pid, vector<glm::vec3> &curOffset)
{
    Planet curPlanet = pArray[pid];
    glm::vec3 fatherPos = sArray[curPlanet.fatherId - 1].pos;
    curShader->setVec3("light.position", fatherPos);
    
    glm::mat4 model;
    glm::mat4 rotateUniform;
    float radius = curPlanet.radius;
    float earthX = sin(glfwGetTime() * curPlanet.rotateSpeed) * radius;
    float earthZ = cos(glfwGetTime() * curPlanet.rotateSpeed) * radius;
    glm::vec3 offsetVec = glm::vec3(earthX, 0.0f, earthZ) + fatherPos;
    curOffset.push_back(offsetVec);
    model = glm::translate(model, offsetVec);
    rotateUniform = glm::rotate(rotateUniform, (float)glfwGetTime() * glm::radians(curPlanet.selfSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
    model *= rotateUniform;
    float scale = curPlanet.scale;
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    curShader->setMatrix4("model", glm::value_ptr(model));
    curShader->setMatrix4("rotation", glm::value_ptr(rotateUniform));
}

void createMoons(Shader *curShader, int mid, glm::vec3 previousOffset)
{
    Moon curMoon = mArray[mid];
    int sunId = pArray[curMoon.fatherId - 1].fatherId;
    curShader->setVec3("light.position", sArray[sunId - 1].pos);
    
    glm::mat4 model;
    glm::mat4 rotateUniform;
    float radius = curMoon.radius;
    float moonX = sin(glfwGetTime() * curMoon.rotateSpeed) * radius;
    float moonZ = cos(glfwGetTime() * curMoon.rotateSpeed) * radius;
    glm::vec3 offsetVec = glm::vec3(moonX, 0.0f, moonZ) + previousOffset;
    model = glm::translate(model, offsetVec);
    rotateUniform = glm::rotate(rotateUniform, (float)glfwGetTime() * glm::radians(curMoon.selfSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
    model *= rotateUniform;
    float scale = curMoon.scale;
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    curShader->setMatrix4("model", glm::value_ptr(model));
    curShader->setMatrix4("rotation", glm::value_ptr(rotateUniform));
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // tell GLFW we want to explicitly use the core-profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // need to add following line on Mac OS X
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Time and Space", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback); 
    
    float rootSix = sqrt(6);
    float rootThree = sqrt(3);
    //set up vertex data
    float init_vertices[] = {
        rootSix/3, 0.0f, -rootThree/3,      0.0f, 0.0f,
        -rootSix/3, 0.0f, -rootThree/3,     1.0f, 0.0f,
        0.0f, rootSix/3, rootThree/3,       0.5f, 1.0f,
        
        0.0f, -rootSix/3, rootThree/3,      0.0f, 0.0f,
        rootSix/3, 0.0f, -rootThree/3,      1.0f, 0.0f,
        0.0f, rootSix/3, rootThree/3,       0.5f, 1.0f,

        -rootSix/3, 0.0f, -rootThree/3,     0.0f, 0.0f,
        0.0f, -rootSix/3, rootThree/3,      1.0f, 0.0f,
        0.0f, rootSix/3, rootThree/3,       0.5f, 1.0f,

        -rootSix/3, 0.0f, -rootThree/3,     0.0f, 0.0f,
        rootSix/3, 0.0f, -rootThree/3,      1.0f, 0.0f,
        0.0f, -rootSix/3, rootThree/3,      0.5f, 1.0f,
    };
    
    vector<float> sphereVec(begin(init_vertices),end(init_vertices));
    buildSphere(sphereVec);
    float *vertices = new float[sphereVec.size()];
    for (int i = 0;i < sphereVec.size();i++)
    {
        vertices[i] = sphereVec[i];
    }
    
    float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.fs");
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
    
    // the advantage of using those buffer objects is that we can send large batches of data all at once to the graphics card without having to send data a vertex a time
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    bindSphereVBO(VBO, sphereVec, vertices);
    
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    bindSphereVBO(VBO, sphereVec, vertices);
    
    vector<string> faces
    {
        "imgs/right.tga",
        "imgs/left.tga",
        "imgs/top.tga",
        "imgs/bottom.tga",
        "imgs/front.tga",
        "imgs/back.tga"
    };
    unsigned int cubemapTexture = loadCubemap(faces);
    
    // texture
//    unsigned int texture1, texture2, texture3;
//    // load and generate the texture
//    loadTextureFile(texture1, "imgs/sun.jpg", GL_RGB, true);
//    loadTextureFile(texture2, "imgs/earth.jpg", GL_RGB, true);
//    loadTextureFile(texture3, "imgs/moon.jpg", GL_RGB, true);

    vector<unsigned int> sunTexture;
    for (int i = 0; i < sArray.size(); i++)
    {
        sunTexture.push_back(loadCubemap(sArray[i].img));
    }
    vector<unsigned int> planetTexture;
    for (int i = 0; i < pArray.size(); i++)
    {
        planetTexture.push_back(loadCubemap(pArray[i].img));
    }
    vector<unsigned int> moonTexture;
    for (int i = 0; i < mArray.size(); i++)
    {
        moonTexture.push_back(loadCubemap(mArray[i].img));
    }
    
    Shader lightObjShader("shaders/light.vs", "shaders/light.fs");
    // finding the uniform location does not need to use the shader program first, but updating a uniform does need to first use the program
    lightObjShader.use();
    lightObjShader.setInt("material.diffuse", 0);
    
    Shader lightSourceShader("shaders/source.vs", "shaders/source.fs");
    lightSourceShader.use();
    lightSourceShader.setInt("texture1", 0);
    
    glEnable(GL_DEPTH_TEST);
    
    // hide the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    Model ourModel("/Users/carry/Downloads/Smoke_Track.obj");
    
    Shader modelShader("shaders/comet.vs", "shaders/comet.fs");
    
    glm::vec3 initComet = camera.getWorldCoByCameraCo(10, 10, 20);
    int randomAngle = rand() % 70 + 10;
    glm::vec3 comtDirection = camera.getDirctionByCameraCo(-cos(glm::radians(float(randomAngle))), -sin(glm::radians(float(randomAngle))), 0);
    float cometTime = 0.0f;
    glm::vec3 yawAxial = glm::normalize(camera.getDirctionByCameraCo(0.0f, 1.0f, 0.0f));
    glm::vec3 pitchAxial = glm::normalize(camera.getDirctionByCameraCo(0.0f, 0.0f, -1.0f));
    float cometRotateAngle = -90.0f - camera.getYaw() - 90.0f;
    float rightAngle = camera.getPitch();
    glm::vec3 rightAxial = glm::normalize(camera.getDirctionByCameraCo(1.0f, 0.0f, 0.0f));
    
    bool isFirstLoop = true;
    
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        if (isFirstLoop)
        {
            lastFrame = currentFrame;
            isFirstLoop = false;
        }
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // input
        processInput(window);
        
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.getZoom()), float(WIN_WIDTH / WIN_HEIGHT), 0.1f, 100.0f);
        
        // sun
        lightSourceShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(lightVAO);

        glm::mat4 view;
        view = camera.getViewMatrix();
        lightSourceShader.setMatrix4("view", glm::value_ptr(view));
        lightSourceShader.setMatrix4("projection", glm::value_ptr(projection));
        
        for (int i = 0; i < sArray.size(); i++)
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, sunTexture[i]);
            createStars(&lightSourceShader, i);
            glDrawArrays(GL_TRIANGLES, 0, sphereVec.size()/5);
        }
        
        if (cometTime >= 10.0f)
        {
            cometTime = 0.0f;
            initComet = camera.getWorldCoByCameraCo(10, 10, 20);
            randomAngle = rand() % 70 + 10;
            comtDirection = camera.getDirctionByCameraCo(-cos(glm::radians(float(randomAngle))), -sin(glm::radians(float(randomAngle))), 0);
            yawAxial = glm::normalize(camera.getDirctionByCameraCo(0.0f, 1.0f, 0.0f));
            pitchAxial = glm::normalize(camera.getDirctionByCameraCo(0.0f, 0.0f, -1.0f));
            cometRotateAngle = -90.0f - camera.getYaw() - 90.0f;
            rightAngle = camera.getPitch();
            rightAxial = glm::normalize(camera.getDirctionByCameraCo(1.0f, 0.0f, 0.0f));
        }
        
        cometTime += deltaTime;
        
        // comet
        modelShader.use();
        modelShader.setMatrix4("view", glm::value_ptr(view));
        modelShader.setMatrix4("projection", glm::value_ptr(projection));
        glm::mat4 cometModel;
        glm::mat4 cometNormal;
        initComet += 0.2f*comtDirection;
        cometModel = glm::translate(cometModel, initComet);
        cometNormal = glm::rotate(cometNormal, glm::radians(float(randomAngle)), pitchAxial);
        cometNormal = glm::rotate(cometNormal, glm::radians(cometRotateAngle), yawAxial);
        cometNormal = glm::rotate(cometNormal, glm::radians(rightAngle), rightAxial);
        cometModel *= cometNormal;
        cometModel = glm::scale(cometModel, glm::vec3(0.01f, 0.01f, 0.01f));
        modelShader.setMatrix4("model", glm::value_ptr(cometModel));
        modelShader.setVec3("lightPos", camera.getPosition());
        modelShader.setMatrix4("rotation", glm::value_ptr(cometNormal));
        ourModel.Draw(modelShader);
        
        // earth
        lightObjShader.use();
//        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        
        lightObjShader.setFloat("light.constant", 1.0f);
        lightObjShader.setFloat("light.linear", 0.027f);
        lightObjShader.setFloat("light.quadratic", 0.0028f);
        
        // the ambient light is usually set to a low intensity, otherwise it'll be too dominant
        lightObjShader.setFloat("light.ambient", 0.2f, 0.2f, 0.2f);
        // the diffuse component of a light source is usually set to the exact color a light to have
        // often a bright white color
        lightObjShader.setFloat("light.diffuse", 0.9f, 0.9f, 0.9f);
        // the specular component is usually kept at vec3(1.0) shining at full intensity
        lightObjShader.setFloat("light.specular", 1.0f, 1.0f, 1.0f);
        lightObjShader.setVec3("viewPos", camera.getPosition());
        
        lightObjShader.setFloat("material.specular", 0.5f, 0.5f, 0.5f);
        lightObjShader.setFloat("material.shininess", 32.0f);
        
        lightObjShader.setMatrix4("view", glm::value_ptr(view));
        lightObjShader.setMatrix4("projection", glm::value_ptr(projection));
        
        vector<glm::vec3> offsetVec;
        for (int i = 0; i < pArray.size(); i++)
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, planetTexture[i]);
            createPlanets(&lightObjShader, i, offsetVec);
            glDrawArrays(GL_TRIANGLES, 0, sphereVec.size()/5);
        }
        
        // moon
//        glBindTexture(GL_TEXTURE_2D, texture3);
        
        for (int i = 0; i < mArray.size(); i++)
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, moonTexture[i]);
            createMoons(&lightObjShader, i, offsetVec[mArray[i].fatherId - 1]);
            glDrawArrays(GL_TRIANGLES, 0, sphereVec.size()/5);
        }
        
        // skybox
        // change depth function so depth test passes when values are equal to depth buffer's content
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        // remove the translation section of transformation matrices by taking the upper-left 3x3 matrix of the 4x4 matrix
        glm::mat4 skyView = glm::mat4(glm::mat3(camera.getViewMatrix()));
        skyboxShader.setMatrix4("view", glm::value_ptr(skyView));
        skyboxShader.setMatrix4("projection", glm::value_ptr(projection));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // set depth function back to default
        glDepthFunc(GL_LESS);
        
        glfwSwapBuffers(window);
        // check if any events are triggered (like keyboard input or mouse movement events)
        glfwPollEvents();
    }
    
    // properly clean all resources that were allocated
    glfwTerminate();
    
    delete []vertices;
    
    return 0;
}
