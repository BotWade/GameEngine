#include "TextureManager.hpp"

vector<Texture*> TextureManager::Textures;

void TextureManager::Init() {
    Textures = vector<Texture*>();
}

Texture* TextureManager::GetTexture(const char* Location, TextureType type) {

    size_t Size = Textures.size();

    for (size_t Index; Index < Size; Index++)
        if (Textures[Index]->path = Location)
            return Textures[Index];

    Texture* texture = new Texture();
    
    if (type == TEXTURE)
        texture->LoadFile(Location);
    else if (type == CUBEMAP)
        texture->LoadCubemap(Location);
    
    Textures.push_back(texture);
    return texture;
}

void TextureManager::Clear() {
    Textures.clear();
    Textures.shrink_to_fit();
}