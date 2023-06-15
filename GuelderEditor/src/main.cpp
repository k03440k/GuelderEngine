//git push -f origin master

//#include <GuelderEngine/GuelderEngine.hpp>
//
//#include <GuelderEngine/Application.hpp>
//#include <GuelderEngine/Utils/ResourceManager.hpp>
//
//#include <memory>
//#define ENABLE_MEMORY_LEAKS_CHECKING
import GuelderEngine;

import GuelderEngine.Core;
import GuelderEngine.Core.Types;//TODO: HIDE PRIVATE SHIT
import GuelderEngine.Debug;
import GuelderEngine.Vulkan;

import <memory>;

#ifdef ENABLE_MEMORY_LEAKS_CHECKING

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

//#include <vld.h>

#endif // ENABLE_MEMORY_LEAKS_CHECKING
using namespace GuelderEngine;
using namespace Types;
using namespace Debug;
using Debug::ConsoleTextColor;
int main(int argc, char** argv)
{
    try
    {
        std::unique_ptr<GEApplication> app = std::make_unique<GEApplication>();

        //Utils::ResourceManager resManager(argv[0]);
        GuelderEngine::Vulkan::VulkanDebugManager m;

        app->Run([] {
            
            });
    }
    catch (const std::exception& e)
    {
        LogError(e.what());
    }

    LogWarning("Guelder Engine Editor closed successfully");
    
#ifdef ENABLE_MEMORY_LEAKS_CHECKING

    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    //_CrtDumpMemoryLeaks();

#endif // ENABLE_MEMORY_LEAKS_CHECKING
#ifndef GE_DEBUG
    //system("pause");
#endif // GE_DEBUG

    return 0;
}