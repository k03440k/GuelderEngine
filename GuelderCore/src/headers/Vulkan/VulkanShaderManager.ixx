module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:VulkanShaderManager;

import GuelderEngine.Core.Types;

export namespace GuelderEngine::Vulkan
{
    class VulkanShaderManager// : INHERIT_GClass(VulkanShaderManager)
    {
    public:
        DELETE_COPY_AND_MOVING(VulkanShaderManager);

        static vk::ShaderModule CreateModule(const std::string_view& shaderSource, const vk::Device& device);
    private:

    };
}