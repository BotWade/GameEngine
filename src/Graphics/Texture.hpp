#ifndef _TEXTURE_
#define _TEXTURE_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

#include "../Math/Vector2.hpp"
#include "../Math/Matrix4.hpp"

using namespace std;

enum TextureType {
    /**
    * Load A Single Texture 
    */
    TEXTURE2D,
    /**
    * LOWPRIORITY TODO: IMPLEMENT CUBEMAP
    * Load 6 Different Textures Into A Cubemap (Not Implemented)
    */
    CUBEMAP,
    /**
    * Load A HDR image Texture
    */
    HDR
};

class Texture {
public:
    ///Two Variables To Make It Thread Safe
    static bool HDRLoaderLoaded;
    static bool HDRLoaderLoading;
    static vector<Matrix4> captureViews;

    int Width;
    int Height;
    int Channels;
    VkFormat format;
    string filePath;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;
    
    VkImage hdrImage;
    VkDeviceMemory hdrImageMemory;
    VkImageView hdrImageView;
    VkSampler hdrSampler;

    TextureType textureType;

    void LoadTexture(string FilePath, TextureType type = TEXTURE2D);
    void LoadTexture(unsigned char* Buffer, Vector2 Size, int Channels);
    void LoadHDR();
    static void LoadHDRLoaders(Vector2 Size);
    void TransitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VkBuffer buffer, uint32_t targetLayer = 0);
    void CopyImageToImage(VkImage srcImage, VkImageLayout srcLayout, VkImage dstImage, VkImageLayout dstLayout, uint32_t dstLayer = 0, uint32_t srcLayer = 0);
    void CreateTextureImageView(TextureType type = TEXTURE2D);
    void CreateTextureSampler(TextureType type = TEXTURE2D);
    void Clear();
    static void CleanHDRLoader();
};

#endif