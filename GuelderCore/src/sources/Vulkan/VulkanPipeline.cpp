module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
module GuelderEngine.Vulkan;
import :VulkanPipeline;

import GuelderEngine.Core.Types;
import :VulkanShaderManager;
import GuelderEngine.Core;

export namespace GuelderEngine::Vulkan
{
    VulkanPipeline::VulkanPipeline(const vk::Device& device, const vk::Extent2D& extent, const vk::Format& swapchainImageFormat, const std::string_view& vertexPath, const std::string_view& fragmentPath)
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
        const vk::Viewport viewport(0.0f, 0.0f, extent.width, extent.height, 0.0f, 1.0f);
        const vk::Rect2D scissor({0, 0}, extent);

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
            {0, 0, 0, 0}
        );

        m_Layout = CreateLayout(device);
        m_RenderPass = CreateRenderPass(device, swapchainImageFormat);

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
    VulkanPipeline::VulkanPipeline(const VulkanPipeline& other)
    {
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;
    }
    VulkanPipeline::VulkanPipeline(VulkanPipeline&& other) noexcept
    {
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;

        other.Reset();
    }
    VulkanPipeline& VulkanPipeline::operator=(const VulkanPipeline& other)
    {
        if(this == &other)
            return *this;

        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;

        return *this;
    }
    VulkanPipeline& VulkanPipeline::operator=(VulkanPipeline&& other) noexcept
    {
        m_GraphicsPipeline = other.m_GraphicsPipeline;
        m_Layout = other.m_Layout;
        m_RenderPass = other.m_RenderPass;

        other.Reset();

        return *this;
    }

    void VulkanPipeline::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyPipelineLayout(m_Layout);
        device.destroyRenderPass(m_RenderPass);
        device.destroyPipeline(m_GraphicsPipeline);
    }

    void VulkanPipeline::Reset() noexcept
    {
        m_GraphicsPipeline = nullptr;
        m_Layout = nullptr;
        m_RenderPass = nullptr;
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
}