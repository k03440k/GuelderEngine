module;
#include <vulkan/vulkan.hpp>
#include "../../../../public/GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :DescriptorPool;

import GuelderEngine.Core.Types;
import <vector>;

namespace GuelderEngine::Vulkan
{
    DescriptorPool(
        const std::vector<vk::DescriptorPoolSize>& poolSizes,
        const uint& maxSets,
        const vk::DescriptorPoolCreateFlags& flags = vk::DescriptorPoolCreateFlags{}
    )
        : m_MaxSets(maxSets)
    {
        const vk::DescriptorPoolCreateInfo info{(uint)poolSizes.size(), poolSizes.data(), maxSets, flags);
        
        GE_ASSERT(device.createDescriptorPool(&info, nullptr, &m_Pool), "Failed to create descriptor pool");
    }

    DescriptorPool::DescriptorPool(DescriptorPool&& other) noexcept
    {
        m_Pool = other.m_Pool;
        
        other.Reset();
    }
    DescriptorPool& DescriptorPool::operator=(DescriptorPool&& other) noexcept
    {
        m_Pool = other.m_Pool;
        
        other.Reset();
        
        return *this;
    }
    void DescriptorPool::Reset() noexcept
    {
        m_Pool = nullptr;
    }
    void DescriptorPool::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyDescriptorPool(m_Pool);
    }
    uint GetMaxSets() const noexcept
    {
        return m_MaxSets;
    }
}