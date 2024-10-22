module;
#include <vulkan/vulkan.hpp>
#include "../../../../public/GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :DescriptorPool;

import GuelderEngine.Core.Types;
import <vector>;

namespace GuelderEngine::Vulkan
{
    DescriptorPool::DescriptorPool(
        const vk::Device& device,
        const std::vector<vk::DescriptorPoolSize>& poolSizes,
        const uint& maxSets,
        const vk::DescriptorPoolCreateFlags& flags
    )
        : m_MaxSets(maxSets)
    {
        const vk::DescriptorPoolCreateInfo info{ flags, maxSets, (uint)poolSizes.size(), poolSizes.data() };

        GE_ASSERT(device.createDescriptorPool(&info, nullptr, &m_DescriptorPool) == vk::Result::eSuccess, "Failed to create descriptor pool");
    }

    DescriptorPool::DescriptorPool(DescriptorPool&& other) noexcept
    {
        m_DescriptorPool = other.m_DescriptorPool;
        m_MaxSets = other.m_MaxSets;

        other.Reset();
    }
    DescriptorPool& DescriptorPool::operator=(DescriptorPool&& other) noexcept
    {
        m_DescriptorPool = other.m_DescriptorPool;
        m_MaxSets = other.m_MaxSets;

        other.Reset();

        return *this;
    }
}
namespace GuelderEngine::Vulkan
{
    void DescriptorPool::Reset() noexcept
    {
        m_DescriptorPool = nullptr;
        m_MaxSets = 0;
    }
    void DescriptorPool::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyDescriptorPool(m_DescriptorPool);
    }
}
namespace GuelderEngine::Vulkan
{
    uint DescriptorPool::GetMaxSets() const noexcept
    {
        return m_MaxSets;
    }
    const vk::DescriptorPool& DescriptorPool::GetDescriptorPool() const noexcept
    {
        return m_DescriptorPool;
    }
}