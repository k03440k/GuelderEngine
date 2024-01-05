module;
#include <vulkan/vulkan.hpp>
#include "../../../public/GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :SwapchainDepthImage;

import :IVulkanObject;
import :IBuffer;
import GuelderEngine.Core.Types;

namespace GuelderEngine::Vulkan
{
    SwapchainDepthImage::SwapchainDepthImage(
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const vk::Extent2D& swapchainExtent,
        const vk::Format& depthFormat,
        const vk::SharingMode& sharingMode,
        const uint& queueFamilyIndexCount,
        const uint* queueFamilyIndices
    )
    {
        vk::ImageCreateInfo imageInfo{
            vk::ImageCreateFlags(),
            vk::ImageType::e2D,
            depthFormat,
            {swapchainExtent.width, swapchainExtent.height, 1},
            1,
            1,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eDepthStencilAttachment,
            sharingMode,
            queueFamilyIndexCount,
            queueFamilyIndices,
            vk::ImageLayout::eUndefined
        };
        
        GE_CLASS_ASSERT(device.createImage(&imageInfo, nullptr, &image) == vk::Result::eSuccess, "cannot create depth image");

        vk::MemoryRequirements memoryRequirements;
        device.getImageMemoryRequirements(image, &memoryRequirements);

        const vk::MemoryAllocateInfo allocateInfo{
        memoryRequirements.size,
        Buffers::IBuffer::FindMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal)
        };

        GE_CLASS_ASSERT(device.allocateMemory(&allocateInfo, nullptr, &memory) == vk::Result::eSuccess, "cannot allocate memory for depth image");
        device.bindImageMemory(image, memory, 0);

        const vk::ImageViewCreateInfo imageViewInfo{
            vk::ImageViewCreateFlags(),
            image,
            vk::ImageViewType::e2D,
            depthFormat,
            {},
            {vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1}
        };

        GE_CLASS_ASSERT(device.createImageView(&imageViewInfo, nullptr, &imageView) == vk::Result::eSuccess, "cannot create depth image view");
    }
    SwapchainDepthImage::SwapchainDepthImage(SwapchainDepthImage&& other) noexcept
    {
        image = other.image;
        imageView = other.imageView;
        memory = other.memory;

        other.Reset();
    }
    SwapchainDepthImage& SwapchainDepthImage::operator=(SwapchainDepthImage&& other) noexcept
    {
        image = other.image;
        imageView = other.imageView;
        memory = other.memory;

        other.Reset();

        return *this;
    }
    void SwapchainDepthImage::Reset() noexcept
    {
        image = nullptr;
        imageView = nullptr;
        memory = nullptr;
    }
    void SwapchainDepthImage::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyImageView(imageView);
        device.destroyImage(image);
        device.freeMemory(memory);
    }
}