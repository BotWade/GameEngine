#include "Vertex.hpp"

vector<VkVertexInputBindingDescription>* Vertex::GetBindingDescription() {
    vector<VkVertexInputBindingDescription>* bindingDescription = new vector<VkVertexInputBindingDescription>(1);
    (*bindingDescription)[0].binding = 0;
    (*bindingDescription)[0].stride = sizeof(Vertex);
    (*bindingDescription)[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
}

vector<VkVertexInputAttributeDescription>* Vertex::GetAttributeDescriptions() {
    vector<VkVertexInputAttributeDescription>* attributeDescriptions = new vector<VkVertexInputAttributeDescription>(3);
    (*attributeDescriptions)[0].binding = 0;
    (*attributeDescriptions)[0].location = 0;
    (*attributeDescriptions)[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    (*attributeDescriptions)[0].offset = offsetof(Vertex, Position);
    (*attributeDescriptions)[1].binding = 0;
    (*attributeDescriptions)[1].location = 1;
    (*attributeDescriptions)[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    (*attributeDescriptions)[1].offset = offsetof(Vertex, Normal);
    (*attributeDescriptions)[2].binding = 0;
    (*attributeDescriptions)[2].location = 2;
    (*attributeDescriptions)[2].format = VK_FORMAT_R32G32_SFLOAT;
    (*attributeDescriptions)[2].offset = offsetof(Vertex, TextureCoordinate);

    return attributeDescriptions;
}