#ifndef _RENDERER_
#define _RENDERER_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "Shader.hpp"
#include "../Core/Window.hpp"
#include "../Math/Ray.hpp"
#include "../Math/Triangle.hpp"
#include "../Mesh/Mesh.hpp"

struct TextUniform {
    Matrix4 Projection;
    Vector3 Color;
};

class VulkanCommandBuffer;
class VulkanCommandPool;

struct SingleTimeCommandData {
    VkCommandBuffer commandBuffer;
    VulkanCommandPool* commandPool;
};

class Renderer {
public:
    static VulkanCommandBuffer* ClearCommandBuffer;
    static atomic_flag QueueInUse;
    
    static void Clear();
    static void Prepare();
    static void RendererClear();
    static SingleTimeCommandData BeginSingleTimeCommands();
    static void EndSingleTimeCommands(SingleTimeCommandData commandBuffer);
    static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags);
    static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    static void SetScissor(Vector4 Region);
    static void SetScissorDefault();
    static void SetDepthMask(bool Active);
};

#endif