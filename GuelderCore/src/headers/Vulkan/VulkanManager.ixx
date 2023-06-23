module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanManager;
import :VulkanDebugManager;

import <string_view>;
import <vector>;
import <optional>;

export namespace GuelderEngine::Vulkan
{
    struct QueueFamilyIndices
    {
        QueueFamilyIndices();
        ~QueueFamilyIndices() = default;

        //location of graphics Queue Family
        std::optional <Types::uint> graphicsFamily;
        std::optional <Types::uint> presentFamily;

        bool IsComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    };
    /*
     *You must manually call .Cleanup() func instead of dtor
     */
    class VulkanDeviceManager final : INHERIT_GClass(VulkanDeviceManager)
    {
    public:
        VulkanDeviceManager() = default;
        VulkanDeviceManager(const vk::Instance & instance);
        ~VulkanDeviceManager() = default;

        void Cleanup() const;

        VulkanDeviceManager& operator=(const VulkanDeviceManager & other);

        static QueueFamilyIndices FindQueueFamily(const vk::PhysicalDevice& device);
    private:
        static bool CheckDeviceExtensionsSupport(const vk::PhysicalDevice & physicalDevice,
                                                 const std::vector<const char*>& requestedExtensions);
        static bool IsDeviceSuitable(const vk::PhysicalDevice& physicalDevice);
        static vk::PhysicalDevice ChoosePhysicalDevice(const vk::Instance& instance);
        static vk::Device CreateDevice(const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices);
        static vk::Queue GetQueue(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const QueueFamilyIndices& indices);

        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_Device;
        vk::Queue m_GraphicsQueue;
        QueueFamilyIndices m_QueueIndices;
    };
    /*
        * Manager which responsible for all about initialization of Vulkan API
        */
    class VulkanManager : INHERIT_GClass(VulkanManager)
    {
    public:
        VulkanManager(const std::string_view& name = "Guelder Engine Editor");
        virtual ~VulkanManager();

        void Cleanup();

        static bool AreExtensionsSupported(const std::vector<const char*>& extensions);
        //static bool IsValidationLayersSupported(const std::vector<const char*>& layers);
    protected:
    private:
        static vk::Instance CreateVkInstance(const char* const name);

        void Init(const std::string_view& name);
        void VkInit(const char* const name);

        vk::Instance m_Instance;
        //dynamic instance dispatcher
        vk::DispatchLoaderDynamic m_DLDI;
        VulkanDeviceManager m_DeviceManager;
#ifdef GE_DEBUG_VULKAN
        static VulkanDebugManager m_DebugManager;
#endif //GE_DEBUG_VULKAN
    };
}