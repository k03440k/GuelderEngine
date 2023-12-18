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
    Pipeline::Pipeline(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo)
    {
        m_ShaderManager = ShaderManager(shaderInfo);

        Create(device, renderPass, shaderInfo);

        //SetMesh(device, physicalDevice, queueFamilyIndices, {});
    }
    /*Pipeline::Pipeline(const Pipeline& other)
    {
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_ShaderManager = other.m_ShaderManager;
    }*/
    Pipeline::Pipeline(Pipeline&& other) noexcept
    {
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_ShaderManager = std::forward<ShaderManager>(other.m_ShaderManager);

        other.Reset();
    }
    /*Pipeline& Pipeline::operator=(const Pipeline& other)
    {
        if(this == &other)
            return *this;
        
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_ShaderManager = other.m_ShaderManager;

        return *this;
    }*/
    Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
    {
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_ShaderManager = std::forward<ShaderManager>(other.m_ShaderManager);

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void Pipeline::Create(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo)
    {
        const auto bindingDescription = Vertex2D::GetBindingDescription();
        const auto attributeDescriptions = Vertex2D::GetAttributeDescriptions(shaderInfo.info);

        const vk::PipelineVertexInputStateCreateInfo vertexInfo{vk::PipelineVertexInputStateCreateFlags(),
            1,
            & bindingDescription,
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
            renderPass,
            0,
            nullptr
        );

        m_GraphicsPipeline = device.createGraphicsPipeline(nullptr, createInfo).value;

        device.destroyShaderModule(vertex);
        device.destroyShaderModule(fragment);
    }
    void Pipeline::Reset() noexcept
    {
        m_GraphicsPipeline = nullptr;
        m_Layout = nullptr;
    }
    void Pipeline::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyPipelineLayout(m_Layout);
        device.destroyPipeline(m_GraphicsPipeline);
    }

    /*void Pipeline::SetMesh(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& queueFamilyIndices, const Mesh2D& mesh)
    {
        if(mesh.GetVertices().size())
        {
            m_VBuffer = Buffers::VertexBuffer(device, physicalDevice, queueFamilyIndices, m_Swapchain.GetCommandPoolTransfer().GetCommandPool(), m_Queues.transfer, mesh.GetVertices());
            m_IBuffer = Buffers::IndexBuffer(device, physicalDevice, queueFamilyIndices, m_Swapchain.GetCommandPoolTransfer().GetCommandPool(), m_Queues.transfer, mesh.GetIndices());
        }
    }*/

    vk::PipelineLayout Pipeline::CreateLayout(const vk::Device& device)
    {
        constexpr vk::PushConstantRange constantInfo(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(SimplePushConstantData));
        constexpr vk::PipelineLayoutCreateInfo layoutInfo(
            vk::PipelineLayoutCreateFlags(),
            0,
            nullptr,
            1,
            &constantInfo
        );
        return device.createPipelineLayout(layoutInfo);
    }

    //void Pipeline::RecordDrawCommands(const vk::CommandBuffer& commandBuffer, const vk::RenderPass& renderPass, const uint& imageIndex, const Buffers::VertexBuffer& vertexBuffer,
    //    const Buffers::IndexBuffer& indexBuffer, const SimplePushConstantData& push) const
    //{
    //    const vk::CommandBufferBeginInfo commandBufferBeginInfo{};

    //    commandBuffer.begin(commandBufferBeginInfo);

    //    const float blueValue = (sin(glfwGetTime()) / 2.0f) + 0.5f;

    //    const vk::ClearValue clearValue{{ 0.25f, 0.25f, blueValue, 1.0f }};
    //    const vk::RenderPassBeginInfo renderPassBeginInfo(
    //        m_RenderPass,
    //        m_Swapchain.GetFrames()[imageIndex].framebuffer,
    //        vk::Rect2D({ 0, 0 }, m_Swapchain.GetExtent2D()),
    //        1,
    //        &clearValue
    //    );

    //    commandBuffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);
    //    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_GraphicsPipeline);

    //    const vk::Viewport viewport{
    //        0.0f,
    //            0.0f,
    //            static_cast<float>(m_Swapchain.GetExtent2D().width),
    //            static_cast<float>(m_Swapchain.GetExtent2D().height),
    //            0.f,
    //            1.0f
    //    };
    //    const vk::Rect2D scissors{{0, 0}, m_Swapchain.GetExtent2D()};

    //    commandBuffer.setViewport(0, 1, &viewport);
    //    commandBuffer.setScissor(0, 1, &scissors);

    //    //TODO: pass vbuffer and ibuffer with reference here. and delete m_VBuffer and m_IBuffer members
    //    //add vulkanmanager method "make vertex buffer" and "make index buffer" store object buffer data inside GEApplication
    //    vertexBuffer.Bind(commandBuffer, { 0 });
    //    if(indexBuffer.GetIndicesCount())
    //        indexBuffer.Bind(commandBuffer, { 0 });
    //    //auto* vbs = new vk::Buffer[vertexBuffers.size()];
    //    //auto* dos = new vk::DeviceSize[vertexBuffers.size()];
    //    //for(size_t i = 0; i < vertexBuffers.size(); i++)
    //    //{
    //    //    vbs[i] = vertexBuffers[i].GetBuffer();
    //    //    dos[i] = 0;
    //    //}
    //    //commandBuffer.bindVertexBuffers(0, vertexBuffers.size(), vbs, dos);
    //    //commandBuffer.bindIndexBuffer(indexBuffers[0].GetBuffer(), {}, vk::IndexType::eUint32, {0});
    //    //delete[] vbs;
    //    //delete[] dos;

    //    ////vertices count
    //    //size_t verticesCount = 0;
    //    //for(auto& vertex : vertexBuffers)
    //    //    verticesCount += vertex.GetVerticesCount();
    //    //size_t indiciesCount = 0;
    //    //for(auto& indicies : indexBuffers)
    //    //    indiciesCount += indicies.GetIndicesCount();

    //    commandBuffer.pushConstants(m_Layout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(push), &push);

    //    if(indexBuffer.GetIndicesCount())
    //        commandBuffer.drawIndexed(indexBuffer.GetIndicesCount(), 1, 0, 0, 0);
    //    else
    //        commandBuffer.draw(vertexBuffer.GetVerticesCount(), 1, 0, 0);

    //    commandBuffer.endRenderPass();
    //    commandBuffer.end();
    //}
    //void Pipeline::Render(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const vk::RenderPass& renderPass, const vk::Extent2D& extent, bool& wasWindowResized,
    //    const QueueFamilyIndices& queueFamilyIndices, const Vulkan::Buffers::VertexBuffer& vertexBuffer, const Buffers::IndexBuffer& indexBuffer, uint currentFrameNumber, const SimplePushConstantData& push)//TODO: remove vectors
    //{
    //    const auto& currentFrame = m_Swapchain.GetFrames()[m_Swapchain.GetCurrentFrameNumber()];

    //    //GE_CORE_CLASS_ASSERT(info.device.waitForFences(1, &currentFrame.sync.GetFlightFence(), VK_TRUE, UINT64_MAX) == vk::Result::eSuccess, "cannot wait for fence");
    //    currentFrame.WaitForImage(device);

    //    uint imageIndex;
    //    try
    //    {
    //        const auto result = device.acquireNextImageKHR(m_Swapchain.GetSwapchain(), UINT64_MAX, currentFrame.sync.GetImageAvailableSemaphore(), nullptr);
    //        imageIndex = result.value;
    //    }
    //    catch(const vk::OutOfDateKHRError& error)
    //    {
    //        Recreate(device, physicalDevice, surface, extent, queueFamilyIndices);
    //        return;
    //    }

    //    //const uint imageIndex = acquire.value;

    //    const auto& commandBuffer = currentFrame.commandBuffer;
    //    commandBuffer.reset();

    //    RecordDrawCommands(commandBuffer, renderPass, imageIndex, vertexBuffer, indexBuffer, push);

    //    const vk::Semaphore waitSemaphores[] = { currentFrame.sync.GetImageAvailableSemaphore() };
    //    const vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    //    const vk::Semaphore signalSemaphores[] = { currentFrame.sync.GetImageRenderFinishedSemaphore() };

    //    const vk::SubmitInfo submitInfo(
    //        1,
    //        waitSemaphores,
    //        waitStages,
    //        1,
    //        &commandBuffer,
    //        1,
    //        signalSemaphores
    //    );
    //    currentFrame.ResetFence(device);

    //    //GE_CORE_CLASS_ASSERT(info.device.resetFences(1, &currentFrame.sync.GetFlightFence()) == vk::Result::eSuccess, "cannot reset fence");

    //    m_Queues.graphics.submit(submitInfo, currentFrame.sync.GetFlightFence());

    //    const vk::SwapchainKHR swapchains[] = { m_Swapchain.GetSwapchain() };

    //    const vk::PresentInfoKHR presentInfo(
    //        1,
    //        signalSemaphores,
    //        1,
    //        swapchains,
    //        &imageIndex
    //    );

    //    //GE_CORE_CLASS_ASSERT(m_Queues.present.presentKHR(presentInfo) == vk::Result::eSuccess, "cannot present");

    //    vk::Result presentResult;
    //    try
    //    {
    //        presentResult = m_Queues.present.presentKHR(presentInfo);
    //    }
    //    catch(const vk::OutOfDateKHRError& error)
    //    {
    //        presentResult = vk::Result::eErrorOutOfDateKHR;
    //    }

    //    if(presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || wasWindowResized)
    //    {
    //        Recreate(device, physicalDevice, surface, extent, queueFamilyIndices);
    //        wasWindowResized = false;
    //        return;
    //    }

    //    //m_Swapchain.m_CurrentFrameNumber = (m_Swapchain.GetCurrentFrameNumber() + 1) % m_Swapchain.m_MaxFramesInFlight;
    //    //m_Swapchain.IncrementCurrentFrame();
    //}

    void Pipeline::Recreate(const vk::Device& device, const vk::RenderPass& renderPass)
    {
        device.waitIdle();

        device.destroyPipelineLayout(m_Layout);
        device.destroyPipeline(m_GraphicsPipeline);

        Create(device, renderPass, m_ShaderManager.GetShaderInfo());
    }

    void Pipeline::SetShaderInfo(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo)
    {
        m_ShaderManager = ShaderManager(shaderInfo);

        Create(device, renderPass, shaderInfo);
    }
    const vk::Pipeline& Pipeline::GetPipeline() const noexcept
    {
        return m_GraphicsPipeline;
    }
    const vk::PipelineLayout& Pipeline::GetPipelineLayout() const noexcept
    {
        return m_Layout;
    }
    const ShaderManager& Pipeline::GetShaderManager() const
    {
        return m_ShaderManager;
    }
}