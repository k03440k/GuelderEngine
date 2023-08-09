module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanFrameBuffer;

import <vector>;

export namespace GuelderEngine::Vulkan
{
    struct VulkanSwapchainFrame;

    class VulkanFrameBuffer
    {
    public:
        DELETE_COPY_AND_MOVING(VulkanFrameBuffer);

        static void Make(
            const vk::Device& device,
            const vk::RenderPass& renderPass,
            const vk::Extent2D& swapchainExtent,
            std::vector<VulkanSwapchainFrame>& frames
        );
    };
}