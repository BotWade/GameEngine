#ifndef _TEXTRENDERER_
#define _TEXTRENDERER_

#include "include/stb_font_consolas_24_latin1.inl"

#define STB_FONT_NAME stb_font_consolas_24_latin1
#define STB_FONT_WIDTH STB_FONT_consolas_24_latin1_BITMAP_WIDTH
#define STB_FONT_HEIGHT STB_FONT_consolas_24_latin1_BITMAP_HEIGHT 
#define STB_FIRST_CHAR STB_FONT_consolas_24_latin1_FIRST_CHAR
#define STB_NUM_CHARS STB_FONT_consolas_24_latin1_NUM_CHARS
#define TEXTOVERLAY_MAX_CHAR_COUNT 2048

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "../Math/Vector2.hpp"

using namespace std;

enum TextAlign { 
    AlignLeft, 
    AlignCenter, 
    AlignRight 
};

class Mesh;
class Texture;
class VulkanPipeLine;
class VulkanCommandBuffer;
class VulkanDescriptor;
class Vector4;

struct TextVertex {
    Vector2 Position;
    Vector2 UV;

    static vector<VkVertexInputBindingDescription>* GetBindingDescription();
    static vector<VkVertexInputAttributeDescription>* GetAttributeDescriptions();
};

///MEDIUMPRIORITY TODO: Currently Text Renderer Is Limited To TEXTOVERLAY_MAX_CHAR_COUNT Characters At A Time, We Need To Make It Dynamic (Allow Infinite Number Of Words Or At least Almost Infinite)
///HIGHPRIORITY TODO: Add Text Mask To Hide Text When It Overflows A Especific Area, Needed For Text Input, Buttons And Others

class TextRenderer {
public:
    static stb_fontchar stbFontData[];
    static Mesh* mesh;
    static Texture* texture;
    static VulkanDescriptor* uniformData;
    static VulkanPipeLine* pipeLine;
    static Vector4* VertexData;
    static VulkanCommandBuffer* CommandBuffer;
    static int NumLetters;

    static void Init();
    static void BeginTextUpdate();
    static void AddText(string Text, Vector2 Position, float TextSize, TextAlign Align = AlignLeft);
    static void EndTextUpdate();
    static void UpdateCommandBuffers();
    static void Clean();
};


#endif