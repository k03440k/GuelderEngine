module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :VulkanQueueFamilyIndices;

import :IVulkanObject;

import <optional>;

namespace GuelderEngine::Vulkan
{
    VulkanQueueFamilyIndices::VulkanQueueFamilyIndices(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
    {
        const std::vector queueFamilies = device.getQueueFamilyProperties();

#ifdef GE_DEBUG_VULKAN
        GE_LOG(VulkanCore, Info, "Device can support ", queueFamilies.size(), " Queue Families");
#endif

        for(size_t i = 0; i < queueFamilies.size(); i++)
        {
            if(queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics && device.getSurfaceSupportKHR(i, surface))
            {
                graphicsFamily = i;
                presentFamily = i;

#ifdef GE_DEBUG_VULKAN
                GE_LOG(VulkanCore, Info, "Queue Family at index ", i, " is suitable for graphics and presenting");
#endif
            }
            if(IsComplete())
                break;
        }

        GE_CORE_CLASS_ASSERT(IsComplete(), "Cannot complete Queue Family(device doesn't support requirements)");
    }
    VulkanQueueFamilyIndices::VulkanQueueFamilyIndices(const VulkanQueueFamilyIndices& other)
    {
        graphicsFamily = other.graphicsFamily;
        presentFamily = other.presentFamily;
    }
    VulkanQueueFamilyIndices& VulkanQueueFamilyIndices::operator=(const VulkanQueueFamilyIndices& other)
    {
        if(this == &other)
            return *this;

        graphicsFamily = other.graphicsFamily;
        presentFamily = other.presentFamily;

        return *this;
    }
    bool VulkanQueueFamilyIndices::IsComplete() const noexcept
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
}