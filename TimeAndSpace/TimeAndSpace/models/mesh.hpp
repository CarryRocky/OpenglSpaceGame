//
//  mesh.hpp
//  TimeAndSpace
//
//  Created by Carry on 2018/8/2.
//  Copyright © 2018 Carry. All rights reserved.
//

#ifndef mesh_hpp
#define mesh_hpp

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
using namespace std;

#include "shader.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int t_id;
    string t_type;
};

class Mesh {
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    
    Mesh(vector<Vertex>, vector<unsigned int>, vector<Texture>);
    void Draw(Shader&);
private:
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

#endif /* mesh_hpp */
