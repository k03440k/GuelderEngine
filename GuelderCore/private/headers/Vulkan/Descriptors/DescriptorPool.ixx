module;
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
#include "../../../../public/GuelderEngine/Utils/Debug.hpp"
export module GuelderEngine.Vulkan:DescriptorPool;

import GuelderEngine.Core.Types;
import :IVulkanObject;
import <vector>;
import :Buffer;

export namespace GuelderEngine::Vulkan
{
    class DescriptorPool : public IVulkanObject
    {
    public:
        DECLARE_DEFAULT_CTOR_AND_DTOR(DescriptorPool);
        DECLARE_DEFAULT_COPYING(DescriptorPool);
        DECLARE_MOVING(DescriptorPool);

        DescriptorPool(
            const vk::Device& device,
            const std::vector<vk::DescriptorPoolSize>& poolSizes,
            const uint& maxSets,
            const vk::DescriptorPoolCreateFlags& flags = vk::DescriptorPoolCreateFlags{}
        );

        //very raw
        template<typename... Types>
        std::vector<vk::DescriptorSet> CreateDescriptorSets(
            const vk::Device& device,
            const std::vector<Buffers::Buffer*>& buffers,
            const std::vector<vk::DescriptorSetLayout>& layouts,
            const std::vector<vk::DescriptorType>& descriptorsTypes
        ) const
        {
            std::vector<vk::DescriptorSet> sets(buffers.size());
            const vk::DescriptorSetAllocateInfo allocateInfo{ m_DescriptorPool, buffers.size(), layouts.data() };

            GE_ASSERT(device.allocateDescriptorSets(&allocateInfo, sets.data()) == vk::Result::eSuccess, "Failed to allocate descriptor sets");

            auto typesSizes = GetSizesOfTypes<Types>();

            for (uint i = 0; i < buffers.size(); i++)
            {
                vk::DescriptorBufferInfo bufferInfo{ buffers[i]->GetBuffer(), 0, typesSizes[i] };

                vk::WriteDescriptorSet writeDescriptorSet{ sets[i], 0, 0, 1, descriptorsTypes[i], nullptr, &bufferInfo, nullptr };

                device.updateDescriptorSets(1, &writeDescriptorSet, 0, nullptr);
            }

            return sets;
        }

        void Reset() noexcept override;
        void Cleanup(const vk::Device& device) const noexcept;

        uint GetMaxSets() const noexcept;
        const vk::DescriptorPool& GetDescriptorPool() const noexcept;

    private:
        template<typename... Types>
        static std::array<uint, sizeof...(Types)> GetSizesOfTypes()
        {
            return { sizeof(Types)... };
        }

        vk::DescriptorPool m_DescriptorPool;
        uint m_MaxSets;
    };
}