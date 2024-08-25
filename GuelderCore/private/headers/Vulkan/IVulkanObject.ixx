module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:IVulkanObject;

import GuelderEngine.Debug;

export namespace GuelderEngine::Vulkan
{

    DECLARE_LOG_CATEGORY_EXTERN(VulkanCore, All, true, false);

    class IVulkanObject
    {
    public:
        DEFINE_INTERFACE(IVulkanObject);

        /**
         *@brief method which must reset all values but not to delete them, so you can use operator=(...) or other assignments. simply set all vk namespace values to nullptr
        */
        virtual void Reset() noexcept = 0;
    };
}