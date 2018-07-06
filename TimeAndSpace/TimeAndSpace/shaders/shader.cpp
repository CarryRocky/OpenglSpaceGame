//
//  shader.cpp
//  TimeAndSpace
//
//  Created by Carry on 2018/7/5.
//  Copyright © 2018 Carry. All rights reserved.
//

#include "shader.hpp"

void Shader::compileShader(unsigned int shader, const char* fileChar)
{
    // the second argument specifies how many strings we're passing as source code
    glShaderSource(shader, 1, &fileChar, NULL);
    glCompileShader(shader);
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FILE:: "<< &fileChar << " COMPILATION_FAILED" << endl;
        cout << infoLog << endl;
    }
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    string vsFile, fsFile;
    ReadFile(vertexPath, vsFile);
    ReadFile(fragmentPath, fsFile);
    const char *vsChar = vsFile.c_str();
    const char *fsChar = fsFile.c_str();
    
    compileShader(vertexShader, vsChar);
    compileShader(fragmentShader, fsChar);
    
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    int  success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        cout << "ERROR::LINKING::SHADER::PROGRAM::FAILED" << endl;
        cout << infoLog << endl;
    }
    // delete the shader objects once have linked them into the program object
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const string &name, bool value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const string &name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const string &name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value1, float value2, float value3, float value4)
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}
