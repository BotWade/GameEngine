#include "Mesh.hpp"
#include "../Graphics/Vulkan/VulkanDeviceManager.hpp"
#include "../Graphics/Vulkan/Vulkan.hpp"
#include "../Graphics/Renderer.hpp"
#include "ObjLoader/Loader.hpp"
#include "../Graphics/Shader.hpp"

Mesh::Mesh() {
    vertexBuffer = nullptr;
    vertexBufferMemory = nullptr;
    indexBuffer = nullptr;
    indexBufferMemory = nullptr;
}

bool Mesh::LoadMesh(const char* FilePath) {

    if (!FileManager::FileExists((FileManager::GetGamePath() + FilePath).c_str())) {
        Debug::Alert("Faild To Mesh Load File - " + (string)FilePath);
        return false;
    }

    Loader loader;
    loader.LoadFile(FilePath);
    
    Path = FilePath;

    Data = loader.LoadedMeshes[0].Vertices;
    Indices = loader.LoadedIndices;

    UpdateData(Data.data(), sizeof(Data[0]) * Data.size());
    UpdateIndex(Indices.data(), sizeof(Indices[0]) * Indices.size(), Indices.size());

    return true;
}

bool Mesh::LoadPrimitive(Primitive primitive) {

    switch (primitive) {
        case QUAD:
            Data = {
                { Vector3(-1, 1, 0), Vector3(), Vector2(0, 1) },
                { Vector3( 1, 1, 0), Vector3(), Vector2(1, 1) },
                { Vector3( 1,-1, 0), Vector3(), Vector2(1, 0) },
                { Vector3(-1,-1, 0), Vector3(), Vector2(0, 0) }
            };
            Indices = {
                0, 1, 3,
                1, 2, 3
            };
            break;
    }

    UpdateData(Data.data(), sizeof(Data[0]) * Data.size());
    UpdateIndex(Indices.data(), sizeof(Indices[0]) * Indices.size(), Indices.size());
}

unsigned int Mesh::GenerateCustomBuffer(GLenum Target) {
    unsigned int customBuffer;
    
    CustomBuffers.push_back(customBuffer);

    return customBuffer;
}

void Mesh::ProcessAttributes(int Type, unsigned long TypeSize, Shader* shader) {
    int Stride = 0;

    for (auto& n : shader->Attributes)
        Stride += n;

    Stride *= TypeSize;
    
    int Offset = 0;
    int Size = shader->Attributes.size();
    
    for (int Index = 0; Index < Size; Index++) {
        attributesDescription.push_back(VkVertexInputAttributeDescription{ 0, Index, static_cast<VkFormat>(Type + shader->Attributes[Index] * 3 - 3), Offset });
        Offset += shader->Attributes[Index];
    }

    AttributesProcessed = true;
}

void Mesh::AlocateData(size_t DataSize) {
    Renderer::CreateBuffer(DataSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, vertexBuffer, vertexBufferMemory);
}

void Mesh::UpdateData(void* data, size_t Size) {
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    
    Renderer::CreateBuffer(Size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* vkData;
    vkMapMemory(VulkanDeviceManager::GetSelectedDevice(), stagingBufferMemory, 0, Size, 0, &vkData);
    mempcpy(vkData, data, Size);
    vkUnmapMemory(VulkanDeviceManager::GetSelectedDevice(), stagingBufferMemory);

    if (vertexBuffer == nullptr)
        AlocateData(Size);

    Renderer::CopyBuffer(stagingBuffer, vertexBuffer, Size);

    vkDestroyBuffer(VulkanDeviceManager::GetSelectedDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanDeviceManager::GetSelectedDevice(), stagingBufferMemory, nullptr);
}

void Mesh::UpdateIndex(void* data, size_t Size, size_t Count) {

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    Renderer::CreateBuffer(Size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* vkData;
    vkMapMemory(VulkanDeviceManager::GetSelectedDevice(), stagingBufferMemory, 0, Size, 0, &vkData);
    memcpy(vkData, data, (size_t) Size);
    vkUnmapMemory(VulkanDeviceManager::GetSelectedDevice(), stagingBufferMemory);

    Renderer::CreateBuffer(Size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    Renderer::CopyBuffer(stagingBuffer, indexBuffer, Size);

    vkDestroyBuffer(VulkanDeviceManager::GetSelectedDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanDeviceManager::GetSelectedDevice(), stagingBufferMemory, nullptr);

    IndexSize = Count;
}

void Mesh::UpdateCustomBuffer(void* Data, size_t Size, int BufferIndex) {
    
}

void Mesh::Bind() {
    VkBuffer vertexBuffers[] = { vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(Vulkan::GetCurrentCommandBuffer(), 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(Vulkan::GetCurrentCommandBuffer(), indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void Mesh::Draw() {
    vkCmdDrawIndexed(Vulkan::GetCurrentCommandBuffer(), IndexSize, 1, 0, 0, 0);
}

void Mesh::Clean() {
    vkDestroyBuffer(VulkanDeviceManager::GetSelectedDevice(), vertexBuffer, nullptr);
    vkFreeMemory(VulkanDeviceManager::GetSelectedDevice(), vertexBufferMemory, nullptr);
    vkDestroyBuffer(VulkanDeviceManager::GetSelectedDevice(), indexBuffer, nullptr);
    vkFreeMemory(VulkanDeviceManager::GetSelectedDevice(), indexBufferMemory, nullptr);
    Data.clear();
    Data.shrink_to_fit();
    Indices.clear();
    Indices.shrink_to_fit();
}