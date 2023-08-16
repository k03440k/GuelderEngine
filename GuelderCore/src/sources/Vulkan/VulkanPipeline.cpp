module;
#include "../../headers/Core/GObject/GClass.hpp"
#include "GuelderEngine/Utils/Debug.hpp"
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
module GuelderEngine.Vulkan;
import :VulkanPipeline;

import :VulkanShaderManager;
import :VulkanSwapchain;
import :VulkanModel;
import :VulkanManager;
import GuelderEngine.Core;
import GuelderEngine.Core.Types;

import <vector>;

//ctors
namespace GuelderEngine::Vulkan
{
    VulkanPipeline::VulkanPipeline(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
        const Types::uint& width, const Types::uint& height, const VulkanQueueFamilyIndices& queueFamilyIndices, const std::string_view& vertexPath, const std::string_view& fragmentPath)
        : m_Swapchain(device, physicalDevice, surface, {width, height}, queueFamilyIndices)
    {
        m_Queues.graphicsQueue = GetGraphicsQueue(device, queueFamilyIndices);
        m_Queues.presentQueue = GetPresentQueue(device, queueFamilyIndices);

        m_ShaderManager = VulkanShaderManager(vertexPath, fragmentPath);

        Create(device, vertexPath, fragmentPath);
        m_Swapchain.MakeFrames(device, m_RenderPass);
    }
    VulkanPipeline::VulkanPipeline(const VulkanPipeline& other)
    {
        m_Queues.graphicsQueue = other.m_Queues.graphicsQueue;
        m_Queues.presentQueue = other.m_Queues.presentQueue;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = other.m_Swapchain;
        m_ShaderManager = other.m_ShaderManager;
    }
    VulkanPipeline::VulkanPipeline(VulkanPipeline&& other) noexcept
    {
        m_Queues.graphicsQueue = other.m_Queues.graphicsQueue;
        m_Queues.presentQueue = other.m_Queues.presentQueue;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = std::forward<VulkanSwapchain>(other.m_Swapchain);
        m_ShaderManager = std::forward<VulkanShaderManager>(other.m_ShaderManager);

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
        m_ShaderManager = other.m_ShaderManager;

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
        m_ShaderManager = std::forward<VulkanShaderManager>(other.m_ShaderManager);

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void VulkanPipeline::Create(const vk::Device& device, const std::string_view& vertexPath, const std::string_view& fragmentPath)
    {
        constexpr vk::PipelineVertexInputStateCreateInfo vertexInfo(vk::PipelineVertexInputStateCreateFlags(), 0, nullptr, 0);
        constexpr vk::PipelineInputAssemblyStateCreateInfo assemblyInfo(vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList);

        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
        shaderStages.reserve(2);

        //vertex shader
        const vk::ShaderModule vertex = VulkanShaderManager::CreateModule(Utils::ResourceManager::GetFileSource(vertexPath), device);
        const vk::PipelineShaderStageCreateInfo vertexShaderInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertex, "main");
        shaderStages.push_back(vertexShaderInfo);
        //fragment shader
        const vk::ShaderModule fragment = VulkanShaderManager::CreateModule(Utils::ResourceManager::GetFileSource(fragmentPath), device);
        const vk::PipelineShaderStageCreateInfo fragmentShaderInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragment, "main");
        shaderStages.push_back(fragmentShaderInfo);

        //viewport
        const vk::Viewport viewport(0.0f, 0.0f, m_Swapchain.m_Extent.width, m_Swapchain.m_Extent.height, 0.0f, 1.0f);
        const vk::Rect2D scissor({ 0, 0 }, m_Swapchain.m_Extent);

        vk::PipelineViewportStateCreateInfo viewportInfo(
            vk::PipelineViewportStateCreateFlags(),
            1,          //viewports
            &viewport,  //viewports
            1,          //scissors
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
        colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
        colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
        colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
        colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;
        const vk::PipelineColorBlendStateCreateInfo colorBlendInfo(
            vk::PipelineColorBlendStateCreateFlags(),
            VK_FALSE,
            vk::LogicOp::eCopy,
            1,
            &colorBlendAttachment,
            { 0, 0, 0, 0 }
        );

        m_Layout = CreateLayout(device);
        m_RenderPass = CreateRenderPass(device, m_Swapchain.m_Format);

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

        m_GraphicsPipeline = device.createGraphicsPipeline(nullptr, createInfo).value;

        device.destroyShaderModule(vertex);
        device.destroyShaderModule(fragment);
    }
    void VulkanPipeline::Reset() noexcept
    {
        m_Queues.graphicsQueue = nullptr;
        m_Queues.presentQueue = nullptr;
        m_GraphicsPipeline = nullptr;
        m_Layout = nullptr;
        m_RenderPass = nullptr;
        m_Swapchain.Reset();
        m_ShaderManager.Reset();
    }
    void VulkanPipeline::Cleanup(const vk::Device& device) const noexcept
    {
        m_Swapchain.Cleanup(device);
        device.destroyPipelineLayout(m_Layout);
        device.destroyRenderPass(m_RenderPass);
        device.destroyPipeline(m_GraphicsPipeline);
    }

