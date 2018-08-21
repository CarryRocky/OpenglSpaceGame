//
//  model.hpp
//  TimeAndSpace
//
//  Created by Carry on 2018/8/2.
//  Copyright © 2018 Carry. All rights reserved.
//

#ifndef model_hpp
#define model_hpp

#include "mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

class Model
{
public:
    Model(char *path)
    {
        loadModel(path);
    }
    void Draw(Shader);
    
    int getMeshesSize()
    {
        return int(meshes.size());
    }
    
    unsigned int getOneMeshVAO(int index)
    {
        return meshes[index].getVAO();
    }
    
    Mesh getOneMesh(int index)
    {
        return meshes[index];
    }
    
    // suitable for instancing use
    unsigned int getPublicTextureId()
    {
        return textures_loaded[0].t_id;
    }
private:
    vector<Mesh> meshes;
    vector<Texture> textures_loaded;
    string directory;

    void loadModel(string path);
    void processNode(aiNode*, const aiScene*);
    Mesh processMesh(aiMesh*, const aiScene*);
    vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, string);
    unsigned int TextureFromFile(const char*, const string&, bool gamma = false);
};

#endif /* model_hpp */
