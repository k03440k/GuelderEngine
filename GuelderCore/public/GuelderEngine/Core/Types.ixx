module;
#include <glm/glm.hpp>
export module GuelderEngine.Core.Types;

//export import :GClass;

import <vector>;

export namespace GuelderEngine
{
    namespace Types
    {
        using byte = char;
        using cstr = char*;
        using ubyte = unsigned char;
        using ushort = unsigned short;
        using uint = unsigned int;

        using Vector2 = glm::vec2;
        using Vector3 = glm::vec3;
    }
}