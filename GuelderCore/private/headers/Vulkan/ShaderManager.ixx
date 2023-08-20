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

        /**
         * @param vertexPath full path to the compiled(.spv) vertex shader
         * @param fragmentPath full path to the compiled(.spv) fragment shader
         */
        ShaderManager(const std::string_view& vertexPath, const std::string_view& fragmentPath, const Types::uint& inPosLocation = 0, const Types::uint& inColorLocation = 1);

        void Reset() noexcept override;

        std::string GetVertexPath() const noexcept;
        std::string GetFragmentPath() const noexcept;

        static vk::ShaderModule CreateModule(const std::string_view& shaderSource, const vk::Device& device);

    private:
        std::string m_VertexPath;
        std::string m_FragmentPath;

        Types::uint m_InPosLocation;
        Types::uint m_InColorLocation;
    };
}