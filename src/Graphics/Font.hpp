#ifndef _CHARACTER_
#define _CHARACTER_

#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <utility>
#include <memory>

#include "../Math/Vector2.hpp"
#include "Vulkan/CommandBufferManager.hpp"
#include "Vulkan/VulkanPipeLine.hpp"
#include "../Mesh/Mesh.hpp"
#include "Texture.hpp"

using namespace std;

struct Character {
    /**
     * Mesh Used By Character
     * Data Order
     * 0---1.
     * | / |
     * 3---2
     */
    Mesh* quad;
    VulkanCommandBuffer* commandBuffer;    
    Texture* texture;
    Vector2 Size;
    Vector2 Bearing;
    unsigned int Advance;
};

struct Font {
    map<char, Character> Characters;
    FT_Face FontFace;
    bool FaildToLoadFont;
    Shader* shader;
    VulkanPipeLine* pipeLine;
    string Name;

    void LoadFont(const char* Path);
    void DrawLetter(char C);
    void Clean();
};

#endif