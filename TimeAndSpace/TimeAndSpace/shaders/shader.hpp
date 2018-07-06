//
//  shader.hpp
//  TimeAndSpace
//
//  Created by Carry on 2018/7/5.
//  Copyright © 2018 Carry. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include "../glad/glad.h"
#include "helpers.hpp"

#include <string>
#include <fstream>
#include <sstream>

#include <iostream>
using namespace std;

class Shader
{
public:
    // the program ID
    unsigned int ID;
    
    // constructor reads and builds the shader
    Shader(const char*, const char*);
    
    void use();
    // utility uniform functions
    void setBool(const string&, bool);
    void setInt(const string&, int);
    void setFloat(const string&, float);
    void setFloat(const string&, float, float, float, float);

private:
    void compileShader(unsigned int, const char*);
};

#endif /* shader_hpp */
