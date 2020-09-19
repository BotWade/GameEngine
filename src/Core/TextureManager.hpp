#ifndef _TEXTUREMANAGER_
#define _TEXTUREMANAGER_

#include "../Graphics/Texture.hpp"
#include <vector>

using namespace std;

class TextureManager {
public:
    static vector<Texture*> textures;

    static Texture* GetTexture(string FilePath, TextureType type = TEXTURE2D);
    static void Clear();
};

#endif