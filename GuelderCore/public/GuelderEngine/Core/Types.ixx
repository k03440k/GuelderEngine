module;
#include <glm/glm.hpp>
export module GuelderEngine.Core.Types;

//export import :GClass;

import <vector>;
import <memory>;

export namespace GuelderEngine
{
    using byte = char;
    using cstr = char*;
    using ubyte = unsigned char;
    using ushort = unsigned short;
    using uint = unsigned int;

    template<uint dimension>
    using Vector = glm::vec<dimension, float, glm::defaultp>;
    using Vector2 = Vector<2>;
    using Vector3 = Vector<3>;

    template<uint dimension>
    using Mat = glm::mat<dimension, dimension, glm::f32, glm::defaultp>;
    using Mat2 = Mat<2>;
    using Mat3 = Mat<3>;
    using Mat4 = Mat<4>;

    //uses std::shared_ptr<T>
    template<class T>
    using SharedPtr = std::shared_ptr<T>;

    //uses std::weak_ptr<T>
    template<class T>
    using WeakPtr = std::weak_ptr<T>;

    //uses std::unique_ptr<T>
    template<class T>
    using UniquePtr = std::unique_ptr<T>;
}