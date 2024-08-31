module;
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:StagingBuffer;

import :Buffer;
import :QueueFamilyIndices;

export namespace GuelderEngine::Vulkan::Buffers
{
    class StagingBuffer : public Buffer
    {
    public:
        DECLARE_DEFAULT(StagingBuffer);

        StagingBuffer(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& indices,
            const void* data,
            const uint& instanceSize,
            const uint& instanceCount,
            const uint& minOffsetAlignment = 1
        );
    };
}