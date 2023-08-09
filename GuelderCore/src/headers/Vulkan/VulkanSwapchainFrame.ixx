module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanSwapchainFrame;

import :IVulkanObject;
import :VulkanSync;

export namespace GuelderEngine::Vulkan
{
    struct VulkanSwapchainFrame : INHERIT_GClass(VulkanSwapchainFrame), public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(VulkanSwapchainFrame);

        VulkanSwapchainFrame(const vk::Device& device, const vk::Image& image, const vk::ImageViewCreateInfo& viewInfo);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        void CreateFrameBuffer(const vk::Device& device, const vk::FramebufferCreateInfo& info);

        /*
         *@brief waits for fence
        */
        void WaitForImage(const vk::Device& device, const uint64_t& delay = UINT64_MAX) const;
        void ResetFence(const vk::Device& device) const;

        vk::Image image;
        vk::ImageView imageView;
        vk::Framebuffer framebuffer;
        vk::CommandBuffer commandBuffer;
        VulkanSwapchainFrameSync sync;

    private:
        void CreateImage(const vk::Device& device, const vk::Image& image, const vk::ImageViewCreateInfo& viewInfo);
    };
}