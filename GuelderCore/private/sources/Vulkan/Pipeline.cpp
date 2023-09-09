module;
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
module GuelderEngine.Vulkan;
import :Pipeline;

import :Manager;
import :ShaderManager;
import :Swapchain;
import :Mesh;

import :VertexBuffer;
import :IndexBuffer;
import :StagingBuffer;

import GuelderEngine.Core;
import GuelderEngine.Core.Types;

import <vector>;

//ctors
namespace GuelderEngine::Vulkan
{
    Pipeline::Pipeline(
        const vk::Device& device, 
        const vk::PhysicalDevice& physicalDevice,
        const vk::SurfaceKHR& surface,
        const vk::Extent2D& extent,
        const QueueFamilyIndices& queueFamilyIndices,
        const ShaderInfo& shaderInfo
    )
        : m_Swapchain(device, physicalDevice, surface, extent, queueFamilyIndices)
    {
        m_Queues.graphics = GetGraphicsQueue(device, queueFamilyIndices);
        m_Queues.present = GetPresentQueue(device, queueFamilyIndices);
        m_Queues.transfer = GetTransferQueue(device, queueFamilyIndices);

        m_ShaderManager = ShaderManager(shaderInfo);

        Create(device, shaderInfo);
        m_Swapchain.MakeFrames(device, m_RenderPass);

        SetMesh(device, physicalDevice, queueFamilyIndices, {});
    }
    Pipeline::Pipeline(const Pipeline& other)
    {
        m_Queues.graphics = other.m_Queues.graphics;
        m_Queues.present = other.m_Queues.present;
        m_Queues.transfer = other.m_Queues.transfer;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = other.m_Swapchain;
        m_ShaderManager = other.m_ShaderManager;
        m_VBuffer = other.m_VBuffer;
        m_IBuffer = other.m_IBuffer;
    }
    Pipeline::Pipeline(Pipeline&& other) noexcept
    {
        m_Queues.graphics = other.m_Queues.graphics;
        m_Queues.present = other.m_Queues.present;
        m_Queues.transfer = other.m_Queues.transfer;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = std::forward<Swapchain>(other.m_Swapchain);
        m_ShaderManager = std::forward<ShaderManager>(other.m_ShaderManager);
        m_VBuffer = std::forward<Buffers::VertexBuffer>(other.m_VBuffer);
        m_IBuffer = other.m_IBuffer;

        other.Reset();
    }
    Pipeline& Pipeline::operator=(const Pipeline& other)
    {
        if(this == &other)
            return *this;

        m_Queues.graphics = other.m_Queues.graphics;
        m_Queues.present = other.m_Queues.present;
        m_Queues.transfer = other.m_Queues.transfer;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = other.m_Swapchain;
        m_ShaderManager = other.m_ShaderManager;
        m_VBuffer = other.m_VBuffer;
        m_IBuffer = other.m_IBuffer;

        return *this;
    }
    Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
    {
        m_Queues.graphics = other.m_Queues.graphics;
        m_Queues.present = other.m_Queues.present;
        m_Queues.transfer = other.m_Queues.transfer;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = std::forward<Swapchain>(other.m_Swapchain);
        m_ShaderManager = std::forward<ShaderManager>(other.m_ShaderManager);
        m_VBuffer = std::forward<Buffers::VertexBuffer>(other.m_VBuffer);
        m_IBuffer = other.m_IBuffer;

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void Pipeline::Create(const vk::Device& device, const ShaderInfo& shaderInfo)
    {
        const auto bindingDescription = Vertex::GetBindingDescription();
        const auto attributeDescriptions = Vertex::GetAttributeDescriptions(shaderInfo.info);

        const vk::PipelineVertexInputStateCreateInfo vertexInfo{vk::PipelineVertexInputStateCreateFlags(),
            1,
            &bindingDescription,
            attributeDescriptions.size(),
            attributeDescriptions.data()
        };
        const vk::PipelineInputAssemblyStateCreateInfo assemblyInfo(vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList);

        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
        shaderStages.reserve(2);

        //vertex shader
        const vk::ShaderModule vertex = ShaderManager::CreateModule(Utils::ResourceManager::GetFileSource(shaderInfo.vertexPath), device);
        const vk::PipelineShaderStageCreateInfo vertexShaderInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertex, "main");
        shaderStages.push_back(vertexShaderInfo);
        //fragment shader
        const vk::ShaderModule fragment = ShaderManager::CreateModule(Utils::ResourceManager::GetFileSource(shaderInfo.fragmentPath), device);
        const vk::PipelineShaderStageCreateInfo fragmentShaderInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragment, "main");
        shaderStages.push_back(fragmentShaderInfo);

