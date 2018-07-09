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
using namespace std;

#include "shader.hpp"
// by defining STB_IMAGE_IMPLEMENTATION the preprocessor modifies the header file such that it only contains the relevant definition source code, effectively turning the header file into a .cpp file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WIN_WIDTH 1200
#define WIN_HEIGHT 750

// keep all input code organized
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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
    
    //set up vertex data
//    // triangle
//    float vertices[] = {
//        // position             // color
//        0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,
//        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
//        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,
////        0.5f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,
//    };
    
    // rectangle
    float vertices[] = {
        // positions            // colors           // texture coords
        0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // the advantage of using those buffer objects is that we can send large batches of data all at once to the graphics card without having to send data a vertex a time
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // the paramater GL_STATIC_DEAW means: the data will most likely not change at all or very rarely
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // texture
    unsigned int texture1, texture2;
    // load and generate the texture
    loadTextureFile(texture1, "imgs/wood.jpg", GL_RGB, true);
    loadTextureFile(texture2, "imgs/leaf.png", GL_RGBA, true);
    
    Shader testShader("shaders/triangle.vs","shaders/triangle.fs");
    
    // the first parameter specifies which vertex attribute to configure
    // pass 0 because has specified the location of the position vertex attribute in the vertex shader with layout (location = 0)
    // because vertex attribute is a vec3 so the second parameter is composed of 3 values
    // the forth parameter specifies if the data wanted to be normalized
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // parameter is the vertex attribute location as its argument
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    testShader.use();
    testShader.setInt("texture1", 0);
    testShader.setInt("texture2", 1);
    
    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // finding the uniform location does not need to use the shader program first, but updating a uniform does need to first use the program
        testShader.use();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        // the last argument specifies an offset in the EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        // check if any events are triggered (like keyboard input or mouse movement events)
        glfwPollEvents();
    }
    
    // properly clean all resources that were allocated
    glfwTerminate();
    return 0;
}
