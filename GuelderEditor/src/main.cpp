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

import <memory>;

DECLARE_LOG_CATEGORY_EXTERN(Editor, All);
DEFINE_LOG_CATEGORY(Editor);

int main(int argc, char** argv)
{
    try
    {
        const auto app = std::make_unique<GEApplication>(argv[0]);

        app->Run([&]
        {

        });
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