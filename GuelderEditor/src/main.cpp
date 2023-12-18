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

int main(int argc, char** argv)
{
    try
    {
        const auto app = std::make_unique<GEApplication>(argv[0]);

        const Vulkan::Mesh2D cube = 
        {
            {
               {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
               {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
               {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
               {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
            },

            {0, 1, 2, 2, 3, 0}
        };

        const Vulkan::Mesh2D triangle =
        {
            {
               {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
               {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
               {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
            },

            {0, 1, 2}
        };

        class MyActor : public Object2D
        {
        public:
            MyActor() = default;
            MyActor(const Object2DCreateInfo& info)
                : Object2D(info) {}

            void Update() override
            {
                transform.rotation = glm::mod(transform.rotation + .001f, glm::two_pi<float>());
            }
        };

        //TODO: think about pointer system, especially about && in MakeActor func and also find out about MATH

        //app->SetMesh(cube);
        app->SpawnRenderActor(MakeActor(MyActor{ Object2DCreateInfo{ {{},{.8f, .5f},{},.25f * glm::two_pi<float>()}, cube} }));
        //app->SpawnRenderActor(triangle);

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