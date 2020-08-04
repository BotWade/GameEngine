#ifndef _TEXTURE_
#define _TEXTURE_

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <vector>
#include "../Core/FileManager.hpp"

class Texture {
public:
    unsigned int TextureId;
    int Width;
    int Height;
    int nrChannels;

    bool LoadFile(const char* Path);
    bool LoadCubemap(const char* Path);
    void Bind();
    void UnBind();
};

#endif