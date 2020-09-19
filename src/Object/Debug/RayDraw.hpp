#ifndef _RAYDRAW_
#define _RAYDRAW_

#include "../Component.hpp"
#include "../../Math/Ray.hpp"
#include "../../Math/Matrix4.hpp"

class Shader;
class Mesh;
class VulkanDescriptor;
class VulkanPipeLine;
class VulkanCommandBuffer;

class RayDraw : public Component {
public:
    Ray ray;
    Shader* shader;
    Mesh* mesh;
    VulkanDescriptor* uniformData;
    VulkanPipeLine* pipeLine;
    VulkanCommandBuffer* commandBuffer;
    Matrix4 model;

    virtual void Load() override;
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void Clean() override;
};

#endif