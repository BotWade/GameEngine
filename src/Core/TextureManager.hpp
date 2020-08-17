#ifndef _TEXTUREMANAGER_
#define _TEXTUREMANAGER_

#include "../Graphics/Texture.hpp"

#include <vector>

using namespace std;

enum TextureType {
    TEXTURE = 0,
    CUBEMAP = 1,
};

class TextureManager {
public:
    static vector<Texture*> Textures;

    static void Init();
    static Texture* GetTexture(const char* Location, TextureType type = TEXTURE);
    static void Clear();
};

#endif