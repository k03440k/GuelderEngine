module;
#include <compare>
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:FrameSync;

import :IVulkanObject;
import GuelderEngine.Core.Types;

export namespace GuelderEngine::Vulkan
{
    class FrameSync : public IVulkanObject
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(FrameSync);
        DECLARE_DEFAULT_COPYING(FrameSync);
        DECLARE_MOVING(FrameSync);

        FrameSync(const vk::Device& device);

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        static vk::Semaphore MakeSemaphore(const vk::Device& device);
        static vk::Fence MakeFence(const vk::Device& device);

        const vk::Fence& GetFlightFence() const noexcept;
        const vk::Semaphore& GetImageAvailableSemaphore() const noexcept;
        const vk::Semaphore& GetImageRenderFinishedSemaphore() const noexcept;
    private:
        vk::Fence m_InFlightFence;
        vk::Semaphore m_ImageAvailable;
        vk::Semaphore m_RenderFinished;
    };
}