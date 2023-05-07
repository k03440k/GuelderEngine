#pragma once

#include "../includes/GuelderEngine/GuelderEngine.hpp"

#include <string>

#include <vulkan/vulkan.hpp>

//TODO: VulkanDebugManager::CreateDebugMessenger

namespace GuelderEngine
{
    namespace Vulkan
    {
        class VulkanDebugLayersManager : INHERIT_GClass(VulkanDebugLayersManager)
        {
        public:
            VulkanDebugLayersManager(const std::vector<const char*> layers);
            ~VulkanDebugLayersManager() = default;

            const std::vector<const char*>& GetLayers() { return m_Layers; }
        private:
            static bool AreValidationLayersSupported(const std::vector<const char*>&layers);

            const std::vector<const char*> m_Layers;
        };
        class VulkanDebugManager : INHERIT_GClass(VulkanDebugManager)
        {
        public:
        public:
            VulkanDebugManager() = default;
            VulkanDebugManager(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi/*,
                const std::vector<const char* const> validationLayers*/);
            ~VulkanDebugManager() = default;

            //const std::vector<const char* const>& GetValidationLayers() { return m_ValidationLayers; }

        private:
            static vk::DebugUtilsMessengerEXT CreateDebugMessenger(const vk::Instance & instance, const vk::DispatchLoaderDynamic & dldi);

            //debug callback
            vk::DebugUtilsMessengerEXT m_DebugMessenger;

            //const VulkanDebugLayersManager m_LayersManager;

            //const std::vector<const char* const>& m_ValidationLayers;
        };
    }
}