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
        Create(device, physicalDevice, surface, width, height, queueFamilyIndices);
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
            frame.Cleanup(device);

        m_CommandBuffer.Cleanup(device);
        device.destroySwapchainKHR(m_Swapchain);
    }
    void VulkanSwapchain::Create(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const Types::uint& width, const Types::uint& height, const VulkanQueueFamilyIndices& queueFamilyIndices)
    {
        m_Details = QuerySwapChainSupport(physicalDevice, surface);
        const auto format = ChooseSwapchainSurfaceFormat(m_Details.formats);
        const auto present = ChooseSwapchainPresentMode(m_Details.presentModes);
        const auto extent = ChooseSwapchainExtent(width, height, m_Details.capabilities);

        const Types::uint imageCount = std::min(m_Details.capabilities.maxImageCount, m_Details.capabilities.minImageCount+1);

        GE_LOG(VulkanCore, Info, "max images count: ", m_Details.capabilities.maxImageCount, "; min images count: ", m_Details.capabilities.minImageCount);

        vk::SwapchainCreateInfoKHR createInfo {vk::SwapchainCreateFlagsKHR()};
        createInfo.minImageCount = imageCount;
        createInfo.surface = surface;
        createInfo.imageFormat = format.format;
        createInfo.imageColorSpace = format.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        createInfo.preTransform = m_Details.capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = present;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

        if(queueFamilyIndices.graphicsFamily.value() != queueFamilyIndices.presentFamily.value())
        {
            const Types::uint queueIndices[] = { queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value() };

            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueIndices;
        }
        else
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;

        m_Swapchain = device.createSwapchainKHR(createInfo);

        const std::vector images = device.getSwapchainImagesKHR(m_Swapchain);
        m_Frames.resize(images.size());

        GE_LOG(VulkanCore, Info, "images count: ", images.size());

        for(Types::uint i = 0; i < images.size(); ++i)
        {
            //store images
            vk::ImageViewCreateInfo imageViewInfo{};
            imageViewInfo.image = images[i];
            imageViewInfo.viewType = vk::ImageViewType::e2D;
            imageViewInfo.components.r = vk::ComponentSwizzle::eIdentity;
            imageViewInfo.components.g = vk::ComponentSwizzle::eIdentity;
            imageViewInfo.components.b = vk::ComponentSwizzle::eIdentity;
            imageViewInfo.components.a = vk::ComponentSwizzle::eIdentity;
            imageViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            imageViewInfo.subresourceRange.baseMipLevel = 0;
            imageViewInfo.subresourceRange.levelCount = 1;
            imageViewInfo.subresourceRange.baseArrayLayer = 0;
            imageViewInfo.subresourceRange.layerCount = 1;
            imageViewInfo.format = format.format;

            m_Frames[i] = VulkanSwapchainFrame(device, images[i], imageViewInfo);
        }

        m_Format = format.format;
        m_Extent = extent;

        m_MaxFramesInFlight = m_Frames.size();
        m_CurrentFrameNumber = 0;

        m_CommandBuffer = VulkanCommandBuffer(device, queueFamilyIndices, m_Frames);
    }
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
    void VulkanSwapchain::MakeFrames(const vk::Device& device, const vk::RenderPass& renderPass)
    {
        VulkanFrameBuffer::Make(device, renderPass, m_Extent, m_Frames);
    }
    void VulkanSwapchain::Recreate(const vk::Device& device, const vk::PhysicalDevice& physicalDevice,
        const vk::SurfaceKHR& surface, const Types::uint& width, const Types::uint& height, 
        const VulkanQueueFamilyIndices& queueFamilyIndices)
    {

    }
    vk::SurfaceFormatKHR VulkanSwapchain::ChooseSwapchainSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats)
    {
        for(auto&& format : formats)
            if(format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
                return format;

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
        GE_LOG(VulkanCore, Info, "\nmin width: ", capabilities.minImageExtent.width, "; min height: ", capabilities.minImageExtent.height, '\n',
            "max width: ", capabilities.maxImageExtent.width, "; max height: ", capabilities.maxImageExtent.height);

        if(capabilities.currentExtent.width != UINT32_MAX)
            return capabilities.currentExtent;
        else
        {
            vk::Extent2D extent;

            extent.width = std::min(capabilities.maxImageExtent.width, std::max(capabilities.minImageExtent.width, width));
            extent.height = std::min(capabilities.maxImageExtent.height, std::max(capabilities.minImageExtent.height, height));

            return extent;
        }
    }
}