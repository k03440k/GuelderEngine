module;
#include <vulkan/vulkan.hpp>
#include "../public/GuelderEngine/Utils/Debug.hpp"
export module GuelderEngine.Vulkan;
import :Swapchain;

//import :SwapchainFrameSync;//removed because of lnk1227
import :QueueFamilyIndices;
import :SwapchainFrame;
import :CommandPool;
import :FrameBuffer;
import :DebugManager;

import <vector>;
import <functional>;

//ctors and operator='s
namespace GuelderEngine::Vulkan
{
    Swapchain::Swapchain(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
        const vk::Extent2D& extent, const QueueFamilyIndices& queueFamilyIndices)
    {
        m_IsSwapchain = false;
        Create(device, physicalDevice, surface, extent, queueFamilyIndices);

        m_CommandPool = CommandPool(device, queueFamilyIndices, vk::QueueFlagBits::eGraphics);//TODO: move command pools to pipeline and finish buffers remaking
        m_CommandPoolTransfer = CommandPool(device, queueFamilyIndices, vk::QueueFlagBits::eTransfer);

        const std::vector images = device.getSwapchainImagesKHR(m_Swapchain);

        CreateFrames(device, m_Format, images);

        m_MaxFramesInFlight = m_Frames.size();
        m_CurrentFrameNumber = 0;
    }
    Swapchain::Swapchain(const Swapchain& other)
    {
        m_Details = other.m_Details;
        m_Extent = other.m_Extent;
        m_Format = other.m_Format;
        m_Frames = other.m_Frames;
        m_Swapchain = other.m_Swapchain;
        m_CommandPool = other.m_CommandPool;
        m_CommandPoolTransfer = other.m_CommandPoolTransfer;
        m_MaxFramesInFlight = other.m_MaxFramesInFlight;
        m_CurrentFrameNumber = other.m_CurrentFrameNumber;
        m_IsSwapchain = other.m_IsSwapchain;
    }
    Swapchain::Swapchain(Swapchain&& other) noexcept
    {
        m_Details = other.m_Details;
        m_Extent = other.m_Extent;
        m_Format = other.m_Format;
        m_Frames = other.m_Frames;
        m_Swapchain = other.m_Swapchain;
        m_CommandPool = std::forward<CommandPool>(other.m_CommandPool);
        m_CommandPoolTransfer = std::forward<CommandPool>(other.m_CommandPoolTransfer);
        m_MaxFramesInFlight = other.m_MaxFramesInFlight;
        m_CurrentFrameNumber = other.m_CurrentFrameNumber;
        m_IsSwapchain = other.m_IsSwapchain;

        other.Reset();
    }
    Swapchain& Swapchain::operator=(const Swapchain& other)
    {
        if(this == &other)
            return *this;

        m_Details = other.m_Details;
        m_Extent = other.m_Extent;
        m_Format = other.m_Format;
        m_Frames = other.m_Frames;
        m_Swapchain = other.m_Swapchain;
        m_CommandPool = other.m_CommandPool;
        m_CommandPoolTransfer = other.m_CommandPoolTransfer;
        m_MaxFramesInFlight = other.m_MaxFramesInFlight;
        m_CurrentFrameNumber = other.m_CurrentFrameNumber;
        m_IsSwapchain = other.m_IsSwapchain;

        return *this;
    }
    Swapchain& Swapchain::operator=(Swapchain&& other) noexcept
    {
        m_Details = other.m_Details;
        m_Extent = other.m_Extent;
        m_Format = other.m_Format;
        m_Frames = other.m_Frames;
        m_Swapchain = other.m_Swapchain;
        m_CommandPool = std::forward<CommandPool>(other.m_CommandPool);
        m_CommandPoolTransfer = std::forward<CommandPool>(other.m_CommandPoolTransfer);
        m_MaxFramesInFlight = other.m_MaxFramesInFlight;
        m_CurrentFrameNumber = other.m_CurrentFrameNumber;
        m_IsSwapchain = other.m_IsSwapchain;

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void Swapchain::Create(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Extent2D& extent, const QueueFamilyIndices& queueFamilyIndices)
    {
        m_Details = QuerySwapChainSupport(physicalDevice, surface);
        const auto format = ChooseSwapchainSurfaceFormat(m_Details.formats);
        const auto present = ChooseSwapchainPresentMode(m_Details.presentModes);
        const auto chosenExtent = ChooseSwapchainExtent(extent, m_Details.capabilities);

        const Types::uint imageCount = std::min(m_Details.capabilities.maxImageCount, m_Details.capabilities.minImageCount+1);

        //GE_LOG(VulkanCore, Info, "max images count: ", m_Details.capabilities.maxImageCount, "; min images count: ", m_Details.capabilities.minImageCount);

        vk::SwapchainCreateInfoKHR createInfo {vk::SwapchainCreateFlagsKHR()};
        createInfo.minImageCount = imageCount;
        createInfo.surface = surface;
        createInfo.imageFormat = format.format;
        createInfo.imageColorSpace = format.colorSpace;
        createInfo.imageExtent = chosenExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        createInfo.preTransform = m_Details.capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = present;
        createInfo.clipped = VK_TRUE;

        if(!m_IsSwapchain)
            createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);
        else
            createInfo.oldSwapchain = m_Swapchain;

        std::vector<Types::uint> queueIndices(2);

        if(queueFamilyIndices.GetGraphicsFamily() != queueFamilyIndices.GetPresentFamily())
        {
            //const Types::uint queueIndices[] = { queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value(), queueFamilyIndices.transferFamily.value() };
            queueIndices[0] = queueFamilyIndices.GetGraphicsFamily();
            queueIndices[1] = queueFamilyIndices.GetPresentFamily();
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        }
        else
        {
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
            queueIndices.push_back(queueFamilyIndices.GetGraphicsFamily());
        }

        if(queueFamilyIndices.GetGraphicsFamily() != queueFamilyIndices.GetTransferFamily())
            queueIndices.push_back(queueFamilyIndices.GetTransferFamily());

        if(queueIndices.size())
        {
            createInfo.queueFamilyIndexCount = queueIndices.size();
            createInfo.pQueueFamilyIndices = queueIndices.data();
        }

        const auto newSwapchain = device.createSwapchainKHR(createInfo);

        if(!m_IsSwapchain)
        {
            m_Swapchain = newSwapchain;
            m_IsSwapchain = true;
        }
        else
        {
            device.destroySwapchainKHR(m_Swapchain);
            m_Swapchain = newSwapchain;
        }

        m_Format = format.format;
        m_Extent = chosenExtent;
    }
    void Swapchain::Reset() noexcept
    {
        m_Details = {};
        m_Extent = vk::Extent2D{};
        m_Format = {};
        m_Swapchain = nullptr;
        m_IsSwapchain = false;

        for(auto&& frame : m_Frames)//idk
            frame.Reset();

        m_Frames.clear();
        m_CommandPool.Reset();
        m_CommandPoolTransfer.Reset();
        m_MaxFramesInFlight = 0;
        m_CurrentFrameNumber = 0;
    }
    void Swapchain::Cleanup(const vk::Device& device) const noexcept
    {
        for(auto&& frame : m_Frames)
            frame.Cleanup(device, m_CommandPool.GetCommandPool());

        m_CommandPool.Cleanup(device);
        m_CommandPoolTransfer.Cleanup(device);
        device.destroySwapchainKHR(m_Swapchain);
    }

