module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
export module GuelderEngine.Vulkan:VulkanManager;

import :VulkanDebugManager;
import :VulkanDeviceManager;
import :VulkanSurfaceManager;

import <string_view>;
import <vector>;
import <optional>;

export namespace GuelderEngine::Vulkan
{
    /*
    * Manager which responsible for all about initialization of Vulkan API
    */
    class VulkanManager : INHERIT_GClass(VulkanManager)
    {
    public:
        //VulkanManager() = default;
        VulkanManager(GLFWwindow* glfwWindow, const std::string_view& name = "Guelder Engine Editor");
        virtual ~VulkanManager();

        DELETE_COPY_AND_MOVE(VulkanManager);

        void Cleanup() const;

        static bool AreExtensionsSupported(const std::vector<const char*>&extensions);
        //static bool IsValidationLayersSupported(const std::vector<const char*>& layers);
    private:
        static vk::Instance CreateVkInstance(const char* name);

        vk::Instance m_Instance;

        //dynamic instance dispatcher
        vk::DispatchLoaderDynamic m_DLDI;
        VulkanDeviceManager m_DeviceManager;
        VulkanSurfaceManager m_SurfaceManager;
#ifdef GE_DEBUG_VULKAN
        VulkanDebugManager m_DebugManager;
#endif //GE_DEBUG_VULKAN
    };
}