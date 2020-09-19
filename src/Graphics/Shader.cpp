#include "Shader.hpp"
#include "../Core/FileManager.hpp"
#include "../Math/Math.hpp"
#include "Vulkan/VulkanDeviceManager.hpp"
#include "Vulkan/Vulkan.hpp"

void Shader::ReadFromFile(const char* ShaderFile) {
    
    ifstream File;

    if (!FileManager::GetFile(ShaderFile, File)) {
        Debug::Alert("Shader File - " + (string)ShaderFile + " Not Found");
        return;
    }

    FilePath = ShaderFile;

    string VertexShader = "";
    string FragmentShader = "";

    ReadingMode readingMode = VERTEX; 
    bool ReadingChange = false;
    
    string Line;
    while (getline(File, Line)) {

        if (Line[0] == '#') {
            if (Line == "#VertexShader") {
                readingMode = VERTEX;
                ReadingChange = true;
            }
            else if (Line == "#FragmentShader") {
                readingMode = FRAGMENT;
                ReadingChange = true;
            }
            else if (Line == "#Attributes") {
                readingMode = ATTRIBUTES;
                ReadingChange = true;
            }
            else if (Line == "#Uniforms") {
                readingMode = UNIFORMS;
                ReadingChange = true;
            }
        }

        if (ReadingChange)
            ReadingChange = false;
        else {
            switch (readingMode) {
                case VERTEX:
                    VertexShader += FileManager::GetGamePath() + Line;
                    break;
                case FRAGMENT:
                    FragmentShader += FileManager::GetGamePath() + Line;
                    break;        
                case ATTRIBUTES:
                    if (is_number(Line))
                        Attributes.push_back(stoi(Line));
                    break;
                case UNIFORMS:
                    if (Line != "")
                        Uniforms.push_back(new Uniform(Line, Uniforms.size()));
                    break;
            }            
        }
    }

    vector<char> VertexData;
    vector<char> FragmentData;

    if (!FileManager::GetFileBinary(VertexShader, VertexData)) {
        Debug::Alert("Failed To Load Shader - " + VertexShader);
        return;
    }
    if (!FileManager::GetFileBinary(FragmentShader, FragmentData)) {
        Debug::Alert("Failed To LOad Shader - "  + FragmentShader);
        return;
    }

    vertShaderModule = CreateShaderModule(VertexData);
    fragShaderModule = CreateShaderModule(FragmentData);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    shaderStages.push_back(vertShaderStageInfo);
    shaderStages.push_back(fragShaderStageInfo);
}

VkShaderModule Shader::CreateShaderModule(vector<char> &code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(VulkanDeviceManager::GetSelectedDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        Debug::Error("Failed To Create Shader Module For Shader - " + (string)FilePath);

    return shaderModule;
}

Uniform* Shader::GetUniform(string Name) {

    for (Uniform* uniform : Uniforms)
        if (uniform->Name == Name)
            return uniform;

    return nullptr;
}

void Shader::Clean() {

    vkDestroyShaderModule(VulkanDeviceManager::GetSelectedDevice(), fragShaderModule, nullptr);
    vkDestroyShaderModule(VulkanDeviceManager::GetSelectedDevice(), vertShaderModule, nullptr);
    
}