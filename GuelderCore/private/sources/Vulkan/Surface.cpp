module;
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
#include "GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :Surface;

namespace GuelderEngine::Vulkan
{
    Surface::Surface(GLFWwindow* window, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice)
    {
        VkSurfaceKHR cStyle;
        GE_ASSERT(glfwCreateWindowSurface(instance, window, nullptr, &cStyle) == VK_SUCCESS, "cannot abstract GLFWwindow for vulkan surface");
        m_Surface = cStyle;

        m_Capabilities = physicalDevice.getSurfaceCapabilitiesKHR(m_Surface);
        m_Format = ChooseSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(m_Surface));
        m_PresentMode = ChoosePresentMode(physicalDevice.getSurfacePresentModesKHR(m_Surface));
    }
    void Surface::Reset() noexcept
    {
        m_Surface = nullptr;
        m_Capabilities = vk::SurfaceCapabilitiesKHR{};
        m_Format = vk::Format::eUndefined;
    }
    void Surface::Cleanup(const vk::Instance& instance) const noexcept
    {
        instance.destroySurfaceKHR(m_Surface);
    }
    void Surface::Recreate(const vk::PhysicalDevice& physicalDevice)
    {
        m_Capabilities = physicalDevice.getSurfaceCapabilitiesKHR(m_Surface);
        m_Format = ChooseSurfaceFormat(physicalDevice.getSurfaceFormatsKHR(m_Surface));
        m_PresentMode = ChoosePresentMode(physicalDevice.getSurfacePresentModesKHR(m_Surface));
    }
    const vk::SurfaceKHR& Surface::GetSurface() const
    {
        return m_Surface;
    }
    const vk::SurfaceCapabilitiesKHR& Surface::GetCapabilities() const
    {
        return m_Capabilities;
    }
    const vk::SurfaceFormatKHR& Surface::GetFormat() const
    {
        return m_Format;
    }
    const vk::PresentModeKHR& Surface::GetPresentMode() const
    {
        return m_PresentMode;
    }
    vk::SurfaceFormatKHR Surface::ChooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats)
    {
        for(auto&& format : formats)
            if(format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
                return format;

        GE_LOG(VulkanCore, Warning, "the device doesn't support: vk::Format::eB8G8R8A8Unorm and vk::ColorSpaceKHR::eSrgbNonlinear");

        return formats[0];
    }
    vk::PresentModeKHR Surface::ChoosePresentMode(const std::vector<vk::PresentModeKHR>& presentModes)
    {
        for(auto&& presentMode : presentModes)
        {
            if(presentMode == vk::PresentModeKHR::eMailbox)
                return presentMode;
        }

        GE_LOG(VulkanCore, Warning, "the device doesn't support Mailbox present mode. Choosing Fifo");

        return vk::PresentModeKHR::eFifo;
    }
}