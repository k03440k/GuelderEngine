module;
#include "../includes/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:QueueFamilyIndices;

import GuelderEngine.Core.Types;

import <optional>;

export namespace GuelderEngine::Vulkan
{
    struct QueueFamilyIndices : INHERIT_GClass(QueueFamilyIndices)
    {
        DECLARE_DEFAULT_CTOR_AND_DTOR(QueueFamilyIndices);
        DECLARE_COPY(QueueFamilyIndices);

        QueueFamilyIndices(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

        //location of graphics Queue Family
        std::optional<Types::uint> graphicsFamily;
        std::optional<Types::uint> presentFamily;

        bool IsComplete() const noexcept;
    };
}