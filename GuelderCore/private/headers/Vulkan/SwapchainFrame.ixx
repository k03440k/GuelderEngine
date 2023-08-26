module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:SwapchainFrame;

import :IVulkanObject;
import :SwapchainFrameSync;

export namespace GuelderEngine::Vulkan
{
    class CommandPool;

    struct SwapchainFrame : public IVulkanObject, INHERIT_GClass(SwapchainFrame)
    {
    public:
        DECLARE_DCAD_AND_CAM(SwapchainFrame);
        
        SwapchainFrame(const vk::Device& device, const vk::ImageViewCreateInfo& viewInfo, const vk::CommandPool& pool);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device, const vk::CommandPool& pool) const noexcept;

        void CreateFrameBuffer(const vk::Device& device, const vk::FramebufferCreateInfo& info);

        void Recreate(const vk::Device& device, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent,
            const vk::ImageViewCreateInfo& viewInfo, const vk::CommandPool& pool);

        /*
         *@brief waits for fence
        */
        void WaitForImage(const vk::Device& device, const uint64_t& delay = UINT64_MAX) const;
        void ResetFence(const vk::Device& device) const;

        vk::Image image;
        vk::ImageView imageView;
        vk::Framebuffer framebuffer;
        vk::CommandBuffer commandBuffer;
        SwapchainFrameSync sync;

    private:
        void CleanupImageView(const vk::Device& device) const noexcept;
        void CleanupFramebuffer(const vk::Device& device) const noexcept;
        void FreeCommandBuffer(const vk::CommandPool& pool, const vk::Device& device) const noexcept;

        void CreateFrameBuffer(const vk::Device& device, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent);
        void CreateImage(const vk::Device& device, const vk::ImageViewCreateInfo& viewInfo);
        void CreateCommandBuffer(const vk::Device& device, const vk::CommandPool& pool);
    };
}