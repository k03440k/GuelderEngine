//git push -f origin master

//#define ENABLE_MEMORY_LEAKS_CHECKING

#pragma warning(disable : 5103)
#pragma warning(disable : 4844)

#ifdef ENABLE_MEMORY_LEAKS_CHECKING

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include <vld.h>

#endif // ENABLE_MEMORY_LEAKS_CHECKING

#define GE_USING_NAMESPACES

#include <GuelderEngine/GuelderEngine.hpp>
#include <glm/gtc/constants.hpp>

import <memory>;

DECLARE_LOG_CATEGORY_EXTERN(Editor, All, true);
DEFINE_LOG_CATEGORY(Editor);

// temporary helper function, creates a 1x1x1 cube centered at offset
Mesh3D CreateCube3DMesh(const Vector3& offset)
{
    Vertices3D vertices
    {

      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

      // top face (orange, remember y axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

    };


    for(auto& v : vertices) {
        v.pos += offset;
    }

    return Mesh3D{vertices};
}

const Mesh2D cube =
{
    {
       {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
       {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
       {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
       {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    },

    {
        0, 1, 2,
        2, 3, 0
    }
};
const Mesh2D triangle =
{
    {
       {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
       {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
       {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    },

    {0, 1, 2}
};
const Mesh2D hexagon =
{
    {
        {{ 0.0f,  0.5f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.25f}, {1.0f, 1.0f, 0.0f}},
        {{-0.5f, -0.25f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.0f, -0.5f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.25f}, {0.0f, 1.0f, 1.0f}},
        {{ 0.5f,  0.25f}, {1.0f, 0.0f, 1.0f}}
    },

    {
        0, 1, 5, // Triangle 1
        1, 2, 5, // Triangle 2
        2, 3, 5, // Triangle 3
        3, 4, 5  // Triangle 4
    }
};

class MyActor2D : public Actor2D
{
public:
    MyActor2D() = default;
    MyActor2D(const Vulkan::Mesh2D& mesh, const Actor2DTransform& transform)
        : Actor2D(mesh, transform) {}

    void Update() override
    {
        transform.rotation = glm::mod(transform.rotation + .001f, glm::two_pi<float>());
        transform.position.y = .2f * sin(GEApplication::GetTime());
    }
};
class MyActor3D : public Actor3D
{
public:
    MyActor3D() = default;
    MyActor3D(const Vulkan::Mesh3D& mesh, const Actor3DTransform& transform)
        : Actor3D(mesh, transform) {}

    void Update() override
    {
        transform.rotation.y = glm::mod(transform.rotation.y + .0001f, glm::two_pi<float>());
        transform.rotation.x = glm::mod(transform.rotation.x + .001f, glm::two_pi<float>());
        transform.rotation.z = glm::mod(transform.rotation.z + .001f, glm::two_pi<float>());

        transform.position.x = glm::mod(transform.position.x + .1f, glm::two_pi<float>());
        transform.position.y = glm::mod(transform.position.y + .1f, glm::two_pi<float>());
    }
};
class CameraActor : public Actor3D
{
public:
    CameraActor(const CameraComponent& cameraComponent = CameraComponent{})//it cannot find CameraActor
    : cameraComponent(cameraComponent) {}

    void Update() override
    {
        
    }

    CameraComponent cameraComponent;
};

int main(int argc, char** argv)
{
    try
    {
        const auto app = std::make_unique<GEApplication>(argv[0]);

        auto world = app->GetWorld();

        auto cameraActor = CameraActor{};
        auto& cameraComponent = cameraActor.cameraComponent;
        cameraComponent.SetViewTarget(Vector3{-1.f, -2.f, 2.f}, Vector3{0.f, 0.f, .5f});
        //TODO: add checking that if the camera is from an actor which exists in world vector
        app->GetGameMode()->GetPlayerController()->camera = &cameraComponent;

        //world->SpawnActor3D
        //(
        //    MakeActor
        //    (
        //        MyActor3D
        //        {

        //                CreateCube3DMesh({0, 0, 0}),
        //                {
        //                    {0, 0, .5f},
        //                    {.1f, .1f, .1f},//scale
        //                    {1,1,1},//position
        //                    {0, 0, 0},//rotation
        //                }

        //            
        //        }
        //    )
        //);

        //pencil
        world->SpawnActor3D
        (
            MakeActor
            (
                MyActor3D
                {


                        CreateCube3DMesh({0, 0, 0}),

                        {
                            {0, 0, .5f},
                            {.4f, 1.5f, .4f},//scale
                            {1,1,1},//position
                            {0, 0, 0},//rotation
                        }

                }
            )
        );
        world->SpawnActor3D
        (
            MakeActor
            (
                MyActor3D
                {
                        CreateCube3DMesh({1.0f, 1.38f, 0}),

                        {
                            {0, 0, .5f},
                            {.4f, .4f, .4f},//scale
                            {1,1,.5f},//position
                            {0, 0, 0},//rotation
                        }

                }
            )

        );
        world->SpawnActor3D
        (
            MakeActor
            (
                MyActor3D
                {

                        CreateCube3DMesh({-1.0f, 1.38f, 0}),

                        {
                            {0, 0, .5f},
                            {.4f, .4f, .4f},//scale
                            {1,1,.5f},//position
                            {0, 0, 0},//rotation
                        }

                }
            )

        );
        //world->SpawnActor2D
        //(
        //    MakeActor
        //    (
        //        MyActor2D
        //        {

        //                triangle,

        //                {
        //                    {},
        //                    {.2f, .2f},//scale
        //                    {.5f, 1.f},//position
        //                    0.f//rotation
        //                }

        //        }
        //    )
        //);
        //world->SpawnActor2D
        //(
        //    MakeActor
        //    (
        //        MyActor2D
        //        {
        //                cube,

        //                {
        //                    {},
        //                    {.2f, .2f},//scale
        //                    {-.5f, 1.f},//position
        //                    0.f//rotation
        //                }

        //        }
        //    )
        //);

        app->Run();
    }
    catch(const std::exception& e)
    {
        GE_LOG(Editor, Error, e.what());
    }

    GE_LOG(Editor, Warning, "Guelder Engine Editor has just closed successfully");

#ifdef ENABLE_MEMORY_LEAKS_CHECKING

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();

#endif // ENABLE_MEMORY_LEAKS_CHECKING

#ifndef GE_DEBUG
    system("pause");
#endif // GE_DEBUG

    return 0;
}