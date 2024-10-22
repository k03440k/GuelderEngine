module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
#include <glfw/glfw3.h>
export module GuelderEngine.UserInterface:UserInterfaceManager;

import GuelderEngine.Core.Types;
import GuelderEngine.Vulkan;

export namespace GuelderEngine::UserInterface
{
    class UserInterfaceManager : public Vulkan::IVulkanObject
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(UserInterfaceManager);
        DECLARE_DEFAULT_COPYING(UserInterfaceManager);
        DECLARE_MOVING(UserInterfaceManager);

        UserInterfaceManager(
            GLFWwindow* _GLFWwindow,
            const vk::Instance& instance,
            const vk::PhysicalDevice& physicalDevice,
            const vk::Device& device,
            const vk::RenderPass& renderPass,
            const Vulkan::QueueFamilyIndices& indices,
            const uint& countOfImages,
            const uint& minImageCount,
            const vk::Queue& queue
        );

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        static void BeginFrame();
        static void RenderFrame(const vk::CommandBuffer& commandBuffer);

        const Vulkan::DescriptorPool& GetDescriptorPool() const;

    private:
        Vulkan::DescriptorPool m_DescriptorPool;
    };
}