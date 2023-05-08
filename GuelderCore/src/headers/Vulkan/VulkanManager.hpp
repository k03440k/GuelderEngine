#pragma once

#include "../includes/GuelderEngine/GuelderEngine.hpp"

#include "VulkanDebugManager.hpp"

#include <vulkan/vulkan.hpp>

namespace GuelderEngine
{
    namespace Vulkan
    {
        class VulkanDeviceManager : INHERIT_GClass(VulkanDeviceManager)
        {
        public:
            VulkanDeviceManager() = default;
            VulkanDeviceManager(const vk::Instance& instance);
            ~VulkanDeviceManager() = default;

            VulkanDeviceManager& operator=(const VulkanDeviceManager& other);
        private:
            static vk::PhysicalDevice ChoosePhysicalDevice(const vk::Instance & instance);

            vk::PhysicalDevice m_PhysicalDevice;
        };
        /*
        * Manager which responsible for all about initialization of Vulkan API
        */
        class VulkanManager : INHERIT_GClass(VulkanManager)
        {
        public:
            VulkanManager(const std::string_view& name = "Guelder Engine Editor");
            ~VulkanManager();

            static bool IsExtensionsSupported(const std::vector<const char*>&extensions);
            //static bool IsValidationLayersSupported(const std::vector<const char*>& layers);
        protected:
        private:
            static vk::Instance CreateVkInstance(const char* const name);

            void Init(const std::string_view & name);
            void VkInit(const char* const name);
            void Cleanup();

            vk::Instance m_Instance;

            //dynamic instance dispatcher
            vk::DispatchLoaderDynamic m_DLDI;

            VulkanDeviceManager m_DeviceManager;
#ifdef DEBUG_VULKAN
            VulkanDebugManager m_DebugManager;
#endif //DEBUG_VULKAN
        };
    }
}