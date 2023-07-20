module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan;
import :VulkanSwapchain;

//import :VulkanPipeline;
import :VulkanSync;
import :VulkanDebugManager;

import <vector>;
import <array>;

//ctors and operator='s
namespace GuelderEngine::Vulkan
{
    VulkanQueueFamilyIndices::VulkanQueueFamilyIndices(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
    {
        const std::vector queueFamilies = device.getQueueFamilyProperties();

#ifdef GE_DEBUG_VULKAN
        GE_LOG(VulkanCore, Info, "Device can support ", queueFamilies.size(), " Queue Families");
#endif

        for(size_t i = 0; i < queueFamilies.size(); i++)
        {
            if(queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics && device.getSurfaceSupportKHR(i, surface))
            {
                graphicsFamily = i;
                presentFamily = i;

#ifdef GE_DEBUG_VULKAN
                GE_LOG(VulkanCore, Info, "Queue Family at index ", i, " is suitable for graphics and presenting");
#endif
            }
            if(IsComplete())
                break;
        }

        GE_CORE_CLASS_ASSERT(IsComplete(), "Cannot complete Queue Family(device doesn't support requirements)");
    }
    VulkanQueueFamilyIndices::VulkanQueueFamilyIndices(const VulkanQueueFamilyIndices& other)
    {
        graphicsFamily = other.graphicsFamily;
        presentFamily = other.presentFamily;
    }
    VulkanQueueFamilyIndices& VulkanQueueFamilyIndices::operator=(const VulkanQueueFamilyIndices& other)
    {
        if(this == &other)
            return *this;

        graphicsFamily = other.graphicsFamily;
        presentFamily = other.presentFamily;

        return *this;
    }

    VulkanSwapchainFrame::VulkanSwapchainFrame(const VulkanSwapchainFrame& other)
    {
        image = other.image;
        imageView = other.imageView;
        framebuffer = other.framebuffer;
        commandBuffer = other.commandBuffer;
    }
    VulkanSwapchainFrame::VulkanSwapchainFrame(VulkanSwapchainFrame&& other) noexcept
    {
        image = other.image;
        imageView = other.imageView;
        framebuffer = other.framebuffer;
        commandBuffer = other.commandBuffer;

        other.Reset();
    }
    VulkanSwapchainFrame& VulkanSwapchainFrame::operator=(const VulkanSwapchainFrame& other)
    {
        if(this == &other)
            return *this;

        image = other.image;
        imageView = other.imageView;
        framebuffer = other.framebuffer;
        commandBuffer = other.commandBuffer;

        return *this;
    }
    VulkanSwapchainFrame& VulkanSwapchainFrame::operator=(VulkanSwapchainFrame&& other) noexcept
    {
        image = other.image;
        imageView = other.imageView;
        framebuffer = other.framebuffer;
        commandBuffer = other.commandBuffer;

        other.Reset();

        return *this;
    }
    VulkanCommandBuffer::VulkanCommandBuffer(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices, const vk::SurfaceKHR& surface,
        std::vector<VulkanSwapchainFrame>& frames)
    {
        m_CommandPool = MakePool(device, queueFamilyIndices, surface);
        m_CommandBuffer = MakeBuffer(device, m_CommandPool, frames);
    }
    VulkanCommandBuffer::VulkanCommandBuffer(const VulkanCommandBuffer& other)
    {
        m_CommandPool = other.m_CommandPool;
        m_CommandBuffer = other.m_CommandBuffer;
    }
    VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandBuffer&& other) noexcept
    {
        m_CommandPool = other.m_CommandPool;
        m_CommandBuffer = other.m_CommandBuffer;

        other.Reset();
    }
    VulkanCommandBuffer& VulkanCommandBuffer::operator=(const VulkanCommandBuffer& other)
    {
        if(this == &other)
            return *this;

        m_CommandPool = other.m_CommandPool;
        m_CommandBuffer = other.m_CommandBuffer;

        return *this;
    }
    VulkanCommandBuffer& VulkanCommandBuffer::operator=(VulkanCommandBuffer&& other) noexcept
    {
        m_CommandPool = other.m_CommandPool;
        m_CommandBuffer = other.m_CommandBuffer;

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void VulkanSwapchainFrame::Reset() noexcept
    {
        image = nullptr;
        imageView = nullptr;
        framebuffer = nullptr;
        commandBuffer = nullptr;
    }
    //VulkanFrameBuffer::VulkanFrameBuffer(const vk::Device& device, const vk::RenderPass& renderPass, const vk::Extent2D& swapchainExtent,
//    std::vector<VulkanSwapchainFrame>& frames)
//{
//    for (Types::uint i = 0; i < frames.size(); ++i)
//    {
//        std::vector attachments{frames[i].imageView};//idk
//        //auto attachment = frames[i].imageView;
//        const vk::FramebufferCreateInfo framebufferInfo(
//            vk::FramebufferCreateFlags(),
//            renderPass,
//            attachments.size(),
//            attachments.data(),
//            swapchainExtent.width,
//            swapchainExtent.height,
//            1
//        );
//        frames[i].framebuffer = device.createFramebuffer(framebufferInfo);
//    }
//}

    void VulkanFrameBuffer::Make(const vk::Device& device, const vk::RenderPass& renderPass,
        const vk::Extent2D& swapchainExtent, std::vector<struct VulkanSwapchainFrame>& frames)
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

            frames[i].framebuffer = device.createFramebuffer(framebufferInfo);
        }
    }

    void VulkanCommandBuffer::Reset() noexcept
    {
        m_CommandPool = nullptr;
        m_CommandBuffer = nullptr;
    }
    void VulkanCommandBuffer::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyCommandPool(m_CommandPool);
    }
    vk::CommandPool VulkanCommandBuffer::MakePool(const vk::Device& device, const VulkanQueueFamilyIndices& queueFamilyIndices, const vk::SurfaceKHR& surface)
    {
        const vk::CommandPoolCreateInfo poolInfo(
            vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            queueFamilyIndices.graphicsFamily.value()
        );

        return device.createCommandPool(poolInfo);
    }
    vk::CommandBuffer VulkanCommandBuffer::MakeBuffer(const vk::Device& device, const vk::CommandPool& pool, std::vector<VulkanSwapchainFrame>& frames)
    {
        const vk::CommandBufferAllocateInfo bufferInfo(
            pool,
            vk::CommandBufferLevel::ePrimary,
            1
        );

        for(auto&& frame : frames)
            frame.commandBuffer = device.allocateCommandBuffers(bufferInfo)[0];

        const vk::CommandBuffer buffer = device.allocateCommandBuffers(bufferInfo)[0];

        return buffer;
    }
}

