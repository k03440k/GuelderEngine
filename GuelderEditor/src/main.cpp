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
import <unordered_set>;
import <iostream>;
#include <random>

DECLARE_LOG_CATEGORY_EXTERN(Editor, All, true);
DEFINE_LOG_CATEGORY(Editor);

#include "CustomGame.hpp"

int main(int argc, char** argv)
{
    try
    {
        const auto app = std::make_unique<GEApplication>(argv[0]);

        auto& world = app->GetWorld();

        auto cameraActor = MakeActor(CameraActor{});
        world->SpawnActor3D(cameraActor);
        auto& cameraComponent = cameraActor->cameraComponent;

        app->gameMode = std::move(std::make_unique<MyGameMode>(MyGameMode{ cameraComponent, world.get() }));

        world->SpawnActor3D
        (
            MakeActor
            (
                MyActor3D
                {

                        CreateCube3DMesh({0, 0, 0}),
                        {
                            {0, 0, .5f},
                            {.1f, .3f, .1f},//scale
                            {1,1,1},//position
                            {0, 0, 0},//rotation
                        }


                },
                "cube"
            )
        );

        //GE_LOG(Editor, Info, "Input number of 3D cubes to be spawned");
        uint countOfNewActors = 50;
        //std::cin >> countOfNewActors;

        SpawnALotOfStuff(world.get(), countOfNewActors);
        //Spawn2Ds(world.get());

        GE_LOG(Editor, Info, "Threre are ", world->GetAllActors().size(), " actors on the scene.");

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