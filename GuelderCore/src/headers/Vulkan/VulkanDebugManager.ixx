module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanDebugManager;

import <vector>;
//TODO: VulkanDebugManager::CreateDebugMessenger

export namespace GuelderEngine::Vulkan
{
    class VulkanDebugLayersManager final : INHERIT_GClass(VulkanDebugLayersManager)
    {
    public:
        using ValidationLayer = const char*;

        VulkanDebugLayersManager(const std::vector<ValidationLayer>& layers);
        ~VulkanDebugLayersManager() = default;

        const std::vector<ValidationLayer>& GetLayers() const { return m_Layers; }
    private:
        static bool AreValidationLayersSupported(const std::vector<ValidationLayer>&layers);

        const std::vector<ValidationLayer> m_Layers;
    };
    class VulkanDebugManager final : INHERIT_GClass(VulkanDebugManager)
    {
    public:
        VulkanDebugManager() = default;
        VulkanDebugManager(const vk::Instance & instance, const vk::DispatchLoaderDynamic & dldi/*,
                const std::vector<const char* const> validationLayers*/);
        ~VulkanDebugManager() = default;

        VulkanDebugManager& operator=(const VulkanDebugManager& other);//because VulkanDebugManager doesn't contain move ctor

        static void LogDeviceProperties(const vk::PhysicalDevice & device);
    private:
        friend class VulkanManager;

        static vk::DebugUtilsMessengerEXT CreateDebugMessenger(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi);

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT * callbackData,
            void* userData);

        //debug callback
        vk::DebugUtilsMessengerEXT m_DebugMessenger;

        //const VulkanDebugLayersManager m_LayersManager;
    };
}
