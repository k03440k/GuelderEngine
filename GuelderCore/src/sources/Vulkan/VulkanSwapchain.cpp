module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan;
import :VulkanSwapchain;

import :VulkanSync;
import :VulkanQueueFamilyIndices;
import :VulkanSwapchainFrame;
import :VulkanCommandBuffer;
import :VulkanFrameBuffer;
import :VulkanDebugManager;

import <vector>;

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
        m_MaxFramesInFlight = m_Frames.size();
        m_CurrentFrameNumber = 0;

        for(auto&& frame : m_Frames)//&& ? &
        {
            frame.sync.m_ImageAvailable = VulkanSync::MakeSemaphore(device);
            frame.sync.m_RenderFinished = VulkanSync::MakeSemaphore(device);
            frame.sync.m_InFlightFence = VulkanSync::MakeFence(device);
        }
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
        m_MaxFramesInFlight = other.m_MaxFramesInFlight;
        m_CurrentFrameNumber = other.m_CurrentFrameNumber;
    }
    VulkanSwapchain::VulkanSwapchain(VulkanSwapchain&& other) noexcept
    {
        m_Details = other.m_Details;
        m_Extent = other.m_Extent;
        m_Format = other.m_Format;
        m_Frames = other.m_Frames;
        m_Swapchain = other.m_Swapchain;
        m_CommandBuffer = std::forward<VulkanCommandBuffer>(other.m_CommandBuffer);
        m_MaxFramesInFlight = other.m_MaxFramesInFlight;
        m_CurrentFrameNumber = other.m_CurrentFrameNumber;

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
        m_MaxFramesInFlight = other.m_MaxFramesInFlight;
        m_CurrentFrameNumber = other.m_CurrentFrameNumber;

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
        m_MaxFramesInFlight = other.m_MaxFramesInFlight;
        m_CurrentFrameNumber = other.m_CurrentFrameNumber;

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
        m_MaxFramesInFlight = 0;
        m_CurrentFrameNumber = 0;
    }
    void VulkanSwapchain::Cleanup(const vk::Device& device) const noexcept
    {
        for(auto&& frame : m_Frames)
        {
            device.destroyImageView(frame.imageView);
            device.destroyFramebuffer(frame.framebuffer);
            frame.sync.Cleanup(device);
        }

        m_CommandBuffer.Cleanup(device);
        device.destroySwapchainKHR(m_Swapchain);
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