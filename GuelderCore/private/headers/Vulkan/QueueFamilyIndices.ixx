module;
#include "../public/GuelderEngine/Utils/Debug.hpp"
#include "../Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Vulkan:QueueFamilyIndices;

import GuelderEngine.Core.Types;

import <optional>;

export namespace GuelderEngine::Vulkan
{
    struct QueueFamilyIndices// : INHERIT_GClass(QueueFamilyIndices)
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(QueueFamilyIndices);
        DECLARE_COPYING(QueueFamilyIndices);

        QueueFamilyIndices(const vk::PhysicalDevice& device/*, const vk::SurfaceKHR& surface*/);

        bool IsComplete() const;

        uint GetGraphicsFamily() const;
        uint GetPresentFamily() const;
        uint GetTransferFamily() const;
    private:
        std::optional<uint> graphicsFamily;
        std::optional<uint> presentFamily;
        std::optional<uint> transferFamily;
    };
}