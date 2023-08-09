module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
module GuelderEngine.Vulkan;
import :VulkanSync;

import GuelderEngine.Core;
import GuelderEngine.Core.Types;

namespace GuelderEngine::Vulkan
{
    VulkanSwapchainFrameSync::VulkanSwapchainFrameSync(const VulkanSwapchainFrameSync& other)
    {
        m_InFlightFence = other.m_InFlightFence;
        m_ImageAvailable = other.m_ImageAvailable;
        m_RenderFinished = other.m_RenderFinished;
    }
    VulkanSwapchainFrameSync::VulkanSwapchainFrameSync(VulkanSwapchainFrameSync&& other) noexcept
    {
        m_InFlightFence = other.m_InFlightFence;
        m_ImageAvailable = other.m_ImageAvailable;
        m_RenderFinished = other.m_RenderFinished;

        other.Reset();
    }
    VulkanSwapchainFrameSync& VulkanSwapchainFrameSync::operator=(const VulkanSwapchainFrameSync& other)
    {
        if(this == &other)
            return *this;

        m_InFlightFence = other.m_InFlightFence;
        m_ImageAvailable = other.m_ImageAvailable;
        m_RenderFinished = other.m_RenderFinished;

        return *this;
    }
    VulkanSwapchainFrameSync& VulkanSwapchainFrameSync::operator=(VulkanSwapchainFrameSync&& other) noexcept
    {
        m_InFlightFence = other.m_InFlightFence;
        m_ImageAvailable = other.m_ImageAvailable;
        m_RenderFinished = other.m_RenderFinished;

        other.Reset();

        return *this;
    }
    VulkanSwapchainFrameSync::VulkanSwapchainFrameSync(const vk::Device& device)
    {
        m_InFlightFence = MakeFence(device);
        m_ImageAvailable = MakeSemaphore(device);
        m_RenderFinished = MakeSemaphore(device);
    }
    void VulkanSwapchainFrameSync::Reset() noexcept
    {
        m_InFlightFence = nullptr;
        m_ImageAvailable = nullptr;
        m_RenderFinished = nullptr;
    }
    void VulkanSwapchainFrameSync::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyFence(m_InFlightFence);
        device.destroySemaphore(m_ImageAvailable);
        device.destroySemaphore(m_RenderFinished);
    }
    vk::Semaphore VulkanSwapchainFrameSync::MakeSemaphore(const vk::Device& device)
    {
        const vk::SemaphoreCreateInfo semaphoreInfo{vk::SemaphoreCreateFlags()};

        return device.createSemaphore(semaphoreInfo);
    }
    vk::Fence VulkanSwapchainFrameSync::MakeFence(const vk::Device& device)
    {
        const vk::FenceCreateInfo fenceInfo(vk::FenceCreateFlags() | vk::FenceCreateFlagBits::eSignaled);

        return device.createFence(fenceInfo);
    }

}