module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan;
import :VulkanSurfaceManager;

namespace GuelderEngine::Vulkan
{
    VulkanSurfaceManager::VulkanSurfaceManager(const vk::Instance& instance, GLFWwindow* glfwWindow)
    {
        VkSurfaceKHR cStyle;
        GE_CORE_CLASS_ASSERT(glfwCreateWindowSurface(instance, glfwWindow, nullptr, &cStyle) == VK_SUCCESS,
            "cannot abstract GLFWwindow for vulkan surface");
        surface = cStyle;//TODO: VulkanSurfaceManager
    }
}