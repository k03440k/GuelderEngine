//TODO: move this file to vulkan folder
module;
#include <vulkan/vulkan.hpp>
module GuelderEngine.Vulkan;
import :RenderSystem;

import :IVulkanObject;
import :Pipeline;
import :Mesh;
import :VertexBuffer;
import :IndexBuffer;

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
}