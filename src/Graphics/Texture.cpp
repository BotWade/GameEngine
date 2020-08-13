#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image"

bool Texture::LoadFile(const char* Path) {

    glGenTextures(1, &TextureId);
    glBindTexture(GL_TEXTURE_2D, TextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (FileManager::FileExists((FileManager::GetGamePath() + (string)Path).c_str())) {
        
        Debug::Alert("Texture doesn't exist - " + (string)Path);
        return false;
    }

    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load((FileManager::GetGamePath() + (string)Path).c_str(), &Width, &Height, &nrChannels, 0);

    if (!data) {
        Debug::Alert("Faild to load texture - " + (string)Path);
        return true;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return true;
}

bool Texture::LoadCubemap(const char* Path) {

    glGenTextures(1, &TextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureId);

    if (!FileManager::FileExists((FileManager::GetGamePath() + (string)Path).c_str())) {
        
        Debug::Alert("Cubemap doen't exist - " + (string)Path);
        return false;
    }

    ifstream File;

    if (!FileManager::GetFile(Path, File)) {

        Debug::Alert("Faild to load Cubemap - " + (string)Path);
        return false;
    }
    
    vector<string> Faces;
    string Line;
    while (getline(File, Line))
        Faces.push_back(Line);

    for (unsigned int Index = 0; Index < Faces.size(); Index++) {
        
        unsigned char *data = stbi_load((FileManager::GetGamePath() + Faces[Index]).c_str(), &Width, &Height, &nrChannels, 0);

        if (data)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Index, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else
            Debug::Alert(" Faild to Load Cubemap Face - " + Faces[Index]);

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return true;
} 

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, TextureId);
}

void Texture::UnBind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}