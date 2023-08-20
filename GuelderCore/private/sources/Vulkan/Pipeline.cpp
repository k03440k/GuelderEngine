module;
#include "../../headers/Core/GObject/GClass.hpp"
#include "GuelderEngine/Utils/Debug.hpp"
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
module GuelderEngine.Vulkan;
import :Pipeline;

import :Manager;
import :ShaderManager;
import :VulkanSwapchain;
import :Model;
import :Mesh;
import :VertexBuffer;
import GuelderEngine.Core;
import GuelderEngine.Core.Types;

import <vector>;

//ctors
namespace GuelderEngine::Vulkan
{
    Pipeline::Pipeline(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface,
        const vk::Extent2D& extent, const QueueFamilyIndices& queueFamilyIndices, const std::string_view& vertexPath, const std::string_view& fragmentPath, const Mesh_t& mesh,
        const Types::uint& inPosLocation, const Types::uint& inColorLocation)
        : m_Swapchain(device, physicalDevice, surface, extent, queueFamilyIndices)
    {
        m_Queues.graphics = GetGraphicsQueue(device, queueFamilyIndices);
        m_Queues.present = Getpresent(device, queueFamilyIndices);

        m_ShaderManager = ShaderManager(vertexPath, fragmentPath, inPosLocation, inColorLocation);

        Create(device, extent, vertexPath, fragmentPath);
        m_Swapchain.MakeFrames(device, m_RenderPass);

        m_VBuffer = VertexBuffer(device, physicalDevice, mesh);
    }
    Pipeline::Pipeline(const Pipeline& other)
    {
        m_Queues.graphics = other.m_Queues.graphics;
        m_Queues.present = other.m_Queues.present;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = other.m_Swapchain;
        m_ShaderManager = other.m_ShaderManager;
        m_VBuffer = other.m_VBuffer;
    }
    Pipeline::Pipeline(Pipeline&& other) noexcept
    {
        m_Queues.graphics = other.m_Queues.graphics;
        m_Queues.present = other.m_Queues.present;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = std::forward<Swapchain>(other.m_Swapchain);
        m_ShaderManager = std::forward<ShaderManager>(other.m_ShaderManager);
        m_VBuffer = std::forward<VertexBuffer>(other.m_VBuffer);

        other.Reset();
    }
    Pipeline& Pipeline::operator=(const Pipeline& other)
    {
        if(this == &other)
            return *this;

        m_Queues.graphics = other.m_Queues.graphics;
        m_Queues.present = other.m_Queues.present;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = other.m_Swapchain;
        m_ShaderManager = other.m_ShaderManager;
        m_VBuffer = other.m_VBuffer;

        return *this;
    }
    Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
    {
        m_Queues.graphics = other.m_Queues.graphics;
        m_Queues.present = other.m_Queues.present;
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
        m_Swapchain = std::forward<Swapchain>(other.m_Swapchain);
        m_ShaderManager = std::forward<ShaderManager>(other.m_ShaderManager);
        m_VBuffer = std::forward<VertexBuffer>(other.m_VBuffer);

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void Pipeline::Create(const vk::Device& device, const vk::Extent2D& extent, const std::string_view& vertexPath, const std::string_view& fragmentPath)
    {
        const auto bindingDescription = Vertex::GetBindingDescription();
        const auto attributeDescriptions = Vertex::GetAttributeDescriptions();

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
        const vk::ShaderModule vertex = ShaderManager::CreateModule(Utils::ResourceManager::GetFileSource(vertexPath), device);
        const vk::PipelineShaderStageCreateInfo vertexShaderInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertex, "main");
        shaderStages.push_back(vertexShaderInfo);
        //fragment shader
        const vk::ShaderModule fragment = ShaderManager::CreateModule(Utils::ResourceManager::GetFileSource(fragmentPath), device);
        const vk::PipelineShaderStageCreateInfo fragmentShaderInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragment, "main");
        shaderStages.push_back(fragmentShaderInfo);

