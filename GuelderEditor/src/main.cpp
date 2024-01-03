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

    Mesh3D toReturn{ vertices };

    return toReturn;
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

int main(int argc, char** argv)
{
    try
    {
        const auto app = std::make_unique<GEApplication>(argv[0]);

        class MyActor2D : public Object2D
        {
        public:
            MyActor2D() = default;
            MyActor2D(const Object2DCreateInfo& info)
                : Object2D(info) {}

            void Update() override
            {
                transform.rotation = glm::mod(transform.rotation + .001f, glm::two_pi<float>());
                transform.position.y = .2f * sin(GEApplication::GetTime());
            }
        };
        class MyActor3D : public Object3D
        {
        public:
            MyActor3D() = default;
            MyActor3D(const Object3DCreateInfo& info)
                : Object3D(info) {}

            void Update() override
            {
                transform.rotation.y = glm::mod(transform.rotation.y + .0001f, glm::two_pi<float>());
                transform.rotation.x = glm::mod(transform.rotation.x + .001f, glm::two_pi<float>());
                transform.rotation.z = glm::mod(transform.rotation.z + .001f, glm::two_pi<float>());

                transform.position.x = glm::mod(transform.position.x + .1f, glm::two_pi<float>());
                transform.position.y = glm::mod(transform.position.y + .1f, glm::two_pi<float>());
            }
        };

        app->SpawnActor3D
        (
            MakeActor
            (
                MyActor3D
                {
                    {
                        {
                            {0, 0, .5f},
                            {.4f, .4f, .4f},//scale
                            {1,1,1},//position
                            {0, 0, 0},//rotation
                        },

                        CreateCube3DMesh({0, 0, 0})
                    }
                }
            )
        );

        //pencil
        /*app->SpawnActor3D
(
    MakeActor
    (
        MyActor3D
        {
            {
                {
                    {0, 0, .5f},
                    {.4f, 1.5f, .4f},//scale
                    {1,1,1},//position
                    {0, 0, 0},//rotation
                },

                CreateCube3DMesh({0, 0, 0})
            }
        }
    )
);
app->SpawnActor3D
(
    MakeActor
    (
        MyActor3D
        {
            {
                {
                    {0, 0, .5f},
                    {.4f, .4f, .4f},//scale
                    {1,1,.5f},//position
                    {0, 0, 0},//rotation
                },

                CreateCube3DMesh({1.0f, 1.38f, 0})
            }
        }
    )
);
app->SpawnActor3D
(
    MakeActor
    (
        MyActor3D
        {
            {
                {
                    {0, 0, .5f},
                    {.4f, .4f, .4f},//scale
                    {1,1,.5f},//position
                    {0, 0, 0},//rotation
                },

                CreateCube3DMesh({-1.0f, 1.38f, 0})
            }
        }
    )
);*/
        /*app->SpawnActor2D
        (
            MakeActor
            (
                MyActor2D
                {
                    {
                        {
                            {},
                            {.2f, .2f},//scale
                            {.5f, 1.f},//position
                            0.f//rotation
                        },

                        triangle
                    }
                }
            )
        );
        app->SpawnActor2D
        (
            MakeActor
            (
                MyActor2D
                {
                    {
                        {
                            {},
                            {.2f, .2f},//scale
                            {-.5f, 1.f},//position
                            0.f//rotation
                        },

                        cube
                    }
                }
            )
        );*/

        app->Run();
    }
    catch (const std::exception& e)
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