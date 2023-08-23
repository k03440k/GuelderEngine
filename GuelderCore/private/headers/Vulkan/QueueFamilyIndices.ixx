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
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(QueueFamilyIndices);
        DECLARE_COPY(QueueFamilyIndices);

        QueueFamilyIndices(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

        bool IsComplete() const noexcept;

        Types::uint GetGraphicsFamily() const noexcept;
        Types::uint GetPresentFamily() const noexcept;
        Types::uint GetTransferFamily() const noexcept;
    private:
        std::optional<Types::uint> graphicsFamily;
        std::optional<Types::uint> presentFamily;
        std::optional<Types::uint> transferFamily;
    };
}