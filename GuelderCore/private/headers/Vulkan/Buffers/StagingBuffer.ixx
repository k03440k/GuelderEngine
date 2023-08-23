module;
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:StagingBuffer;

import :IBuffer;
import :Mesh;
import :QueueFamilyIndices;

export namespace GuelderEngine::Vulkan::Buffers
{
    class StagingBuffer : public IBuffer
    {
    public:
        DECLARE_DCAD_AND_CAM(StagingBuffer);

        StagingBuffer(const vk::Device& device, const vk::PhysicalDevice& physicalDevice, const QueueFamilyIndices& indices, const Mesh_t& mesh);
    };
}