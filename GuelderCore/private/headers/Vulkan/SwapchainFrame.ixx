module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:SwapchainFrame;

import :IVulkanObject;
import :SwapchainFrameSync;

import <limits>;

export namespace GuelderEngine::Vulkan
{
    struct SwapchainFrame : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(SwapchainFrame);
        
        SwapchainFrame(const vk::Device& device, const vk::ImageViewCreateInfo& viewInfo, const vk::CommandPool& pool);

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device, const vk::CommandPool& pool) const noexcept;

        void Recreate(const vk::Device& device, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent,
            const vk::ImageViewCreateInfo& viewInfo, const vk::CommandPool& pool);

        /**
         *@brief waits for fence
        */
        void WaitForImage(const vk::Device& device, const uint64_t& delay = std::numeric_limits<uint64_t>::max()) const;
        void ResetFence(const vk::Device& device) const;
        void FreeCommandBuffer(const vk::CommandPool& pool, const vk::Device& device) const noexcept;

        vk::Image image;
        vk::ImageView imageView;
        vk::Framebuffer framebuffer;
        vk::CommandBuffer commandBuffer;
        SwapchainFrameSync sync;

    private:
        void CleanupImageView(const vk::Device& device) const noexcept;
        void CleanupFramebuffer(const vk::Device& device) const noexcept;

        void CreateFrameBuffer(const vk::Device& device, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent);
        void CreateImage(const vk::Device& device, const vk::ImageViewCreateInfo& viewInfo);
        void CreateCommandBuffer(const vk::Device& device, const vk::CommandPool& pool);
    };
}