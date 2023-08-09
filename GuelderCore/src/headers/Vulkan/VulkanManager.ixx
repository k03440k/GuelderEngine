module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
export module GuelderEngine.Vulkan:VulkanManager;

import :IVulkanObject;
import :VulkanDebugManager;
import :VulkanDeviceManager;
import GuelderEngine.Core.Types;
//import :VulkanSurfaceManager;

import <string_view>;
import <vector>;
import <optional>;

using namespace GuelderEngine::Types;

export namespace GuelderEngine::Vulkan
{
    class VulkanScene : public IVulkanObject
    {
    public:
        DECLARE_COPY_AND_MOVING(VulkanScene);

        VulkanScene();
        ~VulkanScene() = default;

        void Reset() noexcept override;

        const std::vector<glm::vec3>& GetTrianglesPositions() const noexcept { return m_TrianglePositions; }
    private:
        std::vector<glm::vec3> m_TrianglePositions;
    };
}
export namespace GuelderEngine::Vulkan
{
    /*
    * Manager which responsible for all about initialization of Vulkan API
    */
    class VulkanManager : public IVulkanObject, INHERIT_GClass(VulkanManager)
    {
    public:
        VulkanManager() = default;
        VulkanManager(GLFWwindow* glfwWindow, const std::string_view& vertPath, const std::string_view& fragPath,
            const std::string_view& name = "Guelder Engine Editor");
        virtual ~VulkanManager();

        DECLARE_COPY_AND_MOVING(VulkanManager);

        virtual void Reset() noexcept override;

        static bool AreExtensionsSupported(const std::vector<const char*>& extensions);
        //void LoadVertexShader(const std::string_view& source);
        //void LoadFragmentShader(const std::string_view& source);
        void Render(GLFWwindow* glfwWindow);
    private:
        static vk::Instance CreateVkInstance(const char* name);
        virtual void Cleanup() const noexcept;

        vk::Instance m_Instance;

        VulkanDeviceManager m_DeviceManager;
        VulkanScene m_Scene;
#ifdef GE_DEBUG_VULKAN
        VulkanDebugManager m_DebugManager;
#endif //GE_DEBUG_VULKAN
    };
}