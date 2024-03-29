module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:SwapchainFrameSync;

import :IVulkanObject;
import GuelderEngine.Core.Types;

export namespace GuelderEngine::Vulkan
{
    class SwapchainFrameSync : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(SwapchainFrameSync);

        SwapchainFrameSync(const vk::Device& device);

        virtual void Reset() noexcept override;
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