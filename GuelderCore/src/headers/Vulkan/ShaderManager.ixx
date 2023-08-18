module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:ShaderManager;

import :IVulkanObject;
import GuelderEngine.Core.Types;

import <string>;
import <string_view>;

export namespace GuelderEngine::Vulkan
{
    class ShaderManager : public IVulkanObject
    {
    public:
        DECLARE_DCAD_AND_CAM(ShaderManager);

        ShaderManager(const std::string_view& vertexPath, const std::string_view& fragmentPath);

        virtual void Reset() noexcept override;

        std::string GetVertexPath() const noexcept;
        std::string GetFragmentPath() const noexcept;

        static vk::ShaderModule CreateModule(const std::string_view& shaderSource, const vk::Device& device);

    private:
        std::string vertexPath;
        std::string fragmentPath;
    };
}