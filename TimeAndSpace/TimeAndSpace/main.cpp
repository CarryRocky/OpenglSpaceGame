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
using namespace std;

#include "shader.hpp"
#include "camera.hpp"
// by defining STB_IMAGE_IMPLEMENTATION the preprocessor modifies the header file such that it only contains the relevant definition source code, effectively turning the header file into a .cpp file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WIN_WIDTH 750
#define WIN_HEIGHT 750

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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
        camera.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
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
    
//    // texture
//    unsigned int texture1, texture2, texture3;
//    // load and generate the texture
//    loadTextureFile(texture1, "imgs/sun.jpg", GL_RGB, true);
//    loadTextureFile(texture2, "imgs/earth.jpg", GL_RGB, true);
//    loadTextureFile(texture3, "imgs/moon.jpg", GL_RGB, true);
//
//    Shader testShader("shaders/triangle.vs", "shaders/triangle.fs");
//
//    testShader.use();
//    testShader.setInt("texture1", 0);
////    testShader.setInt("texture2", 1);
    
    Shader lightSourceShader("shaders/source.vs", "shaders/source.fs");
    Shader lightObjShader("shaders/light.vs", "shaders/light.fs");
    
    glEnable(GL_DEPTH_TEST);
    
    // hide the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // input
        processInput(window);
        
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
//        // finding the uniform location does not need to use the shader program first, but updating a uniform does need to first use the program
//        testShader.use();
//
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, texture1);
////        glActiveTexture(GL_TEXTURE1);
////        glBindTexture(GL_TEXTURE_2D, texture2);
//        glBindVertexArray(VAO);
        
        lightSourceShader.use();
        glBindVertexArray(lightVAO);

        glm::mat4 view;
        view = camera.getViewMatrix();
        lightSourceShader.setMatrix4("view", glm::value_ptr(view));
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.getZoom()), float(WIN_WIDTH / WIN_HEIGHT), 0.1f, 100.0f);
        lightSourceShader.setMatrix4("projection", glm::value_ptr(projection));
        glm::mat4 model;
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        lightSourceShader.setMatrix4("model", glm::value_ptr(model));

//        // the last argument specifies an offset in the EBO
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, sphereVec.size()/5);
        
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, texture2);
        lightObjShader.use();
        glBindVertexArray(VAO);
        
        // the ambient light is usually set to a low intensity, otherwise it'll be too dominant
        lightObjShader.setFloat("light.ambient",  0.2f, 0.2f, 0.2f);
        // the diffuse component of a light source is usually set to the exact color a light to have
        // often a bright white color
        lightObjShader.setFloat("light.diffuse",  0.5f, 0.5f, 0.5f);
        // the specular component is usually kept at vec3(1.0) shining at full intensity
        lightObjShader.setFloat("light.specular", 1.0f, 1.0f, 1.0f);
        lightObjShader.setFloat("light.position", 0.0f, 0.0f, 1.0f);
        lightObjShader.setVec3("viewPos", camera.getPosition());
        
        lightObjShader.setFloat("material.ambient",  1.0f, 0.5f, 0.31f);
        lightObjShader.setFloat("material.diffuse",  1.0f, 0.5f, 0.31f);
        lightObjShader.setFloat("material.specular", 0.5f, 0.5f, 0.5f);
        lightObjShader.setFloat("material.shininess", 32.0f);
        
        lightObjShader.setMatrix4("view", glm::value_ptr(view));
        lightObjShader.setMatrix4("projection", glm::value_ptr(projection));
        
        glm::mat4 model2;
        glm::mat4 rotateUniform2;
        float radius = 0.8f;
        float earthX = sin(glfwGetTime()) * radius;
        float earthZ = cos(glfwGetTime()) * radius;
        model2 = glm::translate(model2, glm::vec3(earthX, 0.0f, earthZ));
        rotateUniform2 = glm::rotate(rotateUniform2, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model2 *= rotateUniform2;
        model2 = glm::scale(model2, glm::vec3(0.1f, 0.1f, 0.1f));
        lightObjShader.setMatrix4("model", glm::value_ptr(model2));
        lightObjShader.setMatrix4("rotation", glm::value_ptr(rotateUniform2));
        
        glDrawArrays(GL_TRIANGLES, 0, sphereVec.size()/5);
        
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, texture3);
        glm::mat4 model3;
        glm::mat4 rotateUniform3;
        float radius2 = 0.25f;
        float earthX2 = sin(glfwGetTime()*4) * radius2;
        float earthZ2 = cos(glfwGetTime()*4) * radius2;
        model3 = glm::translate(model3, glm::vec3(earthX2, 0.0f, earthZ2) + glm::vec3(earthX, 0.0f, earthZ));
        rotateUniform3 = glm::rotate(rotateUniform3, (float)glfwGetTime() * glm::radians(100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model3 *= rotateUniform3;
        model3 = glm::scale(model3, glm::vec3(0.04f, 0.04f, 0.04f));
        lightObjShader.setMatrix4("model", glm::value_ptr(model3));
        lightObjShader.setMatrix4("rotation", glm::value_ptr(rotateUniform3));

        glDrawArrays(GL_TRIANGLES, 0, sphereVec.size()/5);
        
        glfwSwapBuffers(window);
        // check if any events are triggered (like keyboard input or mouse movement events)
        glfwPollEvents();
    }
    
    // properly clean all resources that were allocated
    glfwTerminate();
    
    delete []vertices;
    return 0;
}
