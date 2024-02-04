module;
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
#include "../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:Surface;

import :IVulkanObject;

namespace GuelderEngine::Vulkan
{
    class Surface final : public IVulkanObject
    {
    public:
        DECLARE_DEFAULT(Surface);

        Surface(GLFWwindow* window, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice);

        void Reset() noexcept override;
        void Cleanup(const vk::Instance& instance) const noexcept;

        /*
         *@brief Must be called before the swapchain recreation
         *@brief This function just updates m_Capabilities, m_Format, m_PresentMode
         */
        void Recreate(const vk::PhysicalDevice& physicalDevice);

        const vk::SurfaceKHR& GetSurface() const;
        const vk::SurfaceCapabilitiesKHR& GetCapabilities() const;
        const vk::SurfaceFormatKHR& GetFormat() const;
        const vk::PresentModeKHR& GetPresentMode() const;
    private:
        static vk::SurfaceFormatKHR ChooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
        static vk::PresentModeKHR ChoosePresentMode(const std::vector<vk::PresentModeKHR>& presentModes);

        vk::SurfaceKHR m_Surface;
        vk::SurfaceCapabilitiesKHR m_Capabilities;
        vk::SurfaceFormatKHR m_Format;
        vk::PresentModeKHR m_PresentMode;
    };
}