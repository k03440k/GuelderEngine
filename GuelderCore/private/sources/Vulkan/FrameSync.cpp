module;
#include <compare>
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :FrameSync;

import GuelderEngine.Utils;
import GuelderEngine.Core.Types;

namespace GuelderEngine::Vulkan
{
    FrameSync::FrameSync(const vk::Device& device)
    {
        m_InFlightFence = MakeFence(device);
        m_ImageAvailable = MakeSemaphore(device);
        m_RenderFinished = MakeSemaphore(device);
    }
    FrameSync::FrameSync(FrameSync&& other) noexcept
    {
        m_InFlightFence = other.m_InFlightFence;
        m_ImageAvailable = other.m_ImageAvailable;
        m_RenderFinished = other.m_RenderFinished;

        other.Reset();
    }
    FrameSync& FrameSync::operator=(FrameSync&& other) noexcept
    {
        m_InFlightFence = other.m_InFlightFence;
        m_ImageAvailable = other.m_ImageAvailable;
        m_RenderFinished = other.m_RenderFinished;

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void FrameSync::Reset() noexcept
    {
        m_InFlightFence = nullptr;
        m_ImageAvailable = nullptr;
        m_RenderFinished = nullptr;
    }
    void FrameSync::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyFence(m_InFlightFence);
        device.destroySemaphore(m_ImageAvailable);
        device.destroySemaphore(m_RenderFinished);
    }

    vk::Semaphore FrameSync::MakeSemaphore(const vk::Device& device)
    {
        const vk::SemaphoreCreateInfo semaphoreInfo{ vk::SemaphoreCreateFlags() };

        return device.createSemaphore(semaphoreInfo);
    }
    vk::Fence FrameSync::MakeFence(const vk::Device& device)
    {
        const vk::FenceCreateInfo fenceInfo(vk::FenceCreateFlags() | vk::FenceCreateFlagBits::eSignaled);

        return device.createFence(fenceInfo);
    }
}
namespace GuelderEngine::Vulkan
{
    const vk::Fence& FrameSync::GetFlightFence() const noexcept { return m_InFlightFence; }
    const vk::Semaphore& FrameSync::GetImageAvailableSemaphore() const noexcept { return m_ImageAvailable; }
    const vk::Semaphore& FrameSync::GetImageRenderFinishedSemaphore() const noexcept { return m_RenderFinished; }
}