module;
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :VulkanFrameBuffer;

import :VulkanSwapchainFrame;

import <vector>;

namespace GuelderEngine::Vulkan
{
    void VulkanFrameBuffer::Make(const vk::Device& device, const vk::RenderPass& renderPass,
        const vk::Extent2D& swapchainExtent, std::vector<VulkanSwapchainFrame>& frames)
    {
        for(Types::uint i = 0; i < frames.size(); ++i)
        {
            std::vector attachments{frames[i].imageView};//idk

            //auto attachment = frames[i].imageView;

            const vk::FramebufferCreateInfo framebufferInfo(
                vk::FramebufferCreateFlags(),
                renderPass,
                attachments.size(),
                attachments.data(),
                swapchainExtent.width,
                swapchainExtent.height,
                1
            );

            //frames[i].framebuffer = device.createFramebuffer(framebufferInfo);
            frames[i].CreateFrameBuffer(device, framebufferInfo);
        }
    }
}