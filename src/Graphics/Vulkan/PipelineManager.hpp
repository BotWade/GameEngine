#ifndef _PIPELINEMANAGER_
#define _PIPELINEMANAGER_

#include <vector>
#include <string>

using namespace std;

class VulkanPipeLine;

class PipelineManager {    
public:
    static vector<VulkanPipeLine*> Pipelines;

    static void AddPipeline(VulkanPipeLine* Pipeline);
    static VulkanPipeLine* GetPipeline(string Name);
    static void CleanPipelines();
    static void RecreatePipeline();
};

#endif