//git push -f origin master

//#define ENABLE_MEMORY_LEAKS_CHECKING

#ifdef ENABLE_MEMORY_LEAKS_CHECKING

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include <vld.h>

#endif // ENABLE_MEMORY_LEAKS_CHECKING

#include <GuelderEngine/GuelderEngine.hpp>

import <memory>;
import <string_view>;

DECLARE_LOG_CATEGORY_EXTERN(Editor, All);
DEFINE_LOG_CATEGORY(Editor);

int main(int argc, char** argv)
{
    try
    {
        auto app = std::make_unique<GEApplication>(argv[0]);
        
        //GE_LOG(Editor, Info, '\n', (app->resourceManager.FindResourcesVariableFileContent("vert_code")));
        //GE_LOG(Editor, Info, '\n', (app->resourceManager.FindResourcesVariableFileContent("frag_code")));

        app->Run([] {});
    }
    catch (const std::exception& e)
    {
        LogError(e.what());
    }

    GE_LOG(Editor, Warning, "Guelder Engine Editor closed");
    
#ifdef ENABLE_MEMORY_LEAKS_CHECKING

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();

#endif // ENABLE_MEMORY_LEAKS_CHECKING
#ifndef GE_DEBUG
    system("pause");
#endif // GE_DEBUG

    return 0;
}