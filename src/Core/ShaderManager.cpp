#include "ShaderManager.hpp"

vector<Shader*> ShaderManager::shaders;

void ShaderManager::Init() {

    shaders = vector<Shader*>();
}

Shader* ShaderManager::GetShader(const char* Location) {

    size_t Size = shaders.size();
    
    for (int Index = 0; Index < Size; Index++) {
        if (shaders[Index]->FilePath == Location)
            return shaders[Index];
    }

    Shader* shader = new Shader();
    shader->ReadFromFile(Location);
    shaders.push_back(shader);
    return shader;
}

void ShaderManager::Clear() {

    for (Shader* shader: shaders)
        shader->Clean();
        
    shaders.clear();
    shaders.shrink_to_fit();
}