//ctors and operator='s
namespace GuelderEngine::Vulkan
{
    VulkanSwapchain::VulkanSwapchain(const vk::Device& device, const vk::PhysicalDevice& physicalDevice,
        const vk::SurfaceKHR& surface, const Types::uint& width, const Types::uint& height,
        const VulkanQueueFamilyIndices& queueFamilyIndices)
    {
        m_Details = QuerySwapChainSupport(physicalDevice, surface);
        auto format = ChooseSwapchainSurfaceFormat(m_Details.formats);
        auto present = ChooseSwapchainPresentMode(m_Details.presentModes);
        auto extent = ChooseSwapchainExtent(width, height, m_Details.capabilities);

        const Types::uint imageCount = std::min(m_Details.capabilities.maxImageCount, m_Details.capabilities.minImageCount+1);

        vk::SwapchainCreateInfoKHR createInfo
        {
            vk::SwapchainCreateFlagsKHR(),
                surface,
                imageCount,
                format.format,
                format.colorSpace,
                extent,
                1,
                vk::ImageUsageFlagBits::eColorAttachment
        };
        const Types::uint queueIndices[] = { queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value() };

        if(queueFamilyIndices.graphicsFamily.value() != queueFamilyIndices.presentFamily.value())
        {
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueIndices;
        }
        else
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;

        createInfo.preTransform = m_Details.capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = present;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

        m_Swapchain = device.createSwapchainKHR(createInfo);
        std::vector images = device.getSwapchainImagesKHR(m_Swapchain);
        m_Frames.resize(images.size());

        for(Types::uint i = 0; i < images.size(); ++i)
        {
            vk::ImageViewCreateInfo createInfoViewCreateInfo{};
            createInfoViewCreateInfo.image = images[i];
            createInfoViewCreateInfo.viewType = vk::ImageViewType::e2D;
            createInfoViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
            createInfoViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
            createInfoViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
            createInfoViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
            createInfoViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            createInfoViewCreateInfo.subresourceRange.baseMipLevel = 0;
            createInfoViewCreateInfo.subresourceRange.levelCount = 1;
            createInfoViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            createInfoViewCreateInfo.subresourceRange.layerCount = 1;
            createInfoViewCreateInfo.format = format.format;

            m_Frames[i].image = images[i];
            m_Frames[i].imageView = device.createImageView(createInfoViewCreateInfo);
        }

        m_Format = format.format;
        m_Extent = extent;

        m_CommandBuffer = VulkanCommandBuffer(device, queueFamilyIndices, surface, m_Frames);
        m_Sync = VulkanSync(device);
    }
    VulkanSwapchain::VulkanSwapchain(const VulkanSwapchainCreateInfo& info)
        : VulkanSwapchain(info.device, info.physicalDevice, info.surface, info.width, info.height, info.queueFamilyIndices) {}
    VulkanSwapchain::VulkanSwapchain(const VulkanSwapchain& other)
    {
        m_Details = other.m_Details;
        m_Extent = other.m_Extent;
        m_Format = other.m_Format;
        m_Frames = other.m_Frames;
        m_Swapchain = other.m_Swapchain;
        m_CommandBuffer = other.m_CommandBuffer;
        m_Sync = other.m_Sync;
    }
    VulkanSwapchain::VulkanSwapchain(VulkanSwapchain&& other) noexcept
    {
        m_Details = other.m_Details;
        m_Extent = other.m_Extent;
        m_Format = other.m_Format;
        m_Frames = other.m_Frames;
        m_Swapchain = other.m_Swapchain;
        m_CommandBuffer = std::forward<VulkanCommandBuffer>(other.m_CommandBuffer);
        m_Sync = std::forward<VulkanSync>(other.m_Sync);

        other.Reset();
    }
    VulkanSwapchain& VulkanSwapchain::operator=(const VulkanSwapchain& other)
    {
        if(this == &other)
            return *this;

        m_Details = other.m_Details;
        m_Extent = other.m_Extent;
        m_Format = other.m_Format;
        m_Frames = other.m_Frames;
        m_Swapchain = other.m_Swapchain;
        m_CommandBuffer = other.m_CommandBuffer;
        m_Sync = other.m_Sync;

        return *this;
    }
    VulkanSwapchain& VulkanSwapchain::operator=(VulkanSwapchain&& other) noexcept
    {
        m_Details = other.m_Details;
        m_Extent = other.m_Extent;
        m_Format = other.m_Format;
        m_Frames = other.m_Frames;
        m_Swapchain = other.m_Swapchain;
        m_CommandBuffer = std::forward<VulkanCommandBuffer>(other.m_CommandBuffer);
        m_Sync = std::forward<VulkanSync>(other.m_Sync);

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    VulkanSwapchainSupportDetails VulkanSwapchain::QuerySwapChainSupport(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface)
    {
        VulkanSwapchainSupportDetails support;

        support.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
        support.formats = physicalDevice.getSurfaceFormatsKHR(surface);
        support.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);

#ifdef GE_DEBUG_VULKAN
        GE_LOG(VulkanCore, Info, "The device can support following Present Modes:");
        for(auto&& mode : support.presentModes)
        {
            VulkanDebugManager::LogPresentMode(mode);
        }
#endif

        return support;
    }
    void VulkanSwapchain::Reset() noexcept
    {
        m_Details = {};
        m_Extent = vk::Extent2D{};
        m_Format = {};
        m_Swapchain = nullptr;
        
        for(auto&& frame : m_Frames)//idk
            frame.Reset();

        m_Frames.clear();
        m_CommandBuffer.Reset();
        m_Sync.Reset();
    }
    void VulkanSwapchain::Cleanup(const vk::Device& device) const noexcept
    {
        for(auto&& frame : m_Frames)
        {
            device.destroyImageView(frame.imageView);
            device.destroyFramebuffer(frame.framebuffer);
        }

        m_CommandBuffer.Cleanup(device);
        device.destroySwapchainKHR(m_Swapchain);
        m_Sync.Cleanup(device);
    }
    void VulkanSwapchain::MakeFrames(const vk::Device& device, const vk::RenderPass& renderPass)
    {
        VulkanFrameBuffer::Make(device, renderPass, m_Extent, m_Frames);
    }
    vk::SurfaceFormatKHR VulkanSwapchain::ChooseSwapchainSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats)
    {
        for(auto&& format : formats)
        {
            if(format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
                return format;
        }

        GE_LOG(VulkanCore, Warning, "the device doesn't support: vk::Format::eB8G8R8A8Unorm and vk::ColorSpaceKHR::eSrgbNonlinear");
        return formats[0];
    }
    vk::PresentModeKHR VulkanSwapchain::ChooseSwapchainPresentMode(const std::vector<vk::PresentModeKHR>& presentModes)
    {
        for(auto&& presentMode : presentModes)
        {
            if(presentMode == vk::PresentModeKHR::eMailbox)
                return presentMode;
        }

        GE_LOG(VulkanCore, Warning, "the device doesn't support Mailbox present mode. Choosing Fifo");
        return vk::PresentModeKHR::eFifo;
    }
    vk::Extent2D VulkanSwapchain::ChooseSwapchainExtent(const Types::uint& width, const Types::uint& height, const vk::SurfaceCapabilitiesKHR& capabilities)
    {
        if(capabilities.currentExtent.width != UINT32_MAX)
            return capabilities.currentExtent;
        else
        {
            vk::Extent2D extent{width, height};

            extent.width = std::min(capabilities.maxImageExtent.width, std::max(capabilities.minImageExtent.width, width));
            extent.height = std::min(capabilities.maxImageExtent.height, std::max(capabilities.minImageExtent.height, height));

            return extent;
        }
    }
}