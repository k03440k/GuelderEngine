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
        DECLARE_COPY(QueueFamilyIndices);

        QueueFamilyIndices(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

        bool IsComplete() const noexcept;

        uint GetGraphicsFamily() const noexcept;
        uint GetPresentFamily() const noexcept;
        uint GetTransferFamily() const noexcept;
    private:
        std::optional<uint> graphicsFamily;
        std::optional<uint> presentFamily;
        std::optional<uint> transferFamily;
    };
}