        //viewport
        const vk::Viewport viewport(0.0f, 0.0f, extent.width, extent.height, 0.0f, 1.0f);
        const vk::Rect2D scissor({ 0, 0 }, extent);

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
    void Pipeline::Reset() noexcept
    {
        m_Queues.graphics = nullptr;
        m_Queues.present = nullptr;
        m_GraphicsPipeline = nullptr;
        m_Layout = nullptr;
        m_RenderPass = nullptr;
        m_Swapchain.Reset();
        m_ShaderManager.Reset();
        m_VBuffer.Reset();
    }
    void Pipeline::Cleanup(const vk::Device& device) const noexcept
    {
        m_VBuffer.Cleanup(device);
        m_Swapchain.Cleanup(device);
        device.destroyPipelineLayout(m_Layout);
        device.destroyRenderPass(m_RenderPass);
        device.destroyPipeline(m_GraphicsPipeline);
    }

    void Pipeline::SetMesh(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const Mesh_t& mesh)
    {
        m_VBuffer = VertexBuffer(device, physicalDevice, mesh);
    }

    vk::PipelineLayout Pipeline::CreateLayout(const vk::Device& device)
    {
        constexpr vk::PushConstantRange constantInfo(vk::ShaderStageFlagBits::eVertex, 0, sizeof(Model));
        constexpr vk::PipelineLayoutCreateInfo layoutInfo(
            vk::PipelineLayoutCreateFlags(),
            0,
            nullptr,
            1,//add in ctor this value?
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
        return device.getQueue(indices.graphicsFamily.value(), 0);
    }
    vk::Queue Pipeline::Getpresent(const vk::Device& device, const QueueFamilyIndices& indices) noexcept
    {
        return device.getQueue(indices.presentFamily.value(), 0);
    }

    void Pipeline::RecordDrawCommands(const vk::CommandBuffer& commandBuffer, const Types::uint& imageIndex, const Scene& scene) const
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

        m_VBuffer.Bind(commandBuffer, { 0 });

        for(auto&& position : scene.GetTrianglesPositions())
        {
            const auto model = glm::translate(glm::mat4(1.0f), position);
            const auto vulkanModel = Model(model);
            commandBuffer.pushConstants(m_Layout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(vulkanModel), &vulkanModel);

            commandBuffer.draw(m_VBuffer.GetVerticesCount(), 1, 0, 0);
        }

        commandBuffer.endRenderPass();
        commandBuffer.end();
    }
    void Pipeline::Render(const vk::Device& device, const vk::PhysicalDevice& physicalDevice,  const vk::SurfaceKHR& surface, const vk::Extent2D& extent,
        const QueueFamilyIndices& queueFamilyIndices, const Scene& scene)
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

        RecordDrawCommands(commandBuffer, imageIndex, scene);

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

        if(presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR)
        {
            Recreate( device, physicalDevice, surface,  extent, queueFamilyIndices );
            return;
        }

        //m_Swapchain.m_CurrentFrameNumber = (m_Swapchain.GetCurrentFrameNumber() + 1) % m_Swapchain.m_MaxFramesInFlight;
        m_Swapchain.IncrementCurrentFrame();
    }

    void Pipeline::Recreate(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::Extent2D& extent,
        const QueueFamilyIndices& queueFamilyIndices)
    {
        device.waitIdle();

        /*Cleanup(device);
        m_Swapchain.Reset();
        m_Swapchain = Swapchain(device, physicalDevice, surface, extent, queueFamilyIndices);

        m_Queues.graphics = GetGraphicsQueue(device, queueFamilyIndices);
        m_Queues.present = Getpresent(device, queueFamilyIndices);

        Create(device, m_ShaderManager.GetVertexPath(), m_ShaderManager.GetFragmentPath());
        m_Swapchain.MakeFrames(device, m_RenderPass);*/
        //TODO: find out why lower recreation didn't work(maybe sync objects)
        device.destroyPipelineLayout(m_Layout);
        device.destroyRenderPass(m_RenderPass);
        device.destroyPipeline(m_GraphicsPipeline);

        Create(device, extent, m_ShaderManager.GetVertexPath(), m_ShaderManager.GetFragmentPath());//possible problem in extent

        m_Swapchain.Recreate( device, physicalDevice, surface, m_RenderPass, extent, queueFamilyIndices );
    }
}