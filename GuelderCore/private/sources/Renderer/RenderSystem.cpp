//TODO: move this file to vulkan folder
module;
#include <vulkan/vulkan.hpp>
#include "../private/headers/Core/GObject/GClass.hpp"
module GuelderEngine.Vulkan;
import :RenderSystem;

import :IVulkanObject;
import :Pipeline;
import :Mesh;
import :VertexBuffer;
import :IndexBuffer;

import <vector>;

namespace GuelderEngine::Vulkan
{
    RenderSystem::RenderSystem(const vk::Device& device, const vk::RenderPass& renderPass, const ShaderInfo& shaderInfo)
        : m_Pipeline(device, renderPass, shaderInfo) {}
    void RenderSystem::Reset() noexcept
    {
        m_Pipeline.Reset();
    }
    void RenderSystem::Cleanup(const vk::Device& device) const noexcept
    {
        m_Pipeline.Cleanup(device);
    }
    void RenderSystem::SetShaderInfo(const vk::Device& device, const vk::RenderPass& renderPass, const Vulkan::ShaderInfo& shaderInfo)
    {
        m_Pipeline.SetShaderInfo(device, renderPass, shaderInfo);
    }
    void RenderSystem::Render(const vk::CommandBuffer& commandBuffer, const SimplePushConstantData& push, const Buffers::VertexBuffer& vertexBuffer, const Buffers::IndexBuffer& indexBuffer)
    {
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline.GetPipeline());

        vertexBuffer.Bind(commandBuffer, { 0 });
        if(indexBuffer.GetIndicesCount())
            indexBuffer.Bind(commandBuffer, { 0 });
        commandBuffer.pushConstants(m_Pipeline.GetPipelineLayout(), vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(push), &push);

        if(indexBuffer.GetIndicesCount())
            commandBuffer.drawIndexed(indexBuffer.GetIndicesCount(), 1, 0, 0, 0);
        else
            commandBuffer.draw(vertexBuffer.GetVerticesCount(), 1, 0, 0);
    }
}