    void Swapchain::CreateFrames(const vk::Device& device, const vk::Format& format, const std::vector<vk::Image>& images)
    {
        m_Frames.resize(images.size());

        //GE_LOG(VulkanCore, Info, "images count: ", images.size());

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
            imageViewInfo.format = format;

            m_Frames[i] = Vulkan::SwapchainFrame(device, imageViewInfo, m_CommandPool.GetCommandPool());
        }
    }
    void Swapchain::MakeFrames(const vk::Device& device, const vk::RenderPass& renderPass)
    {
        FrameBuffer::Make(device, renderPass, m_Extent, m_Frames);
    }

    SwapchainSupportDetails Swapchain::QuerySwapChainSupport(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface)
    {
        SwapchainSupportDetails support;
        support.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
        support.formats = physicalDevice.getSurfaceFormatsKHR(surface);
        support.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);

#ifdef GE_DEBUG_VULKAN
        /*GE_LOG(VulkanCore, Info, "The device can support following Present Modes:");
        for(auto&& mode : support.presentModes)
        {
            DebugManager::LogPresentMode(mode);
        }*/
#endif

        return support;
    }
    vk::SurfaceFormatKHR Swapchain::ChooseSwapchainSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats)
    {
        for(auto&& format : formats)
            if(format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
                return format;

        GE_LOG(VulkanCore, Warning, "the device doesn't support: vk::Format::eB8G8R8A8Unorm and vk::ColorSpaceKHR::eSrgbNonlinear");
        return formats[0];
    }
    vk::PresentModeKHR Swapchain::ChooseSwapchainPresentMode(const std::vector<vk::PresentModeKHR>& presentModes)
    {
        for(auto&& presentMode : presentModes)
        {
            if(presentMode == vk::PresentModeKHR::eMailbox)
                return presentMode;
        }

        GE_LOG(VulkanCore, Warning, "the device doesn't support Mailbox present mode. Choosing Fifo");
        return vk::PresentModeKHR::eFifo;
    }
    vk::Extent2D Swapchain::ChooseSwapchainExtent(const vk::Extent2D& extent, const vk::SurfaceCapabilitiesKHR& capabilities)
    {
        if(capabilities.currentExtent.width != UINT32_MAX)
            return capabilities.currentExtent;
        else
        {
            vk::Extent2D chosenExtent{
                std::min(capabilities.maxImageExtent.width, std::max(capabilities.minImageExtent.width, extent.width)),
                std::min(capabilities.maxImageExtent.height, std::max(capabilities.minImageExtent.height, extent.height))
            };

            return chosenExtent;
        }
    }

    void Swapchain::Recreate(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::RenderPass& renderPass, const vk::Extent2D& extent,
        const QueueFamilyIndices& queueFamilyIndices)
    {
        Create(device, physicalDevice, surface, extent, queueFamilyIndices);

        const std::vector images = device.getSwapchainImagesKHR(m_Swapchain);

        for(Types::uint i = 0; i < images.size(); i++)
        {
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
            imageViewInfo.format = m_Format;

            m_Frames[i].Recreate(device, renderPass,
                extent, imageViewInfo, m_CommandPool.GetCommandPool());
        }
    }
}
namespace GuelderEngine::Vulkan
{
    const CommandPool& Swapchain::GetCommandPool() const noexcept
    {
        return m_CommandPool;
    }
    const CommandPool& Swapchain::GetCommandPoolTransfer() const noexcept
    {
        return m_CommandPoolTransfer;
    }
    Types::uint Swapchain::IncrementCurrentFrame() noexcept
    {
        m_CurrentFrameNumber = (m_CurrentFrameNumber + 1) % m_MaxFramesInFlight;
        return m_CurrentFrameNumber;
    }
    const std::vector<SwapchainFrame>& Swapchain::GetFrames() const noexcept
    {
        return m_Frames;
    }
    const vk::SwapchainKHR& Swapchain::GetSwapchain() const noexcept
    {
        return m_Swapchain;
    }
    const vk::Format& Swapchain::GetFormat() const noexcept
    {
        return m_Format;
    }
    const vk::Extent2D& Swapchain::GetExtent2D() const noexcept
    {
        return m_Extent;
    }
    Types::uint Swapchain::GetCurrentFrameNumber() const noexcept
    {
        return m_CurrentFrameNumber;
    }
    Types::uint Swapchain::GetMaxFramesInFlight() const noexcept
    {
        return m_MaxFramesInFlight;
    }
}