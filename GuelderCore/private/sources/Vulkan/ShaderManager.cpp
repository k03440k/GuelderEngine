module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
module GuelderEngine.Vulkan;
import :ShaderManager;

import GuelderEngine.Core.Types;
import GuelderEngine.Core;

namespace GuelderEngine::Vulkan
{
    ShaderManager::ShaderManager(const std::string_view& vertexPath, const std::string_view& fragmentPath, const Types::uint& inPosLocation, const Types::uint& inColorLocation)
        : m_VertexPath(vertexPath), m_FragmentPath(fragmentPath), m_InPosLocation(inPosLocation), m_InColorLocation(inColorLocation)
    {

    }
    void ShaderManager::Reset() noexcept
    {
        m_VertexPath.clear();
        m_FragmentPath.clear();
    }
    std::string ShaderManager::GetVertexPath() const noexcept
    {
        return m_VertexPath;
    }
    std::string ShaderManager::GetFragmentPath() const noexcept
    {
        return m_FragmentPath;
    }
    ShaderManager::ShaderManager(const ShaderManager& other)
    {
        m_VertexPath = other.m_VertexPath;
        m_FragmentPath = other.m_FragmentPath;
        m_InPosLocation = other.m_InPosLocation;
        m_InColorLocation = other.m_InColorLocation;
    }
    ShaderManager::ShaderManager(ShaderManager&& other) noexcept
    {
        m_VertexPath = std::forward<std::string>(other.m_VertexPath);
        m_FragmentPath = std::forward<std::string>(other.m_FragmentPath);
        m_InPosLocation = other.m_InPosLocation;
        m_InColorLocation = other.m_InColorLocation;

        other.Reset();
    }
    ShaderManager& ShaderManager::operator=(const ShaderManager& other)
    {
        if(this == &other)
            return *this;

        m_VertexPath = other.m_VertexPath;
        m_FragmentPath = other.m_FragmentPath;
        m_InPosLocation = other.m_InPosLocation;
        m_InColorLocation = other.m_InColorLocation;

        return *this;
    }
    ShaderManager& ShaderManager::operator=(ShaderManager&& other) noexcept
    {
        m_VertexPath = std::forward<std::string>(other.m_VertexPath);
        m_FragmentPath = std::forward<std::string>(other.m_FragmentPath);
        m_InPosLocation = other.m_InPosLocation;
        m_InColorLocation = other.m_InColorLocation;

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