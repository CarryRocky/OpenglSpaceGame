//
//  main.cpp
//  TimeAndSpace
//
//  Created by Carry on 2018/6/29.
//  Copyright © 2018 Carry. All rights reserved.
//

#include "glad/glad.h"
#include "shader.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

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
    float vertices[] = {
        // position             // color
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,
//        0.5f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,
        
    };
    unsigned int indices[] = {
        0, 1, 2,
//        0, 2, 3
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
    
    Shader testShader("triangle.vs","triangle.fs");
    
    // the first parameter specifies which vertex attribute to configure
    // pass 0 because has specified the location of the position vertex attribute in the vertex shader with layout (location = 0)
    // because vertex attribute is a vec3 so the second parameter is composed of 3 values
    // the forth parameter specifies if the data wanted to be normalized
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // parameter is the vertex attribute location as its argument
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // finding the uniform location does not need to use the shader program first, but updating a uniform does need to first use the program
        testShader.use();
        
//        float timeValue = glfwGetTime();
//        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
//        testShader.setFloat("ourColor", 0.5f, greenValue, 0.5f, 1.0f);
        
        glBindVertexArray(VAO);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
