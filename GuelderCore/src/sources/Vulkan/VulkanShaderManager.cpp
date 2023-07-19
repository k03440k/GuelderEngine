module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
module GuelderEngine.Vulkan;
import :VulkanShaderManager;

import GuelderEngine.Core.Types;

namespace GuelderEngine::Vulkan
{
    vk::ShaderModule VulkanShaderManager::CreateModule(const std::string_view& shaderSource, const vk::Device& device)
    {
        const vk::ShaderModuleCreateInfo moduleInfo(vk::ShaderModuleCreateFlags(), shaderSource.size(),
            reinterpret_cast<const uint32_t*>(shaderSource.data()));

        return device.createShaderModule(moduleInfo);
    }
}