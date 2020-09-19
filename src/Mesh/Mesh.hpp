#ifndef _MESH_
#define _MESH_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "../Graphics/Vertex.hpp"

using namespace std;

class Shader;

enum Primitive {
    QUAD
};

class Mesh {
public:

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    const char* Path;

    bool AttributesProcessed = false;
    
    vector<Vertex> Data;
    vector<uint32_t> Indices;
    size_t IndexSize;
    vector<VkVertexInputAttributeDescription> attributesDescription;

    vector<unsigned int> CustomBuffers;

    Mesh();

    bool LoadMesh(const char* FilePath);
    bool LoadPrimitive(Primitive primitive);
    void ProcessAttributes(int Type, unsigned long TypeSize, Shader* shader);
    void AlocateData(size_t DataSize);
    void UpdateData(void* data, size_t Size);
    void UpdateIndex(void* data, size_t Size, size_t Count);
    unsigned int GenerateCustomBuffer(GLenum Target = GL_ARRAY_BUFFER);
    void UpdateCustomBuffer(void* Data, size_t Size, int BufferIndex);
    void Bind();
    void Draw();
    void Clean();
};

#endif