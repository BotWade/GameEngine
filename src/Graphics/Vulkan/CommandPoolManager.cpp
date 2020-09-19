#include "CommandPoolManager.hpp"
#include "VulkanCommandPool.hpp"

vector<CommnadPoolData*> CommandPoolManager::CommandPools;

VulkanCommandPool* CommandPoolManager::GetCommandPool() {
    for (size_t Index = 0; Index < CommandPools.size(); Index++) {
        if (!CommandPools[Index]->InUse) {
            CommandPools[Index]->InUse = true;
            return CommandPools[Index]->CommandPool;
        }
    }
    
    VulkanCommandPool* commandPool = new VulkanCommandPool();
    commandPool->CreatePool();
    commandPool->UniqueID = CommandPools.size();
    CommnadPoolData* Data = new CommnadPoolData();
    Data->CommandPool = commandPool;
    Data->InUse = true;
    CommandPools.push_back(Data);
    return commandPool;
}

void CommandPoolManager::FreeCommandPool(VulkanCommandPool* commandPool) {
    CommandPools[commandPool->UniqueID]->InUse = false;
}

void CommandPoolManager::Clean() {
    for (size_t Index = 0; Index < CommandPools.size(); Index++)
        CommandPools[Index]->CommandPool->Clean();
}