export module GuelderEngine.Vulkan:IVulkanBase;

export namespace GuelderEngine::Vulkan
{
    class IVulkanBase
    {
    public:
        virtual ~IVulkanBase() = default;

        /*
         *@brief method which must reset all values but not to delete them, so you can use operator=(...) or other assignments
         *@brief simply set all vk namespace values to nullptr
         */
        virtual void Reset() = 0;
    };
}