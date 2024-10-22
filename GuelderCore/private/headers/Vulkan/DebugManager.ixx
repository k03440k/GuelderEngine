module;
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:DebugManager;

import :IVulkanObject;

import <vector>;

export namespace GuelderEngine::Vulkan
{
    bool AreValidationLayersSupported(const std::vector<const char*>& layers);

    class DebugManager final : public IVulkanObject
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(DebugManager);
        DECLARE_DEFAULT_COPYING(DebugManager);
        DECLARE_MOVING(DebugManager);

        DebugManager(const vk::Instance& instance);

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
            void* userData
        );

        //debug callback
        vk::DebugUtilsMessengerEXT m_DebugMessenger;

        //dynamic instance dispatcher
        vk::DispatchLoaderDynamic m_DLDI;

        //const VulkanDebugLayersManager m_LayersManager;
    };
}