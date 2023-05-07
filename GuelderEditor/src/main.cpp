#include <GuelderEngine/GuelderEngine.hpp>

#include <GuelderEngine/Application.hpp>
#include <GuelderEngine/Utils/ResourceManager.hpp>

#include <memory>

#define ENABLE_MEMORY_LEAKS_CHECKING

#ifdef ENABLE_MEMORY_LEAKS_CHECKING

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif // ENABLE_MEMORY_LEAKS_CHECKING

int main(int argc, char** argv)
{
	try
	{
		Utils::ResourceManager resManager(argv[0]);
		
		std::unique_ptr<GEApplication> app = std::make_unique<GEApplication>();
		app->Run([] {
			
			});
	}
	catch (const std::exception& e)
	{
		LogError(e.what());
	}

	LogWarning("Guelder Engine Editor closed successfully");

#ifdef ENABLE_MEMORY_LEAKS_CHECKING

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

#endif // ENABLE_MEMORY_LEAKS_CHECKING
    return 0;
}