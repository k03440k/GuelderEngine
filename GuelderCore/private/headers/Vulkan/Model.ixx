module;
#include <glm/glm.hpp>
export module GuelderEngine.Vulkan:Model;

export namespace GuelderEngine::Vulkan
{
    struct Model
    {
        glm::mat4 model;
    };
}