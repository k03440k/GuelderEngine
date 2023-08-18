module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
module GuelderEngine.Vulkan;
import :ShaderManager;

import GuelderEngine.Core.Types;
import GuelderEngine.Core;

namespace GuelderEngine::Vulkan
{
    ShaderManager::ShaderManager(const std::string_view& vertexPath, const std::string_view& fragmentPath)
        : vertexPath(vertexPath), fragmentPath(fragmentPath)
    {

    }
    void ShaderManager::Reset() noexcept
    {
        vertexPath.clear();
        fragmentPath.clear();
    }
    std::string ShaderManager::GetVertexPath() const noexcept
    {
        return vertexPath;
    }
    std::string ShaderManager::GetFragmentPath() const noexcept
    {
        return fragmentPath;
    }
    ShaderManager::ShaderManager(const ShaderManager& other)
    {
        vertexPath = other.vertexPath;
        fragmentPath = other.fragmentPath;
    }
    ShaderManager::ShaderManager(ShaderManager&& other) noexcept
    {
        vertexPath = std::forward<std::string>(other.vertexPath);
        fragmentPath = std::forward<std::string>(other.fragmentPath);

        other.Reset();
    }
    ShaderManager& ShaderManager::operator=(const ShaderManager& other)
    {
        if(this == &other)
            return *this;

        vertexPath = other.vertexPath;
        fragmentPath = other.fragmentPath;

        return *this;
    }
    ShaderManager& ShaderManager::operator=(ShaderManager&& other) noexcept
    {
        vertexPath = std::forward<std::string>(other.vertexPath);
        fragmentPath = std::forward<std::string>(other.fragmentPath);

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    vk::ShaderModule ShaderManager::CreateModule(const std::string_view& shaderSource, const vk::Device& device)
    {
        const vk::ShaderModuleCreateInfo moduleInfo(vk::ShaderModuleCreateFlags(), shaderSource.size(),
            reinterpret_cast<const uint32_t*>(shaderSource.data()));

        return device.createShaderModule(moduleInfo);
    }
}