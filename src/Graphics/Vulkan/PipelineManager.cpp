#include "PipelineManager.hpp"
#include "VulkanPipeLine.hpp"

vector<VulkanPipeLine*> PipelineManager::Pipelines;

void PipelineManager::AddPipeline(VulkanPipeLine* Pipeline) {

    Pipelines.push_back(Pipeline);
}

VulkanPipeLine* PipelineManager::GetPipeline(string Name) {
    for (VulkanPipeLine* pipeline : Pipelines)
        if (pipeline->Name == Name)
            return pipeline;

    return nullptr;
}

void PipelineManager::CleanPipelines() {
    for (VulkanPipeLine* pipeline : Pipelines)
        pipeline->Clean();
}

void PipelineManager::RecreatePipeline() {

    for (VulkanPipeLine* pipeline : Pipelines) {
        pipeline->CreatePipelineLayout();
        pipeline->CreatePipeline(pipeline->currentShader, pipeline->currentRenderPass);
    }
}