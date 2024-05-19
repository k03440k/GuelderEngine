//TODO: make smth like in UE4. To add a Mesh component
//TODO: rework system of meshes, so to make a mesh component responsible for that
module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Actors:ActorComponent;

export import :GObject;
import :RenderActorTransform;

export namespace GuelderEngine
{
    template<uint, typename RotationType = float>
    class SceneComponent;

    using SceneComponent2D = SceneComponent<2>;
    using SceneComponent3D = SceneComponent<3, Vector3>;

    class ActorComponent : public GObjectComponent
    {
    public:
        ActorComponent() = default;
        ~ActorComponent() override = default;

        virtual void BeginPlay() {}
        virtual void Update(float deltaTime) {}//make some sort of primitive component without update method
    };
    template<uint dimension, typename RotationType = float>
    class SceneComponent : public ActorComponent
    {
    public:
        using Transform = RenderActorTransform<dimension, RotationType>;
    public:
        SceneComponent() = default;
        ~SceneComponent() override = default;

        Transform transform;
    };
}