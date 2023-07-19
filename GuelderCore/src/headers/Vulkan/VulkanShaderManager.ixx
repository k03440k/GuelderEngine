module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:VulkanShaderManager;

import GuelderEngine.Core.Types;

export namespace GuelderEngine::Vulkan
{
    class VulkanShaderManager : INHERIT_GClass(VulkanShaderManager)
    {
    public:
        VulkanShaderManager() = default;
        //VulkanShaderManager(const std::string_view& );
        ~VulkanShaderManager() = default;

        VulkanShaderManager(const VulkanShaderManager& other);
        VulkanShaderManager(VulkanShaderManager&& other) noexcept;
        VulkanShaderManager& operator=(const VulkanShaderManager& other);
        VulkanShaderManager& operator=(VulkanShaderManager&& other) noexcept;

        static vk::ShaderModule CreateModule(const std::string_view& shaderSource, const vk::Device& device);
    private:

    };
}