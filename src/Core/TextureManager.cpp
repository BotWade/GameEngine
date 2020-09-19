#include "TextureManager.hpp"

vector<Texture*> TextureManager::textures;

Texture* TextureManager::GetTexture(string FilePath, TextureType type) {
    
    for (Texture* texture : textures)
        if (texture->filePath == FilePath)
            return texture;

    Texture* texture = new Texture();
    texture->LoadTexture(FilePath, type);
    textures.push_back(texture);
    return texture;
}

void TextureManager::Clear() {
    for (Texture* texture : textures)
        texture->Clear();
}