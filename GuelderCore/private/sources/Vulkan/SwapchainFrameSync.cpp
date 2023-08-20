module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
module GuelderEngine.Vulkan;
import :SwapchainFrameSync;

import GuelderEngine.Core;
import GuelderEngine.Core.Types;

namespace GuelderEngine::Vulkan
{
    SwapchainFrameSync::SwapchainFrameSync(const SwapchainFrameSync& other)
    {
        m_InFlightFence = other.m_InFlightFence;
        m_ImageAvailable = other.m_ImageAvailable;
        m_RenderFinished = other.m_RenderFinished;
    }
    SwapchainFrameSync::SwapchainFrameSync(SwapchainFrameSync&& other) noexcept
    {
        m_InFlightFence = other.m_InFlightFence;
        m_ImageAvailable = other.m_ImageAvailable;
        m_RenderFinished = other.m_RenderFinished;

        other.Reset();
    }
    SwapchainFrameSync& SwapchainFrameSync::operator=(const SwapchainFrameSync& other)
    {
        if(this == &other)
            return *this;

        m_InFlightFence = other.m_InFlightFence;
        m_ImageAvailable = other.m_ImageAvailable;
        m_RenderFinished = other.m_RenderFinished;

        return *this;
    }
    SwapchainFrameSync& SwapchainFrameSync::operator=(SwapchainFrameSync&& other) noexcept
    {
        m_InFlightFence = other.m_InFlightFence;
        m_ImageAvailable = other.m_ImageAvailable;
        m_RenderFinished = other.m_RenderFinished;

        other.Reset();

        return *this;
    }
    SwapchainFrameSync::SwapchainFrameSync(const vk::Device& device)
    {
        m_InFlightFence = MakeFence(device);
        m_ImageAvailable = MakeSemaphore(device);
        m_RenderFinished = MakeSemaphore(device);
    }
    void SwapchainFrameSync::Reset() noexcept
    {
        m_InFlightFence = nullptr;
        m_ImageAvailable = nullptr;
        m_RenderFinished = nullptr;
    }
    void SwapchainFrameSync::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyFence(m_InFlightFence);
        device.destroySemaphore(m_ImageAvailable);
        device.destroySemaphore(m_RenderFinished);
    }
    vk::Semaphore SwapchainFrameSync::MakeSemaphore(const vk::Device& device)
    {
        const vk::SemaphoreCreateInfo semaphoreInfo{vk::SemaphoreCreateFlags()};

        return device.createSemaphore(semaphoreInfo);
    }
    vk::Fence SwapchainFrameSync::MakeFence(const vk::Device& device)
    {
        const vk::FenceCreateInfo fenceInfo(vk::FenceCreateFlags() | vk::FenceCreateFlagBits::eSignaled);

        return device.createFence(fenceInfo);
    }
	const vk::Fence& SwapchainFrameSync::GetFlightFence() const noexcept { return m_InFlightFence; }
	const vk::Semaphore& SwapchainFrameSync::GetImageAvailableSemaphore() const noexcept { return m_ImageAvailable; }
	const vk::Semaphore& SwapchainFrameSync::GetImageRenderFinishedSemaphore() const noexcept { return m_RenderFinished; }
}