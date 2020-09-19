#include "Uniform.hpp"
#include "Vulkan/VulkanDeviceManager.hpp"
#include "Vulkan/Vulkan.hpp"
#include "../Core/Debug.hpp"
#include "Renderer.hpp"

#include <vector>

Uniform::Uniform(string name, uint32_t Index) {
    
    if (name.find(':') != string::npos) {
        vector<string> Values = Explode(name, ':');
        if (Values.size() < 2)
            Debug::Alert("Uniform Name Invalid - " + name);
        else if (Values[1] == "BUFFER")
            type = UNIFORM_BUFFER;
        else
            type = UNIFORM_TEXTURE;

        Name = Values[0];
    }
    else {
        type = UNIFORM_BUFFER;
        Name = name;
    }
    LayoutBinding.binding = Index;
    LayoutBinding.descriptorCount = 1;
    LayoutBinding.pImmutableSamplers = nullptr;

    switch (type) {
        case UNIFORM_BUFFER:
            LayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            LayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        case UNIFORM_TEXTURE:
            LayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            LayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
    }
}