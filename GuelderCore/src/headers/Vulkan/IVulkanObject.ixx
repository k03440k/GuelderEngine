module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
export module GuelderEngine.Vulkan:IVulkanObject;

import GuelderEngine.Debug;

export namespace GuelderEngine::Vulkan
{
    DECLARE_LOG_CATEGORY_EXTERN(VulkanCore, All);
    class IVulkanObject
    {
    public:
        virtual ~IVulkanObject() = default;

        /*
         *@brief method which must reset all values but not to delete them, so you can use operator=(...) or other assignments
         *@brief simply set all vk namespace values to nullptr
        */
        virtual void Reset() noexcept = 0;
    };
}