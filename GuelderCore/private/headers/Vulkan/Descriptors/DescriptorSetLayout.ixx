module;
#include <vulkan/vulkan.hpp>
#include "../../Core/GObject/GClass.hpp"
export module GuelderEngine.Vulkan:DescriptorSetLayout;

import GuelderEngine.Core.Types;
import :IVulkanObject;
import <vector>;

export namespace GuelderEngine::Vulkan
{
	class DescriptorSetLayout : public IVulkanObject
    {
        public:
            DECLARE_DEFAULT_CTOR_AND_DTOR(DescriptorSetLayout);
            DECLARE_DEFAULT_COPYING(DescriptorSetLayout);
            DECLARE_MOVING(DescriptorSetLayout);
            
            DescriptorSetLayout(
                const vk::Device& device,
                const std::vector<vk::DescriptorSetLayoutBinding>& bindings
            );
            
            void Reset() noexcept override;
            void Cleanup(const vk::Device& device) const noexcept;
            
        private:
        vk::DescriptorSetLayout m_Layout;
    };
}