module;
#include <vulkan/vulkan.hpp>
#include "../public/GuelderEngine/Utils/Debug.hpp"
export module GuelderEngine.Vulkan;
import :Swapchain;

//import :SwapchainFrameSync;//removed because of lnk1227
import :DeviceManager;
import :QueueFamilyIndices;
import :SwapchainFrame;
import :SwapchainDepthImage;
import :CommandPool;
import :FrameBuffer;
import :DebugManager;

import <vector>;
import <functional>;

//ctors and operator='s
namespace GuelderEngine::Vulkan
{
    Swapchain::Swapchain(
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const vk::SurfaceKHR& surface,
        const vk::SurfaceCapabilitiesKHR& surfaceCapabilities,
        const vk::SurfaceFormatKHR& surfaceFormat,
        const vk::PresentModeKHR& presentMode,
        const vk::Extent2D& extent,
        const vk::CommandPool& commandPool,
        const QueueFamilyIndices& queueFamilyIndices)
    {
        m_IsSwapchain = false;
        Create(device, physicalDevice, surface, surfaceCapabilities, surfaceFormat, presentMode, extent, queueFamilyIndices);

        const std::vector images = device.getSwapchainImagesKHR(m_Swapchain);

        CreateFrames(device, m_Format, commandPool, images);
        CreateDepthImages(device, physicalDevice, m_DepthFormat, queueFamilyIndices, images);
        m_RenderPass = CreateRenderPass(device, physicalDevice, m_Format);
        FrameBuffer::Make(device, m_RenderPass, m_Extent, m_Frames, m_DepthImages);

        m_MaxFramesInFlight = m_Frames.size();
        m_CurrentFrameNumber = 0;
    }
    Swapchain::Swapchain(Swapchain&& other) noexcept
    {
        m_Extent = other.m_Extent;
        m_Format = other.m_Format;
        m_DepthFormat = other.m_DepthFormat;
        m_Frames = other.m_Frames;
        m_DepthImages = other.m_DepthImages;
        m_Swapchain = other.m_Swapchain;
        m_MaxFramesInFlight = other.m_MaxFramesInFlight;
        m_CurrentFrameNumber = other.m_CurrentFrameNumber;
        m_IsSwapchain = other.m_IsSwapchain;
        m_RenderPass = other.m_RenderPass;

        other.Reset();
    }
    Swapchain& Swapchain::operator=(Swapchain&& other) noexcept
    {
        m_Extent = other.m_Extent;
        m_Format = other.m_Format;
        m_DepthFormat = other.m_DepthFormat;
        m_Frames = other.m_Frames;
        m_DepthImages = other.m_DepthImages;
        m_Swapchain = other.m_Swapchain;
        m_MaxFramesInFlight = other.m_MaxFramesInFlight;
        m_CurrentFrameNumber = other.m_CurrentFrameNumber;
        m_IsSwapchain = other.m_IsSwapchain;
        m_RenderPass = other.m_RenderPass;

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    vk::RenderPass Swapchain::CreateRenderPass(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::Format& swapchainImageFormat)
    {
        const vk::AttachmentDescription colorAttachment(
            vk::AttachmentDescriptionFlags(),
            swapchainImageFormat,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR
        );
        //TODO: subpass dependencies SETUP THIS SHIT
        const vk::AttachmentDescription depthAttachment{
            vk::AttachmentDescriptionFlags(),
            DeviceManager::FindSupportedFormat(physicalDevice, 
                {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint}, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment),
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eDontCare,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eDepthStencilAttachmentOptimal
        };

        const vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
        const vk::AttachmentReference depthAttachmentRef(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

        const vk::SubpassDescription subpassDescription(
            vk::SubpassDescriptionFlags(),
            vk::PipelineBindPoint::eGraphics,
            0,
            nullptr,
            1,
            &colorAttachmentRef,
            nullptr,
            &depthAttachmentRef
        );

        const vk::SubpassDependency subpassDependency{
            VK_SUBPASS_EXTERNAL,
            0,
            vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
            vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
            {},
            vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite
        };

        std::array attachments{ colorAttachment, depthAttachment };

        const vk::RenderPassCreateInfo info{
            vk::RenderPassCreateFlagBits(),
            attachments.size(),
            attachments.data(),
            1,
            &subpassDescription,
            1,
            &subpassDependency
        };

        return device.createRenderPass(info);
    }
    void Swapchain::Create
    (
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const vk::SurfaceKHR& surface,
        const vk::SurfaceCapabilitiesKHR& surfaceCapabilities,
        const vk::SurfaceFormatKHR& surfaceFormat,
        const vk::PresentModeKHR& presentMode,
        const vk::Extent2D& extent,
        const QueueFamilyIndices& queueFamilyIndices
    )
    {
        const auto chosenExtent = ChooseExtent(extent, surfaceCapabilities);

        const uint imageCount = std::min(surfaceCapabilities.maxImageCount, surfaceCapabilities.minImageCount+1);

        //GE_LOG(VulkanCore, Info, "max images count: ", m_Details.capabilities.maxImageCount, "; min images count: ", m_Details.capabilities.minImageCount);

        vk::SwapchainCreateInfoKHR createInfo {vk::SwapchainCreateFlagsKHR()};
        createInfo.minImageCount = imageCount;
        createInfo.surface = surface;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = chosenExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        createInfo.preTransform = surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        if(!m_IsSwapchain)
            createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);
        else
            createInfo.oldSwapchain = m_Swapchain;

        std::vector<uint> queueIndices(2);

        if(queueFamilyIndices.GetGraphicsFamily() != queueFamilyIndices.GetPresentFamily())
        {
            //const uint queueIndices[] = { queueFamilyIndices.graphicsFamily.value(), queueFamilyIndices.presentFamily.value(), queueFamilyIndices.transferFamily.value() };
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
            //m_IsSwapchain = true;
        }
        else
        {
            device.destroySwapchainKHR(m_Swapchain);
            m_Swapchain = newSwapchain;
        }

        const auto oldFormat = m_Format;
        const auto oldDepthFormat = m_DepthFormat;

        m_Format = surfaceFormat.format;
        m_DepthFormat = DeviceManager::FindSupportedFormat(
            physicalDevice, {vk::Format::eD32Sfloat,vk::Format::eD32SfloatS8Uint,vk::Format::eD24UnormS8Uint},
            vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);

        if(m_IsSwapchain && oldFormat != m_Format && oldDepthFormat != m_DepthFormat)
            GE_THROW("old swapchain formats are not equal to the new ones");//edge case. better to create a callback to handle this situation

        if(!m_IsSwapchain)
            m_IsSwapchain = true;

        m_Extent = chosenExtent;
    }
    void Swapchain::Reset() noexcept
    {
        m_Extent = vk::Extent2D{};
        m_Format = vk::Format::eUndefined;
        m_DepthFormat = vk::Format::eUndefined;
        m_Swapchain = nullptr;
        m_RenderPass = nullptr;
        m_IsSwapchain = false;

        //I think that I lose here a bunch of memory, because I don't perform any .Cleanup's methods of images

        m_Frames.clear();
        m_DepthImages.clear();
        m_MaxFramesInFlight = 0;
        m_CurrentFrameNumber = 0;
    }
    void Swapchain::Cleanup(const vk::Device& device, const vk::CommandPool& commandPool) const noexcept
    {
        for(auto&& frame : m_Frames)
            frame.Cleanup(device, commandPool);

        device.destroySwapchainKHR(m_Swapchain);
        for(auto&& frame : m_DepthImages)
            frame.Cleanup(device);
        device.destroyRenderPass(m_RenderPass);
    }

    void Swapchain::CreateFrames(const vk::Device& device, const vk::Format& format, const vk::CommandPool& commandPool, const std::vector<vk::Image>& images)
    {
        m_Frames.resize(images.size());

        //GE_LOG(VulkanCore, Info, "images count: ", images.size());

        for(uint i = 0; i < images.size(); ++i)
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

            m_Frames[i] = SwapchainFrame(device, imageViewInfo, commandPool);
        }
    }
    void Swapchain::CreateDepthImages(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::Format& format, const QueueFamilyIndices& queueFamilyIndices, const std::vector<vk::Image>& images)
    {
        m_DepthImages.resize(images.size());

        uint queueFamilyIndexCount;
        const uint* pQueueFamilyIndices = nullptr;
        vk::SharingMode sharingMode;

        if(queueFamilyIndices.GetGraphicsFamily() != queueFamilyIndices.GetTransferFamily())
        {
            const uint uniqueIndices[] = { queueFamilyIndices.GetGraphicsFamily(), queueFamilyIndices.GetTransferFamily() };
            queueFamilyIndexCount = 2;
            pQueueFamilyIndices = uniqueIndices;
            sharingMode = vk::SharingMode::eConcurrent;
        }
        else
            sharingMode = vk::SharingMode::eExclusive;

        for (uint i = 0; i < images.size(); ++i)
        {
            m_DepthImages[i] = SwapchainDepthImage(device, physicalDevice, m_Extent, format, sharingMode, queueFamilyIndexCount, pQueueFamilyIndices);
        }
    }
    vk::Extent2D Swapchain::ChooseExtent(const vk::Extent2D& extent, const vk::SurfaceCapabilitiesKHR& capabilities)
    {
        if(capabilities.currentExtent.width != UINT32_MAX)
            return capabilities.currentExtent;

        vk::Extent2D chosenExtent{
            std::min(capabilities.maxImageExtent.width, std::max(capabilities.minImageExtent.width, extent.width)),
                std::min(capabilities.maxImageExtent.height, std::max(capabilities.minImageExtent.height, extent.height))
        };

        return chosenExtent;
    }

