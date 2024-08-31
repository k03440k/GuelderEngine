module;
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:IndexBuffer;

import :Buffer;
import :Mesh;

export namespace GuelderEngine::Vulkan::Buffers
{
    class IndexBuffer final : public Buffer
    {
    public:
        DECLARE_DEFAULT(IndexBuffer);

        IndexBuffer(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& queueFamilyIndices,
            const vk::CommandPool& transferPool,
            const vk::Queue& transferQueue,
            const Indices& indices
        );

        void Reset() noexcept override;

        void Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const;

        uint GetIndicesCount() const noexcept;
    private:
        uint m_IndicesCount;
    };
}