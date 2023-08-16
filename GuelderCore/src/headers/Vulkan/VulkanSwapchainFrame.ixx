module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanSwapchainFrame;

import :IVulkanObject;
import :VulkanSync;

export namespace GuelderEngine::Vulkan
{
    class VulkanCommandPool;

    struct VulkanSwapchainFrame : public IVulkanObject, INHERIT_GClass(VulkanSwapchainFrame)
    {
    public:
        DECLARE_DCAD_AND_CAM(VulkanSwapchainFrame);
        
        VulkanSwapchainFrame(const vk::Device& device, const vk::ImageViewCreateInfo& viewInfo, const VulkanCommandPool& pool);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Device& device, const VulkanCommandPool& pool) const noexcept;

        void CreateFrameBuffer(const vk::Device& device, const vk::FramebufferCreateInfo& info);

        void Recreate(const vk::Device& device, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent,
            const vk::ImageViewCreateInfo& viewInfo, const VulkanCommandPool& pool);

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
        void CleanupImageView(const vk::Device& device) const noexcept;
        void CleanupFramebuffer(const vk::Device& device) const noexcept;
        void FreeCommandBuffer(const VulkanCommandPool& pool, const vk::Device& device) const noexcept;

        void CreateFrameBuffer(const vk::Device& device, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent);
        void CreateImage(const vk::Device& device, const vk::ImageViewCreateInfo& viewInfo);
        void CreateCommandBuffer(const vk::Device& device, const VulkanCommandPool& pool);
    };
}