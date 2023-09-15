module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :QueueFamilyIndices;

import :IVulkanObject;

import <optional>;

namespace GuelderEngine::Vulkan
{
    QueueFamilyIndices::QueueFamilyIndices(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
    {
        const std::vector queueFamilies = device.getQueueFamilyProperties();
        
        GE_LOG(VulkanCore, Info, "Device can support ", queueFamilies.size(), " Queue Families");

        for(size_t i = 0; i < queueFamilies.size(); i++)
        {
            if(queueFamilies[i].queueFlags & vk::QueueFlagBits::eTransfer)
            {
                transferFamily = i;

                GE_LOG(VulkanCore, Info, "Queue Family at index ", i, " is suitable for transfering memory");
            }
            if(queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
            {
                graphicsFamily = i;
                
                GE_LOG(VulkanCore, Info, "Queue Family at index ", i, " is suitable for graphics");
            }
            if(device.getSurfaceSupportKHR(i, surface))
            {
                presentFamily = i;
                
                GE_LOG(VulkanCore, Info, "Queue Family at index ", i, " is suitable for presenting");
            }
            if(IsComplete())
                break;
        }

        GE_CLASS_ASSERT(IsComplete(), "Cannot complete Queue Family(device doesn't support requirements)");
    }
    QueueFamilyIndices::QueueFamilyIndices(const QueueFamilyIndices& other)
    {
        graphicsFamily = other.graphicsFamily;
        presentFamily = other.presentFamily;
        transferFamily = other.transferFamily;
    }
    QueueFamilyIndices& QueueFamilyIndices::operator=(const QueueFamilyIndices& other)
    {
        if(this == &other)
            return *this;

        graphicsFamily = other.graphicsFamily;
        presentFamily = other.presentFamily;
        transferFamily = other.transferFamily;

        return *this;
    }
    bool QueueFamilyIndices::IsComplete() const noexcept
    {
        return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
    }
    Types::uint QueueFamilyIndices::GetGraphicsFamily() const noexcept
    {
        return graphicsFamily.value();
    }
    Types::uint QueueFamilyIndices::GetPresentFamily() const noexcept
    {
        return presentFamily.value();
    }
    Types::uint QueueFamilyIndices::GetTransferFamily() const noexcept
    {
        return transferFamily.value();
    }
}