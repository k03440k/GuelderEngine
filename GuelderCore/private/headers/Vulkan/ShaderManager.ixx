module;
#include <vulkan/vulkan.hpp>
#include "../../headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:ShaderManager;

import :IVulkanObject;
export import :Mesh;
import GuelderEngine.Core.Types;

import <string>;
import <string_view>;

export namespace GuelderEngine::Vulkan
{
    struct ShaderInfo
    {
        ShaderInfo() = default;
        ShaderInfo(const std::string& vertexPath, const std::string& fragmentPath,
            const VertexAttributeDescriptionsInfo& info = { {VertexFormat::Vec2Float, 0}, {VertexFormat::Vec3Float, 1} });

        VertexAttributeDescriptionsInfo info;
        std::string vertexPath;
        std::string fragmentPath;
    };
    class ShaderManager
    {
    public:
        //DECLARE_DCAD_AND_CAM(ShaderManager);
        DECLARE_DEFAULT_CTOR_AND_DTOR(ShaderManager);

        ShaderManager(const ShaderInfo& info);

        const ShaderInfo& GetShaderInfo() const noexcept;

        static vk::ShaderModule CreateModule(const std::string_view& shaderSource, const vk::Device& device);

    private:
        ShaderInfo m_Info;
    };
}