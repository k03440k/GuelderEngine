module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:VulkanQueueFamilyIndices;

import GuelderEngine.Core.Types;

import <optional>;

export namespace GuelderEngine::Vulkan
{
    struct VulkanQueueFamilyIndices : INHERIT_GClass(VulkanQueueFamilyIndices)
    {
        DECLARE_DEFAULT_CTOR_AND_DTOR(VulkanQueueFamilyIndices);
        DECLARE_COPY(VulkanQueueFamilyIndices);

        VulkanQueueFamilyIndices(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

        //location of graphics Queue Family
        std::optional<Types::uint> graphicsFamily;
        std::optional<Types::uint> presentFamily;

        bool IsComplete() const noexcept;
    };
}