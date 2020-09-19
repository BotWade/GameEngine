#ifndef _SKYBOX_
#define _SKYBOX_

#include "Component.hpp"
#include "../Mesh/Mesh.hpp"
#include "../Graphics/Vulkan/VulkanPipeLine.hpp"
#include "../Graphics/Vulkan/Vulkan.hpp"
#include "../Graphics/Vulkan/VulkanDescriptor.hpp"

class Skybox : public Component {
public:
    Mesh* mesh;
    Shader* shader;
    VulkanPipeLine* pipeLine;
    VulkanCommandBuffer* commandBuffer;
    VulkanDescriptor uniformData;
    
    Matrix4 model;

    void LoadSkybox(const char* Skybox, const char* Mesh, const char* Shader);
    void PreRender() override;
    void Update() override;
    void PosRender() override;
    void Clean() override;
};

#endif