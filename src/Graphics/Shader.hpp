#ifndef _SHADER_
#define _SHADER_

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "../Math/Math.hpp"
#include "../Core/FileManager.hpp"
#include "../Core/Debug.hpp"
#include "Uniform.hpp"

class Shader {    
public:
    unsigned int ProgramId;
    unsigned int VertexId;
    unsigned int FragmentId;
    
    const char* Path;

    vector<int> Attributes;
    vector<Uniform> Uniforms;
    
    void ReadFromFile(const char* ShaderFile);
    bool CompileShader(const char* ShaderDta, unsigned int& Id, unsigned int ShaderType, char* Error);
    bool CompileProgram(unsigned int& Id, vector<unsigned int> Shaders, char* Error);
    bool CheckShader(unsigned int ShaderId, char* Error);
    bool CheckProgram(unsigned int ProgramId, char* Error);
    void Bind();
    void UnBind();
    unsigned int GetUniformId(string Name);

    ~Shader();
};

#endif