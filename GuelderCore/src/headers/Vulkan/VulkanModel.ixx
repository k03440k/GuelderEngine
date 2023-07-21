module;
#include <glm/glm.hpp>
export module GuelderEngine.Vulkan:VulkanModel;

export namespace GuelderEngine::Vulkan
{
    struct VulkanModel
    {
        glm::mat4 model;
    };
}