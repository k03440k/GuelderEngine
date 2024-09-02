module;
#include <vulkan/vulkan.hpp>
#include "../../../../public/GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Vulkan;
import :DescriptorSetLayout;

import GuelderEngine.Core.Types;
import <vector>;

namespace GuelderEngine::Vulkan
{
    DescriptorSetLayout(
        const vk::Device& device,
        const std::vector<vk::DescriptorSetLayoutBinding>& bindings
    )
    {   
        const vk::DescriptorSetLayoutCreateInfo info{(uint)bindings.size(), bindings.data()};
        
        m_Layout = device.createDescriptorSetLayout(info);
    }
    DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout&& other) noexcept
    {
        m_Layout = other.m_Layout;
        
        other.Reset();
    }
    DescriptorSetLayout& DescriptorSetLayout::operator=(DescriptorSetLayout&& other) noexcept
    {
        m_Layout = other.m_Layout;
        
        other.Reset();
        
        return *this;
    }
    void DescriptorSetLayout::Reset() noexcept
    {
        m_Layout = nullptr;
    }
    void DescriptorSetLayout::Cleanup(const vk::Device& device) const noexcept
    {
        device.destroyDescriptorSetLayout(m_Layout);
    }
}