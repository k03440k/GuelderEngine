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
        // left face (white)
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

        // right face (yellow)
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

        // top face (orange, remember y axis points down)
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

        // tail face (green)
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    };

    for(auto& v : vertices) {
        v.pos += offset;
    }

    Indices indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                          12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

    return Mesh3D{vertices, indices };
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

    bool IsComplete() const noexcept override { return false; }

    void Update() override
    {
        
    }

    CameraComponent cameraComponent;
};
class MyPlayerController : public PlayerController
{
public:
    DECLARE_DEFAULT_COPYING_AND_MOVING(MyPlayerController);
    MyPlayerController() = default;
    ~MyPlayerController() override = default;

    void Update(GLFWwindow* window, float deltaTime) override
    {
        Vector3 rotate{ 0 };
        if(KeyboardController::IsKeyPressed(window, Keys::Up))
            rotate.x += 1.f;
        if(KeyboardController::IsKeyPressed(window, Keys::Down))
            rotate.x -= 1.f;
        if(KeyboardController::IsKeyPressed(window, Keys::Right))
            rotate.y += 1.f;
        if(KeyboardController::IsKeyPressed(window, Keys::Left))
            rotate.y -= 1.f;

        if(glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) 
            camera->transform.rotation += lookSpeed * deltaTime * glm::normalize(rotate);
        
        camera->transform.rotation.x = glm::clamp(camera->transform.rotation.x, -1.5f, 1.5f);
        camera->transform.rotation.y = glm::mod(camera->transform.rotation.y, glm::two_pi<float>());

        float yaw = camera->transform.rotation.y;
        const Vector3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
        const Vector3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
        const Vector3 upDir{ 0.f, -1.f, 0.f };

        Vector3 moveDir{ 0.f };
        if(KeyboardController::IsKeyPressed(window, Keys::W)) moveDir += forwardDir;
        if(KeyboardController::IsKeyPressed(window, Keys::S)) moveDir -= forwardDir;
        if(KeyboardController::IsKeyPressed(window, Keys::D)) moveDir += rightDir;
        if(KeyboardController::IsKeyPressed(window, Keys::A)) moveDir -= rightDir;
        if(KeyboardController::IsKeyPressed(window, Keys::Z)) moveDir += upDir;
        if(KeyboardController::IsKeyPressed(window, Keys::X)) moveDir -= upDir;

        if(glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
            camera->transform.translation += moveSpeed * deltaTime * glm::normalize(moveDir);
    }
    float moveSpeed = 1.f;
    float lookSpeed = 1.f;
};
class MyGameMode : public GameMode
{
public:
    DECLARE_DEFAULT_COPYING_AND_MOVING(MyGameMode);
    MyGameMode(CameraComponent& cameraComponent)
    {
        m_PlayerController = std::move(std::make_unique<MyPlayerController>(MyPlayerController{}));
        cameraComponent.SetViewTarget(Vector3{ -1.f, -5.f, 2.f }, Vector3{ 0.f, 0.f, .5f });
        //TODO: add checking that if the camera is from an actor which exists in world vector
        m_PlayerController->camera = &cameraComponent;
    }
    ~MyGameMode() override = default;
};

int main(int argc, char** argv)
{
    try
    {
        const auto app = std::make_unique<GEApplication>(argv[0]);

        auto& world = app->GetWorld();

        auto cameraActor = MakeActor(CameraActor{});
        world->SpawnActor3D(cameraActor);
        auto& cameraComponent = cameraActor->cameraComponent;

        app->gameMode = std::move(std::make_unique<MyGameMode>(MyGameMode{ cameraComponent }));

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


                        CreateCube3DMesh({0, 0.f, 0}),

                        {
                            {0, 0, .5f},
                            {.4f, 1.5f, .4f},//scale
                            {2,1,1},//position
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
                            {2,1,.5f},//position
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
                            {2,1,.5f},//position
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