module;
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:IndexBuffer;

import :IBuffer;
import :Mesh;

export namespace GuelderEngine::Vulkan::Buffers
{
    class IndexBuffer final : public IBuffer
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(IndexBuffer);

        IndexBuffer(
            const vk::Device& device,
            const vk::PhysicalDevice& physicalDevice,
            const QueueFamilyIndices& queueFamilyIndices,
            const vk::CommandPool& transferPool,
            const vk::Queue& transferQueue,
            const Indices& indices
        );
        IndexBuffer(const IndexBuffer& other) noexcept;
        IndexBuffer& operator=(const IndexBuffer& other) noexcept;

        void Reset() noexcept override;

        void Bind(const vk::CommandBuffer& cmdBuffer, const vk::DeviceSize& offset) const;

        Types::uint GetIndicesCount() const noexcept;
    private:
        Types::uint m_IndicesCount;
    };
}