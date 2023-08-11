module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
module GuelderEngine.Vulkan;
import :VulkanShaderManager;

import GuelderEngine.Core.Types;
import GuelderEngine.Core;

namespace GuelderEngine::Vulkan
{
    VulkanShaderManager::VulkanShaderManager(const std::string_view& vertexPath, const std::string_view& fragmentPath)
        : vertexPath(vertexPath), fragmentPath(fragmentPath)
    {

    }
    void VulkanShaderManager::Reset() noexcept
    {
        vertexPath.clear();
        fragmentPath.clear();
    }
    std::string VulkanShaderManager::GetVertexPath() const noexcept
    {
        return vertexPath;
    }
    std::string VulkanShaderManager::GetFragmentPath() const noexcept
    {
        return fragmentPath;
    }
    VulkanShaderManager::VulkanShaderManager(const VulkanShaderManager& other)
    {
        vertexPath = other.vertexPath;
        fragmentPath = other.fragmentPath;
    }
    VulkanShaderManager::VulkanShaderManager(VulkanShaderManager&& other) noexcept
    {
        vertexPath = std::forward<std::string>(other.vertexPath);
        fragmentPath = std::forward<std::string>(other.fragmentPath);

        other.Reset();
    }
    VulkanShaderManager& VulkanShaderManager::operator=(const VulkanShaderManager& other)
    {
        if(this == &other)
            return *this;

        vertexPath = other.vertexPath;
        fragmentPath = other.fragmentPath;

        return *this;
    }
    VulkanShaderManager& VulkanShaderManager::operator=(VulkanShaderManager&& other) noexcept
    {
        vertexPath = std::forward<std::string>(other.vertexPath);
        fragmentPath = std::forward<std::string>(other.fragmentPath);

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    vk::ShaderModule VulkanShaderManager::CreateModule(const std::string_view& shaderSource, const vk::Device& device)
    {
        const vk::ShaderModuleCreateInfo moduleInfo(vk::ShaderModuleCreateFlags(), shaderSource.size(),
            reinterpret_cast<const uint32_t*>(shaderSource.data()));

        return device.createShaderModule(moduleInfo);
    }
}