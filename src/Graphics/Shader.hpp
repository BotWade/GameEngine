#ifndef _SHADER_
#define _SHADER_

#include "Uniform.hpp"
#include "Vulkan/VulkanPipeLine.hpp"
#include "Vulkan/VulkanDescriptorPool.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

enum ReadingMode {
    VERTEX,
    FRAGMENT,
    ATTRIBUTES,
    UNIFORMS
};

class Shader {
public:
    const char* FilePath;
    vector<int> Attributes;
    vector<Uniform*> Uniforms;
    vector<VkPipelineShaderStageCreateInfo> shaderStages;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    
    void ReadFromFile(const char* ShaderFile);
    void Clean();
    Uniform* GetUniform(string Name);
    VkShaderModule CreateShaderModule(vector<char> &code);
};

#endif