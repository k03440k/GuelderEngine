//git push -f origin master

//#define ENABLE_MEMORY_LEAKS_CHECKING
import GuelderEngine;

#include <GuelderEngine/Utils/Debug.hpp>

import <memory>;
import <string_view>;

#ifdef ENABLE_MEMORY_LEAKS_CHECKING

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include <vld.h>

#endif // ENABLE_MEMORY_LEAKS_CHECKING

using namespace GuelderEngine;
using namespace Types;
using namespace Debug;
using Debug::ConsoleForegroundColor;

DECLARE_LOG_CATEGORY_EXTERN(User, All);
DEFINE_LOG_CATEGORY(User);

int main(int argc, char** argv)
{
    try
    {
        auto app = std::make_unique<GEApplication>();

        //Utils::ResourceManager resManager(argv[0]);

        //app->Run([] {});
    }
    catch (const std::exception& e)
    {
        LogError(e.what());
    }

    GE_LOG(User, Warning, "Guelder Engine Editor closed");
    
#ifdef ENABLE_MEMORY_LEAKS_CHECKING

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();

#endif // ENABLE_MEMORY_LEAKS_CHECKING
#ifndef GE_DEBUG
    //system("pause");
#endif // GE_DEBUG

    return 0;
}