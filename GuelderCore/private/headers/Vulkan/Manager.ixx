module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
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
    class Scene : public IVulkanObject
    {
    public:
        DECLARE_COPY_AND_MOVING(Scene);

        Scene();
        ~Scene() = default;

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
        VulkanManager(GLFWwindow* glfwWindow, const Types::uint& width, const Types::uint& height, const std::string_view& vertPath, const std::string_view& fragPath, const Mesh_t& mesh = {},
            const std::string_view& name = "Guelder Engine Editor");
        virtual ~VulkanManager();

        DECLARE_COPY_AND_MOVING(VulkanManager);

        virtual void Reset() noexcept override;

        static bool AreExtensionsSupported(const std::vector<const char*>& extensions);
        //must be called before Render
        void SetMesh(const Mesh& mesh);
        //void LoadVertexShader(const std::string_view& source);
        //void LoadFragmentShader(const std::string_view& source);
        /**
         * \param width window width
         * \param height window height
         */
        void Render(Types::uint width, Types::uint height);
    private:
        static vk::Instance CreateVkInstance(const char* name);
        virtual void Cleanup() const noexcept;

        vk::Instance m_Instance;

        DeviceManager m_DeviceManager;
        Pipeline m_Pipeline;
        Scene m_Scene;
#ifdef GE_DEBUG_VULKAN
        DebugManager m_DebugManager;
#endif //GE_DEBUG_VULKAN
    };
}