    vk::PipelineLayout VulkanPipeline::CreateLayout(const vk::Device& device)
    {
        constexpr vk::PushConstantRange constantInfo(vk::ShaderStageFlagBits::eVertex, 0, sizeof(VulkanModel));
        constexpr vk::PipelineLayoutCreateInfo layoutInfo(
            vk::PipelineLayoutCreateFlags(),
            0,
            nullptr,
            1,//add in ctor this value?
            &constantInfo
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

    void VulkanPipeline::RecordDrawCommands(const vk::CommandBuffer& commandBuffer, const Types::uint& imageIndex, const VulkanScene& scene) const
    {
        const vk::CommandBufferBeginInfo commandBufferBeginInfo{};

        commandBuffer.begin(commandBufferBeginInfo);

        const float greenValue = (sin(glfwGetTime()) / 2.0f) + 0.5f;

        const vk::ClearValue clearValue{{ 1.0f, greenValue, 0.25f, 1.0f }};
        const vk::RenderPassBeginInfo renderPassBeginInfo(
            m_RenderPass,
            m_Swapchain.m_Frames[imageIndex].framebuffer,
            vk::Rect2D({ 0, 0 }, m_Swapchain.m_Extent),
            1,
            &clearValue
        );

        commandBuffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_GraphicsPipeline);

        for(auto&& position : scene.GetTrianglesPositions())
        {
            const auto model = glm::translate(glm::mat4(1.0f), position);
            const auto vulkanModel = VulkanModel(model);
            commandBuffer.pushConstants(m_Layout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(vulkanModel), &vulkanModel);

            commandBuffer.draw(3, 1, 0, 0);//hardcode
        }

        commandBuffer.endRenderPass();
        commandBuffer.end();
    }
    void VulkanPipeline::Render(const vk::Device& device, const vk::PhysicalDevice& physicalDevice,  const vk::SurfaceKHR& surface, const vk::Extent2D& extent,
        const VulkanQueueFamilyIndices& queueFamilyIndices, const VulkanScene& scene)
    {
        const auto& currentFrame = m_Swapchain.m_Frames[m_Swapchain.m_CurrentFrameNumber];

        //GE_CORE_CLASS_ASSERT(info.device.waitForFences(1, &currentFrame.sync.m_InFlightFence, VK_TRUE, UINT64_MAX) == vk::Result::eSuccess, "cannot wait for fence");
        currentFrame.WaitForImage(device);

        Types::uint imageIndex;
        try
        {
            const auto result = device.acquireNextImageKHR(m_Swapchain.m_Swapchain, UINT64_MAX, currentFrame.sync.m_ImageAvailable, nullptr);
            imageIndex = result.value;
        }
        catch(const vk::OutOfDateKHRError error)
        {
            Recreate( device, physicalDevice, surface, extent, queueFamilyIndices);
            return;
        }

        //const Types::uint imageIndex = acquire.value;

        const auto& commandBuffer = currentFrame.commandBuffer;
        commandBuffer.reset();

        RecordDrawCommands(commandBuffer, imageIndex, scene);

        const vk::Semaphore waitSemaphores[] = { currentFrame.sync.m_ImageAvailable };
        const vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
        const vk::Semaphore signalSemaphores[] = { currentFrame.sync.m_RenderFinished };

        const vk::SubmitInfo submitInfo(
            1,
            waitSemaphores,
            waitStages,
            1,
            &commandBuffer,
            1,
            signalSemaphores
        );
        currentFrame.ResetFence(device);

        //GE_CORE_CLASS_ASSERT(info.device.resetFences(1, &currentFrame.sync.m_InFlightFence) == vk::Result::eSuccess, "cannot reset fence");

        m_Queues.graphicsQueue.submit(submitInfo, currentFrame.sync.m_InFlightFence);

        const vk::SwapchainKHR swapchains[] = { m_Swapchain.m_Swapchain };

        const vk::PresentInfoKHR presentInfo(
            1,
            signalSemaphores,
            1,
            swapchains,
            &imageIndex
        );

        //GE_CORE_CLASS_ASSERT(m_Queues.presentQueue.presentKHR(presentInfo) == vk::Result::eSuccess, "cannot present");

        vk::Result presentResult;
        try
        {
            presentResult = m_Queues.presentQueue.presentKHR(presentInfo);
        }
        catch(const vk::OutOfDateKHRError& error)
        {
            presentResult = vk::Result::eErrorOutOfDateKHR;
        }

        if(presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR)
        {
            Recreate( device, physicalDevice, surface,  extent, queueFamilyIndices );
            return;
        }

        m_Swapchain.m_CurrentFrameNumber = (m_Swapchain.m_CurrentFrameNumber + 1) % m_Swapchain.m_MaxFramesInFlight;
    }

    void VulkanPipeline::Recreate(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Extent2D& extent,
        const VulkanQueueFamilyIndices& queueFamilyIndices)
    {
        device.waitIdle();

        GE_LOG(VulkanCore, Info, "Recreating Swapchain");

        Cleanup(device);
        m_Swapchain.Reset();
        m_Swapchain = VulkanSwapchain(device, physicalDevice, surface, extent, queueFamilyIndices);

        m_Queues.graphicsQueue = GetGraphicsQueue(device, queueFamilyIndices);
        m_Queues.presentQueue = GetPresentQueue(device, queueFamilyIndices);

        Create(device, m_ShaderManager.GetVertexPath(), m_ShaderManager.GetFragmentPath());
        m_Swapchain.MakeFrames(device, m_RenderPass);
        //TODO: find out why lower recreation didn't work(maybe sync objects)
        /*device.destroyPipelineLayout(m_Layout);
        device.destroyRenderPass(m_RenderPass);
        device.destroyPipeline(m_GraphicsPipeline);

        Create( device, m_ShaderManager.GetVertexPath(), m_ShaderManager.GetFragmentPath());

        m_Swapchain.Recreate( device, physicalDevice, surface, m_RenderPass, extent, queueFamilyIndices );*/
    }
}