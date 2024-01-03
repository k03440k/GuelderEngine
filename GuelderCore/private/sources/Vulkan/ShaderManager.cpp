module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
module GuelderEngine.Vulkan;
import :ShaderManager;

import GuelderEngine.Core.Types;
import GuelderEngine.Utils;

namespace GuelderEngine::Vulkan
{
    ShaderInfo::ShaderInfo(const std::string& vertexPath, const std::string& fragmentPath, const VertexAttributeDescriptionsInfo& info)
        : info(info), vertexPath(vertexPath), fragmentPath(fragmentPath){}
    ShaderManager::ShaderManager(const ShaderInfo& info)
        : m_Info(info)
    {

    }
    const ShaderInfo& ShaderManager::GetShaderInfo() const noexcept
    {
        return m_Info;
    }
    //ShaderManager::ShaderManager(const ShaderManager& other)
    //{
    //    m_Info = other.m_Info;
    //}
    //ShaderManager::ShaderManager(ShaderManager&& other) noexcept
    //{
    //    m_Info = other.m_Info;
    //}
    //ShaderManager& ShaderManager::operator=(const ShaderManager& other)
    //{
    //    m_Info = other.m_Info;
    //}
    //ShaderManager& ShaderManager::operator=(ShaderManager&& other) noexcept
    //{
    //    m_Info = other.m_Info;
    //}
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