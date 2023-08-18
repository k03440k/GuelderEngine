module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:DeviceManager;

import :IVulkanObject;
import :DebugManager;
import :VulkanSwapchain;
import :Pipeline;
import GuelderEngine.Core.Types;

import <optional>;

export namespace GuelderEngine::Vulkan
{
    /*
     *You must manually call .Cleanup() func instead of dtor
    */
    class DeviceManager final : public IVulkanObject, INHERIT_GClass(DeviceManager)
    {
    public:
        DECLARE_DCAD_AND_CAM(DeviceManager);

        DeviceManager(const vk::Instance& instance, GLFWwindow* glfwWindow, const vk::Extent2D& windowSize, const std::string_view& vertPath, const std::string_view& fragPath, const std::vector<const char*>& extensions = {});

        void Reset() noexcept override;
        void Cleanup(const vk::Instance& instance) const noexcept;

        void Render(Types::uint width, Types::uint height, const Scene& scene);
    private:
        static bool CheckDeviceExtensionsSupport(const vk::PhysicalDevice& physicalDevice,
            const std::vector<const char*>& requestedExtensions);
        static bool IsDeviceSuitable(const vk::PhysicalDevice& physicalDevice, const std::vector<const char*>& extensions = {});
        static vk::PhysicalDevice ChoosePhysicalDevice(const vk::Instance& instance, const std::vector<const char*>& extensions = {});
        static vk::Device CreateDevice(const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices, const std::vector<const char*>& extensions = {});

        friend class VulkanManager;

        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_Device;
        QueueFamilyIndices m_QueueIndices;

        vk::SurfaceKHR m_Surface;
        
        Pipeline m_Pipeline;
    };
}