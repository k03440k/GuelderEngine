module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanSurfaceManager;

//struct GLFWwindow;

export namespace GuelderEngine::Vulkan
{
    class VulkanSurfaceManager : INHERIT_GClass(VulkanSurfaceManager)
    {
    public:
        VulkanSurfaceManager() = default;
        VulkanSurfaceManager(const vk::Instance& instance, GLFWwindow* glfwWindow);
        ~VulkanSurfaceManager() = default;

        operator vk::SurfaceKHR() const { return surface; }

        vk::SurfaceKHR surface;
    private:
        vk::Queue m_PresentQueue;
    };
}