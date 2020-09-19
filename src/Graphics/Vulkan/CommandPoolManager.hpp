#ifndef _COMMANDPOOLMANAGER_
#define _COMMANDPOOLMANAGER_

class VulkanCommandPool;

using namespace std;

#include <vector>

struct CommnadPoolData {
    VulkanCommandPool* CommandPool;
    bool InUse;
};

/*
* A Thread Safe Command Pool Manager
*/
class CommandPoolManager {
public:
    static vector<CommnadPoolData*> CommandPools;

    /**
    * Get And Use Command Pool, Command Pool Will Be Locked
    */
    static VulkanCommandPool* GetCommandPool();
    /**
    * Free Command Pool
    * @param commandPool The Command Pool You Want To Unlock
    */
    static void FreeCommandPool(VulkanCommandPool* commandPool);
    /**
    * Clean Comamnd Pool
    */
    static void Clean();
};

#endif