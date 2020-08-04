#ifndef _MESH_
#define _MESH_

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include "ObjLoader/Loader.hpp"
#include "../Graphics/Shader.hpp"

using namespace std;

class Mesh {
public:

    unsigned int VBO, VAO, EBO;

    const char* Path;

    bool AttributesProcessed = false;
    
    vector<Vertex> Data;
    vector<unsigned int> Indices;
    bool LoadMesh(const char* FilePath);
    void ProcessAttributes(int Type, unsigned long TypeSize, Shader* shader);
    void Bind();
    void UnBind();
    void Draw();
};

#endif