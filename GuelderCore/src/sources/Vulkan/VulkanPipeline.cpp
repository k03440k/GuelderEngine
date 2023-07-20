module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
#include "GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :VulkanPipeline;

import :VulkanShaderManager;
import :VulkanSwapchain;
import GuelderEngine.Core;
import GuelderEngine.Core.Types;

namespace GuelderEngine::Vulkan
{
    /*VulkanPipeline::VulkanPipeline(const vk::Device& device, const vk::Extent2D& extent, const vk::Format& swapchainImageFormat, const std::string_view& vertexPath, const std::string_view& fragmentPath)
    {
        
    }*/
    VulkanPipeline::VulkanPipeline(const VulkanSwapchainCreateInfo& swapchainInfo, const std::string_view& vertexPath, const std::string_view& fragmentPath)
        : m_Swapchain(swapchainInfo)
    {
        m_Queues.graphicsQueue = GetGraphicsQueue(swapchainInfo.device, swapchainInfo.queueFamilyIndices);
        m_Queues.presentQueue = GetPresentQueue(swapchainInfo.device, swapchainInfo.queueFamilyIndices);

        constexpr vk::PipelineVertexInputStateCreateInfo vertexInfo(vk::PipelineVertexInputStateCreateFlags(), 0, nullptr, 0);
        constexpr vk::PipelineInputAssemblyStateCreateInfo assemblyInfo(vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList);

        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
        shaderStages.reserve(2);

        //vertex shader
        const vk::ShaderModule vertex = VulkanShaderManager::CreateModule(Utils::ResourceManager::GetFileSource(vertexPath), swapchainInfo.device);
        const vk::PipelineShaderStageCreateInfo vertexShaderInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertex, "main");
        shaderStages.push_back(vertexShaderInfo);
        //fragment shader
        const vk::ShaderModule fragment = VulkanShaderManager::CreateModule(Utils::ResourceManager::GetFileSource(fragmentPath), swapchainInfo.device);
        const vk::PipelineShaderStageCreateInfo fragmentShaderInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragment, "main");
        shaderStages.push_back(fragmentShaderInfo);

        //viewport
        const vk::Viewport viewport(0.0f, 0.0f, m_Swapchain.m_Extent.width, m_Swapchain.m_Extent.height, 0.0f, 1.0f);
        const vk::Rect2D scissor({ 0, 0 }, m_Swapchain.m_Extent);

        vk::PipelineViewportStateCreateInfo viewportInfo(
            vk::PipelineViewportStateCreateFlags(),
            1,          //viewports count
            &viewport,  //viewports
            1,          //scissors count
            &scissor    //scissors
        );

        //rasterizer
        const vk::PipelineRasterizationStateCreateInfo rasterizationInfo(
            vk::PipelineRasterizationStateCreateFlags(),
            VK_FALSE,
            VK_FALSE,
            vk::PolygonMode::eFill,
            vk::CullModeFlagBits::eBack,
            vk::FrontFace::eClockwise,
            VK_FALSE,
            0.f, 0.f, 0.f, 1.0f
        );

        //multisampling
        const vk::PipelineMultisampleStateCreateInfo multisampleInfo(
            vk::PipelineMultisampleStateCreateFlags(),
            vk::SampleCountFlagBits::e1,
            VK_FALSE
        );

        //color blend
        vk::PipelineColorBlendAttachmentState colorBlendAttachment;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        const vk::PipelineColorBlendStateCreateInfo colorBlendInfo(
            vk::PipelineColorBlendStateCreateFlags(),
            VK_FALSE,
            vk::LogicOp::eCopy,
            1,
            &colorBlendAttachment,
            { 0, 0, 0, 0 }
        );

        m_Layout = CreateLayout(swapchainInfo.device);
        m_RenderPass = CreateRenderPass(swapchainInfo.device, m_Swapchain.m_Format);

        const vk::GraphicsPipelineCreateInfo createInfo(
            vk::PipelineCreateFlagBits(),
            shaderStages.size(),
            shaderStages.data(),
            &vertexInfo,
            &assemblyInfo,
            nullptr,
            &viewportInfo,
            &rasterizationInfo,
            &multisampleInfo,
            nullptr,
            &colorBlendInfo,
            nullptr,
            m_Layout,
            m_RenderPass,
            0,
            nullptr
        );

        m_GraphicsPipeline = swapchainInfo.device.createGraphicsPipeline(nullptr, createInfo).value;

        swapchainInfo.device.destroyShaderModule(vertex);
        swapchainInfo.device.destroyShaderModule(fragment);
        
        m_Swapchain.MakeFrames(swapchainInfo.device, m_RenderPass);
    }
    VulkanPipeline::VulkanPipeline(const VulkanPipeline& other)
    {
        m_Queues.graphicsQueue = other.m_Queues.graphicsQueue;
        m_Queues.presentQueue = other.m_Queues.presentQueue;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = other.m_Swapchain;
    }
    VulkanPipeline::VulkanPipeline(VulkanPipeline&& other) noexcept
    {
        m_Queues.graphicsQueue = other.m_Queues.graphicsQueue;
        m_Queues.presentQueue = other.m_Queues.presentQueue;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = std::forward<VulkanSwapchain>(other.m_Swapchain);

        other.Reset();
    }
    VulkanPipeline& VulkanPipeline::operator=(const VulkanPipeline& other)
    {
        if(this == &other)
            return *this;

        m_Queues.graphicsQueue = other.m_Queues.graphicsQueue;
        m_Queues.presentQueue = other.m_Queues.presentQueue;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = other.m_Swapchain;

        return *this;
    }
    VulkanPipeline& VulkanPipeline::operator=(VulkanPipeline&& other) noexcept
    {
        m_Queues.graphicsQueue = other.m_Queues.graphicsQueue;
        m_Queues.presentQueue = other.m_Queues.presentQueue;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = std::forward<VulkanSwapchain>(other.m_Swapchain);

        other.Reset();

        return *this;
    }
    void VulkanPipeline::Reset() noexcept
    {
        m_Queues.graphicsQueue = nullptr;
        m_Queues.presentQueue = nullptr;
        m_GraphicsPipeline = nullptr;
        m_Layout = nullptr;
        m_RenderPass = nullptr;
        m_Swapchain.Reset();
    }
    void VulkanPipeline::Cleanup(const vk::Device& device) const noexcept
    {
        m_Swapchain.Cleanup(device);
        device.destroyPipelineLayout(m_Layout);
        device.destroyRenderPass(m_RenderPass);
        device.destroyPipeline(m_GraphicsPipeline);
    }
    void VulkanPipeline::RecordDrawCommands(const vk::CommandBuffer& commandBuffer, const Types::uint& imageIndex) const
    {
        const vk::CommandBufferBeginInfo commandBufferBeginInfo{};

        commandBuffer.begin(commandBufferBeginInfo);

        const vk::ClearValue clearColor(/*std::array<float, 4>*/{ 1.0f, 0.5f, 0.25f, 1.0f });
        const vk::RenderPassBeginInfo renderPassBeginInfo(
            m_RenderPass,
            m_Swapchain.m_Frames[imageIndex].framebuffer,
            vk::Rect2D({ 0, 0 }, m_Swapchain.m_Extent),
            1,
            &clearColor
        );

        commandBuffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_GraphicsPipeline);

        commandBuffer.draw(3, 1, 0, 0);//hardcode

        commandBuffer.endRenderPass();
        commandBuffer.end();
    }

    void VulkanPipeline::Render(const vk::Device& device) const
    {
        GE_CORE_CLASS_ASSERT(device.waitForFences(1, &m_Swapchain.m_Sync.m_InFlightFence, VK_TRUE, UINT64_MAX) == vk::Result::eSuccess,
            "cannot wait for fence");
        GE_CORE_CLASS_ASSERT(device.resetFences(1, &m_Swapchain.m_Sync.m_InFlightFence) == vk::Result::eSuccess,
            "cannot reset fence");

        const Types::uint imageIndex =
            device.acquireNextImageKHR(m_Swapchain.m_Swapchain, UINT64_MAX, m_Swapchain.m_Sync.m_ImageAvailable, nullptr).value;

        const auto commandBuffer = m_Swapchain.m_Frames[imageIndex].commandBuffer;
        commandBuffer.reset();

        RecordDrawCommands(commandBuffer, imageIndex);

        vk::Semaphore waitSemaphores[] = { m_Swapchain.m_Sync.m_ImageAvailable };
        const vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
        const vk::Semaphore signalSemaphores[] = { m_Swapchain.m_Sync.m_RenderFinished };

        const vk::SubmitInfo submitInfo(
            1,
            waitSemaphores,
            waitStages,
            1,
            &commandBuffer,
            1,
            signalSemaphores
        );

        m_Queues.graphicsQueue.submit(submitInfo, m_Swapchain.m_Sync.m_InFlightFence);

        const vk::SwapchainKHR swapchains[] = {m_Swapchain.m_Swapchain};

        const vk::PresentInfoKHR presentInfo(
            1,
            signalSemaphores,
            1,
            swapchains,
            &imageIndex
        );

        GE_CORE_CLASS_ASSERT(m_Queues.presentQueue.presentKHR(presentInfo) == vk::Result::eSuccess, "cannot present");
    }

    vk::PipelineLayout VulkanPipeline::CreateLayout(const vk::Device& device)
    {
        constexpr vk::PipelineLayoutCreateInfo layoutInfo(
        vk::PipelineLayoutCreateFlags(),
        0,
        nullptr,
        0
        );
        return device.createPipelineLayout(layoutInfo);
    }
    vk::RenderPass VulkanPipeline::CreateRenderPass(const vk::Device& device, const vk::Format& swapchainImageFormat)
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

        const vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
        const vk::SubpassDescription subpassDescription(
            vk::SubpassDescriptionFlags(),
            vk::PipelineBindPoint::eGraphics,
            0,
            nullptr,
            1,
            &colorAttachmentRef
        );
        const vk::RenderPassCreateInfo info(
            vk::RenderPassCreateFlags(),
            1,
            &colorAttachment,
            1,
            &subpassDescription
        );

        return device.createRenderPass(info);
    }
    vk::Queue VulkanPipeline::GetGraphicsQueue(const vk::Device& device, const VulkanQueueFamilyIndices& indices) noexcept
    {
        return device.getQueue(indices.graphicsFamily.value(), 0);
    }
    vk::Queue VulkanPipeline::GetPresentQueue(const vk::Device& device, const VulkanQueueFamilyIndices& indices) noexcept
    {
        return device.getQueue(indices.presentFamily.value(), 0);
    }
}