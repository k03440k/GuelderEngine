module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:FrameContext;

import :IVulkanObject;
import :FrameSync;

import <limits>;

export namespace GuelderEngine::Vulkan
{
    struct FrameContext : public IVulkanObject
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(FrameContext);
        DECLARE_DEFAULT_COPYING(FrameContext);
        DECLARE_MOVING(FrameContext);

        FrameContext(
            const vk::Device& device,
            const vk::CommandPool& commandPool,
            const vk::RenderPass& renderPass,
            const std::vector<vk::ImageView>& attachments,
            const vk::Extent2D& extent
        );

        void Reset() noexcept override;
        virtual void Cleanup(const vk::Device& device, const vk::CommandPool& pool) const noexcept;

        void Recreate(
            const vk::Device& device,
            const vk::RenderPass& renderPass,
            const std::vector<vk::ImageView>& attachments,
            const vk::CommandPool& commandPool,
            const vk::Extent2D& swapchainExtent
        );

        void FreeCommandBuffer(const vk::CommandPool& commandPool, const vk::Device& device) const noexcept;

        vk::Framebuffer frameBuffer;
        vk::CommandBuffer commandBuffer;

    protected:
        static vk::Framebuffer CreateFrameBuffer(const vk::Device& device, const std::vector<vk::ImageView>& attachments, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent);
        static vk::CommandBuffer CreateCommandBuffer(const vk::Device& device, const vk::CommandPool& pool);
    };
}