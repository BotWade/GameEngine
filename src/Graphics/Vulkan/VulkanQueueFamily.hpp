#ifndef _VULKANQUEUEFAMILY_
#define _VULKANQUEUEFAMILY_

#include <stdint.h>
#include <optional>

using namespace std;

struct QueueFamilyIndices {
    optional<uint32_t> graphicsFamily;
    optional<uint32_t> presentFamily;
    
    inline bool IsComplete();
};

inline bool QueueFamilyIndices::IsComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
}

#endif