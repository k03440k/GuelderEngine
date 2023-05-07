#include "../../headers/Vulkan/VulkanDebugManager.hpp"

namespace GuelderEngine
{
    namespace Vulkan
    {
        VulkanDebugLayersManager::VulkanDebugLayersManager(const std::vector<const char*> layers)
            : m_Layers(layers)
        {
            GE_CORE_ASSERT(AreValidationLayersSupported(layers), "validation layers are not supported");
        }
        bool VulkanDebugLayersManager::AreValidationLayersSupported(const std::vector<const char*>& layers)
        {
            GE_CORE_ASSERT(layers.size() > 0, "layers size is zero");

            std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

#ifdef DEBUG_VULKAN
            LOG_INFO("Device can support following layers:");
            for (const auto& layer : supportedLayers)
            {
                LOG_INFO('\t', layer.layerName);
            }
#endif // DEBUG_VULKAN

            bool found = false;
            for (const auto& extension : layers)
            {
                found = false;
                for (const auto& supportedExtension : supportedLayers)
                {
                    if (strcmp(extension, supportedExtension.layerName) == 0)
                    {
                        found = true;
#ifdef DEBUG_VULKAN
                        LOG_INFO("Layer \"", extension, "\" is supported");
#endif //DEBUG_VULKAN
                    }
                }
                if (!found)
                {
#ifdef DEBUG_VULKAN
                    LOG_INFO("Layer \"", extension, "\" is not supported");
#endif //DEBUG_VULKAN
                    return false;
                }
            }

            return true;
        }
        VulkanDebugManager::VulkanDebugManager(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi)
        {
        }
        /*VulkanDebugManager::VulkanDebugManager(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi, const std::vector<const char* const> validationLayers)
            : m_LayersManager(validationLayers)
        {

        }*/
    }
}