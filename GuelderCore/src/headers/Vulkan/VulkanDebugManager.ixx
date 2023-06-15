module;

//#include "../includes/GuelderEngine/GuelderEngine.hpp"

//#include <string>
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>

export module GuelderEngine.Vulkan:VulkanDebugManager;

//import <string>;
//import <vector>;
import <iostream>;
import <stdexcept>;
import <string_view>;
import <string>;
import <sstream>;
import <chrono>;
import <type_traits>;
import <functional>;
import <vector>;
//TODO: VulkanDebugManager::CreateDebugMessenger

export namespace GuelderEngine
{
    namespace Vulkan
    {
        class VulkanDebugLayersManager : INHERIT_GClass(VulkanDebugLayersManager)
        {
        public:
            using ValidationLayer = const char*;

            VulkanDebugLayersManager(const std::vector<ValidationLayer> layers);
            ~VulkanDebugLayersManager() = default;

            const std::vector<ValidationLayer>& GetLayers() { return m_Layers; }
        private:
            static bool AreValidationLayersSupported(const std::vector<ValidationLayer>&layers);

            const std::vector<ValidationLayer> m_Layers;
        };
        class VulkanDebugManager : INHERIT_GClass(VulkanDebugManager)
        {
        public:
            VulkanDebugManager() = default;
            VulkanDebugManager(const vk::Instance & instance, const vk::DispatchLoaderDynamic & dldi/*,
                const std::vector<const char* const> validationLayers*/);
            ~VulkanDebugManager() = default;

            VulkanDebugManager& operator=(const VulkanDebugManager & other);

            static void LogDeviceProperties(const vk::PhysicalDevice & device);

            //const std::vector<const char* const>& GetValidationLayers() { return m_ValidationLayers; }

            //const vk::DebugUtilsMessengerEXT& GetDebuggerMessenger() { return m_DebugMessenger; }
            void PP(int& a)
            {
                a = 555;
            }
        private:
            friend class VulkanManager;

            static vk::DebugUtilsMessengerEXT CreateDebugMessenger(const vk::Instance & instance, const vk::DispatchLoaderDynamic & dldi);

            static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT messageServerity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT * callbackData,
                void* userData);

            //debug callback
            vk::DebugUtilsMessengerEXT m_DebugMessenger;

            //const VulkanDebugLayersManager m_LayersManager;

            //const std::vector<const char* const>& m_ValidationLayers;
        };
    }
}