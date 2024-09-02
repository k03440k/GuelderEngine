module;
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:DescriptorPool;

import GuelderEngine.Core.Types;
import :IVulkanObject;
import <vector>;

export namespace GuelderEngine::Vulkan
{
	class DescriptorPool : public IVulkanObject
    {
        public:
            DECLARE_DEFAULT_CTOR_AND_DTOR(DescriptorPool);
            DECLARE_DEFAULT_COPYING(DescriptorPool);
            DECLARE_MOVING(DescriptorPool);
            
            DescriptorPool(
                const std::vector<vk::DescriptorPoolSize>& poolSizes,
                const uint& maxSets,
                const vk::DescriptorPoolCreateFlags& flags = vk::DescriptorPoolCreateFlags{}
                );
            
            void Reset() noexcept override;
            void Cleanup(const vk::Device& device) const noexcept;
            
            uint GetMaxSets() const noexcept;
            
        private:
        vk::DescriptorPool m_Pool;
        uint m_MaxSets;
    };
}