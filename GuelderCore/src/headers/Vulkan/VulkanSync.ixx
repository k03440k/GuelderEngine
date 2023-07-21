module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:VulkanSync;

import :IVulkanObject;
import GuelderEngine.Core.Types;

export namespace GuelderEngine::Vulkan
{
    class VulkanSync : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(VulkanSync);

        VulkanSync(const vk::Device& device);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        static vk::Semaphore MakeSemaphore(const vk::Device& device);
        static vk::Fence MakeFence(const vk::Device& device);
    private:
        friend class VulkanSwapchain;
        friend class VulkanPipeline;

        vk::Fence m_InFlightFence;
        vk::Semaphore m_ImageAvailable;
        vk::Semaphore m_RenderFinished;
    };
}