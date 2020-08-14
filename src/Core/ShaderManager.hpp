#ifndef _SHADERMANAGER_
#define _SHADERMANAGER_

#include "../Graphics/Shader.hpp"
#include <vector>

using namespace std;

class ShaderManager {
public:
    static vector<Shader*> shaders;

    static void Init();
    static Shader* GetShader(const char* Location);
    static void Clear();
};

#endif