    void Swapchain::Recreate
    (
        const vk::Device& device,
        const vk::PhysicalDevice& physicalDevice,
        const vk::SurfaceKHR& surface,
        const vk::SurfaceCapabilitiesKHR& surfaceCapabilities,
        const vk::SurfaceFormatKHR& surfaceFormat,
        const vk::PresentModeKHR& presentMode,
        const vk::Extent2D& extent,
        const vk::CommandPool& commandPool,
        const QueueFamilyIndices& queueFamilyIndices)
    {
        Create(device, physicalDevice, surface, surfaceCapabilities, surfaceFormat, presentMode, extent, queueFamilyIndices);

        const std::vector images = device.getSwapchainImagesKHR(m_Swapchain);

        for(auto&& image : m_DepthImages)
            image.Cleanup(device);

        CreateDepthImages(device, physicalDevice, m_DepthFormat, queueFamilyIndices, images);

        for(uint i = 0; i < images.size(); i++)
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

            m_Frames[i].Recreate(device, m_RenderPass,
                extent, imageViewInfo, m_DepthImages[i].imageView, commandPool);
        }
    }
    bool Swapchain::CompareSwapchainFormats(const Swapchain& other) const noexcept
    {
        return m_Format == other.m_Format && m_DepthFormat == other.m_DepthFormat;
    }
}
namespace GuelderEngine::Vulkan
{
    const SwapchainFrame& Swapchain::GetCurrentFrame() const
    {
        return m_Frames[m_CurrentFrameNumber];
    }
    const std::vector<SwapchainFrame>& Swapchain::GetFrames() const
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
    const vk::Format& Swapchain::GetDepthFormat() const noexcept
    {
        return m_DepthFormat;
    }
    const vk::Extent2D& Swapchain::GetExtent2D() const noexcept
    {
        return m_Extent;
    }
    uint& Swapchain::GetCurrentFrameNumber() noexcept
    {
        return m_CurrentFrameNumber;
    }
    uint Swapchain::GetMaxFramesInFlight() const noexcept
    {
        return m_MaxFramesInFlight;
    }
    const vk::RenderPass& Swapchain::GetRenderPass() const noexcept
    {
        return m_RenderPass;
    }
    float Swapchain::GetAspectRatio() const
    {
        return static_cast<float>(m_Extent.width) / static_cast<float>(m_Extent.height);
    }
}