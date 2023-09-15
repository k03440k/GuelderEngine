module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
export module GuelderEngine.Vulkan:Manager;

import :IVulkanObject;
import :DebugManager;
import :DeviceManager;
import :Mesh;
import GuelderEngine.Core.Types;
//import :VulkanSurfaceManager;

import <string_view>;
import <vector>;
import <optional>;

using namespace GuelderEngine::Types;

export namespace GuelderEngine::Vulkan
{
    /*
    * Manager which responsible for all about initialization of Vulkan API
    */
    class VulkanManager : public IVulkanObject//, INHERIT_GClass(VulkanManager)
    {
    public:
        VulkanManager() = default;
        VulkanManager(GLFWwindow* glfwWindow, const Types::uint& width, const Types::uint& height, const ShaderInfo& shaderInfo = {}, const std::string_view& name = "Guelder Engine Editor");
        virtual ~VulkanManager();

        DECLARE_COPY_AND_MOVING(VulkanManager);

        void Reset() noexcept override;

        static bool AreExtensionsSupported(const std::vector<const char*>& extensions);
        //must be called before Render
        void SetMesh(const Mesh& mesh);
        void SetShaderInfo(const ShaderInfo& shaderInfo);
        /**
         * \param width window width
         * \param height window height
         */
        void Render(Types::uint width, Types::uint height, bool& wasWindowResized);
    private:
        static vk::Instance CreateVkInstance(const char* name);
        virtual void Cleanup() const noexcept;

        vk::Instance m_Instance;

        DeviceManager m_DeviceManager;
        Pipeline m_Pipeline;
#ifdef GE_DEBUG_VULKAN
        DebugManager m_DebugManager;
#endif //GE_DEBUG_VULKAN
    };
}