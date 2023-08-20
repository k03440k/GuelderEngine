module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:DebugManager;

import :IVulkanObject;

import <vector>;

export namespace GuelderEngine::Vulkan
{
    class DebugLayersManager final : INHERIT_GClass(DebugLayersManager)
    {
    public:
        using ValidationLayer = const char*;

        DebugLayersManager(const std::vector<ValidationLayer>& layers);
        ~DebugLayersManager() = default;

        const std::vector<ValidationLayer>& GetLayers() const { return m_Layers; }
    private:
        static bool AreValidationLayersSupported(const std::vector<ValidationLayer>&layers);

        const std::vector<ValidationLayer> m_Layers;
    };
    class DebugManager final : public IVulkanObject, INHERIT_GClass(DebugManager)
    {
    public:
        DECLARE_DCAD_AND_CAM(DebugManager);
        DebugManager(const vk::Instance& instance/*,
                const std::vector<const char* const> validationLayers*/);

        virtual void Reset() noexcept override;
        void Cleanup(const vk::Instance& instance) const noexcept;

        static void LogDeviceProperties(const vk::PhysicalDevice& device);
        static void LogPresentMode(const vk::PresentModeKHR& mode);
    private:
        static vk::DebugUtilsMessengerEXT CreateDebugMessenger(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi);

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT * callbackData,
            void* userData);

        //debug callback
        vk::DebugUtilsMessengerEXT m_DebugMessenger;

        //dynamic instance dispatcher
        vk::DispatchLoaderDynamic m_DLDI;

        //const VulkanDebugLayersManager m_LayersManager;
    };
}