module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanSwapchainFrame;

import :IVulkanObject;
import :VulkanSync;

export namespace GuelderEngine::Vulkan
{
    struct VulkanSwapchainFrame : public IVulkanObject
    {
        DECLARE_DCAD_AND_CAM(VulkanSwapchainFrame);

        virtual void Reset() noexcept override;

        vk::Image image;
        vk::ImageView imageView;
        vk::Framebuffer framebuffer;
        vk::CommandBuffer commandBuffer;
        VulkanSync sync;
    };
}