        vk::PipelineViewportStateCreateInfo viewportInfo(
            vk::PipelineViewportStateCreateFlags(),
            1,          //viewports
            nullptr,  //viewports
            1,          //scissors
            nullptr    //scissors
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

        //color blending
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
        m_RenderPass = CreateRenderPass(device, m_Swapchain.GetFormat());

        const std::vector<vk::DynamicState> dynamicStateEnables{vk::DynamicState::eViewport, vk::DynamicState::eScissor};
        vk::PipelineDynamicStateCreateInfo dynamicStateInfo{};
        dynamicStateInfo.pDynamicStates = dynamicStateEnables.data();
        dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());

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
            &dynamicStateInfo,
            m_Layout,
            m_RenderPass,
            0,
            nullptr
        );

        m_GraphicsPipeline = device.createGraphicsPipeline(nullptr, createInfo).value;

        device.destroyShaderModule(vertex);
        device.destroyShaderModule(fragment);
    }
    void Pipeline::Reset() noexcept
    {
        m_Queues.graphics = nullptr;
        m_Queues.present = nullptr;
        m_Queues.transfer = nullptr;
        m_GraphicsPipeline = nullptr;
        m_Layout = nullptr;
        m_RenderPass = nullptr;
        m_Swapchain.Reset();
        m_VBuffer.Reset();
        m_IBuffer.Reset();
    }
    void Pipeline::Cleanup(const vk::Device& device) const noexcept
    {
        m_VBuffer.Cleanup(device);
        m_IBuffer.Cleanup(device);
        m_Swapchain.Cleanup(device);
        device.destroyPipelineLayout(m_Layout);
        device.destroyRenderPass(m_RenderPass);
        device.destroyPipeline(m_GraphicsPipeline);
    }

    void Pipeline::SetMesh(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& queueFamilyIndices, const Mesh& mesh)
    {
        if(mesh.GetVertices().size())
        {
            m_VBuffer = Buffers::VertexBuffer(device, physicalDevice, queueFamilyIndices, m_Swapchain.GetCommandPoolTransfer().GetCommandPool(), m_Queues.transfer, mesh.GetVertices());
            m_IBuffer = Buffers::IndexBuffer(device, physicalDevice, queueFamilyIndices, m_Swapchain.GetCommandPoolTransfer().GetCommandPool(), m_Queues.transfer, mesh.GetIndices());
        }
    }
    void Pipeline::SetShaderInfo(const vk::Device& device, const ShaderInfo& shaderInfo)
    {
        m_ShaderManager = ShaderManager(shaderInfo);

        Create(device, shaderInfo);
    }

    vk::PipelineLayout Pipeline::CreateLayout(const vk::Device& device)
    {
        constexpr vk::PushConstantRange constantInfo(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(Vertex));
        constexpr vk::PipelineLayoutCreateInfo layoutInfo(
            vk::PipelineLayoutCreateFlags(),
            0,
            nullptr,
            1,
            &constantInfo
        );
        return device.createPipelineLayout(layoutInfo);
    }
    vk::RenderPass Pipeline::CreateRenderPass(const vk::Device& device, const vk::Format& swapchainImageFormat)
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

    vk::Queue Pipeline::GetGraphicsQueue(const vk::Device& device, const QueueFamilyIndices& indices) noexcept
    {
        return device.getQueue(indices.GetGraphicsFamily(), 0);
    }
    vk::Queue Pipeline::GetPresentQueue(const vk::Device& device, const QueueFamilyIndices& indices) noexcept
    {
        return device.getQueue(indices.GetPresentFamily(), 0);
    }
    vk::Queue Pipeline::GetTransferQueue(const vk::Device& device, const QueueFamilyIndices& indices) noexcept
    {
        return device.getQueue(indices.GetTransferFamily(), 0);
    }

    void Pipeline::RecordDrawCommands(const vk::CommandBuffer& commandBuffer, const Types::uint& imageIndex) const
    {
        const vk::CommandBufferBeginInfo commandBufferBeginInfo{};

        commandBuffer.begin(commandBufferBeginInfo);

        const float greenValue = (sin(glfwGetTime()) / 2.0f) + 0.5f;

        const vk::ClearValue clearValue{{ 1.0f, greenValue, 0.25f, 1.0f }};
        const vk::RenderPassBeginInfo renderPassBeginInfo(
            m_RenderPass,
            m_Swapchain.GetFrames()[imageIndex].framebuffer,
            vk::Rect2D({ 0, 0 }, m_Swapchain.GetExtent2D()),
            1,
            &clearValue
        );

        commandBuffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_GraphicsPipeline);

        const vk::Viewport viewport{
            0.0f,
            0.0f,
            static_cast<float>(m_Swapchain.GetExtent2D().width),
            static_cast<float>(m_Swapchain.GetExtent2D().height),
            0.f,
            1.0f
        };
        const vk::Rect2D scissors{{0, 0}, m_Swapchain.GetExtent2D()};

        commandBuffer.setViewport(0, 1, &viewport);
        commandBuffer.setScissor(0, 1, &scissors);

        m_VBuffer.Bind(commandBuffer, { 0 });
        if(m_IBuffer.GetIndicesCount())
            m_IBuffer.Bind(commandBuffer, { 0 });

        //TODO: add possibility to push custom constant data
        struct SimplePushConstantData
        {
            glm::vec2 pos;
            alignas(16) glm::vec3 color;
        };
        std::vector v{ SimplePushConstantData(
            { sin(glfwGetTime()) / 5.0f * cos(glfwGetTime()) , sin(glfwGetTime()) / 8.0f * tan(glfwGetTime())},//idk how does it work
            { (sin(glfwGetTime()) / 5.0f) + 0.5f, (sin(glfwGetTime()) / 2.0f) + 0.5f, (sin(glfwGetTime()) / 2.0f) + 0.5f }) };

        for(auto&& ve : v)
        {
            //const auto model = glm::translate(glm::mat4(1.0f), position);
            const auto vulkanModel = ve;
            commandBuffer.pushConstants(m_Layout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(vulkanModel), &vulkanModel);
            
            if(m_IBuffer.GetIndicesCount())
                commandBuffer.drawIndexed(m_IBuffer.GetIndicesCount(), 1, 0, 0, 0);
            else
                commandBuffer.draw(m_VBuffer.GetVerticesCount(), 1, 0, 0);
        }

        commandBuffer.endRenderPass();
        commandBuffer.end();
    }
    void Pipeline::Render(const vk::Device& device, const vk::PhysicalDevice& physicalDevice,  const vk::SurfaceKHR& surface, const vk::Extent2D& extent, bool& wasWindowResized,
        const QueueFamilyIndices& queueFamilyIndices)
    {
        const auto& currentFrame = m_Swapchain.GetFrames()[m_Swapchain.GetCurrentFrameNumber()];

        //GE_CORE_CLASS_ASSERT(info.device.waitForFences(1, &currentFrame.sync.GetFlightFence(), VK_TRUE, UINT64_MAX) == vk::Result::eSuccess, "cannot wait for fence");
        currentFrame.WaitForImage(device);

        Types::uint imageIndex;
        try
        {
            const auto result = device.acquireNextImageKHR(m_Swapchain.GetSwapchain(), UINT64_MAX, currentFrame.sync.GetImageAvailableSemaphore(), nullptr);
            imageIndex = result.value;
        }
        catch(const vk::OutOfDateKHRError& error)
        {
            Recreate( device, physicalDevice, surface, extent, queueFamilyIndices);
            return;
        }

        //const Types::uint imageIndex = acquire.value;

        const auto& commandBuffer = currentFrame.commandBuffer;
        commandBuffer.reset();

        RecordDrawCommands(commandBuffer, imageIndex);

        const vk::Semaphore waitSemaphores[] = { currentFrame.sync.GetImageAvailableSemaphore() };
        const vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
        const vk::Semaphore signalSemaphores[] = { currentFrame.sync.GetImageRenderFinishedSemaphore() };

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

        //GE_CORE_CLASS_ASSERT(info.device.resetFences(1, &currentFrame.sync.GetFlightFence()) == vk::Result::eSuccess, "cannot reset fence");

        m_Queues.graphics.submit(submitInfo, currentFrame.sync.GetFlightFence());

        const vk::SwapchainKHR swapchains[] = { m_Swapchain.GetSwapchain() };

        const vk::PresentInfoKHR presentInfo(
            1,
            signalSemaphores,
            1,
            swapchains,
            &imageIndex
        );

        //GE_CORE_CLASS_ASSERT(m_Queues.present.presentKHR(presentInfo) == vk::Result::eSuccess, "cannot present");

        vk::Result presentResult;
        try
        {
            presentResult = m_Queues.present.presentKHR(presentInfo);
        }
        catch(const vk::OutOfDateKHRError& error)
        {
            presentResult = vk::Result::eErrorOutOfDateKHR;
        }

        if(presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || wasWindowResized)
        {
            Recreate(device, physicalDevice, surface,  extent, queueFamilyIndices);
            wasWindowResized = false;
            return;
        }

        //m_Swapchain.m_CurrentFrameNumber = (m_Swapchain.GetCurrentFrameNumber() + 1) % m_Swapchain.m_MaxFramesInFlight;
        m_Swapchain.IncrementCurrentFrame();
    }

    void Pipeline::Recreate(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Extent2D& extent,
        const QueueFamilyIndices& queueFamilyIndices)
    {
        device.waitIdle();
        
        device.destroyPipelineLayout(m_Layout);
        device.destroyRenderPass(m_RenderPass);
        device.destroyPipeline(m_GraphicsPipeline);

        Create(device, m_ShaderManager.GetShaderInfo());

        m_Swapchain.Recreate( device, physicalDevice, surface, m_RenderPass, extent, queueFamilyIndices );
    }
}