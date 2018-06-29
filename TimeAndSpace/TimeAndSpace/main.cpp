//
//  main.cpp
//  TimeAndSpace
//
//  Created by Carry on 2018/6/29.
//  Copyright © 2018 Carry. All rights reserved.
//

#include "glad/glad.h"
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
    cout<<"cur width and height "<<width<<" "<<height<<endl;
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
    
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    while(!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glfwSwapBuffers(window);
        // check if any events are triggered (like keyboard input or mouse movement events)
        glfwPollEvents();
    }
    
    // properly clean all resources that were allocated
    glfwTerminate();
    return 0;
}
