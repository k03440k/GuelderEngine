//#define GLFW_INCLUDE_VULKAN
//#include <glfw/glfw3.h>
//#include <vulkan/vulkan.hpp>
//#include "Utils/Debug.hpp"
//#include "Utils/Utils.hpp"
//#include "../src/headers/Core/GObject/GClass.hpp"

export module GuelderEngine;

export import GuelderEngine.Core;
//export import :ResourceManager;
//export import :Window;
//export import :Application;
//export import GuelderEngine.Types;
//export import GuelderEngine.Types:GClass;
export import GuelderEngine.Debug;
//GuelderEngine::GEApplication a;
#define GE_USING_NAMESPACES_AND_CLASSES

#ifdef GE_USING_NAMESPACES_AND_CLASSES

//export using namespace GuelderEngine;
//export using namespace Utils;
//export using namespace Debug;
//export using Debug::ConsoleTextColor;
//using Debug::Logger;
//using Debug::LogLevel;

#endif // GE_USING_NAMESPACES_AND_CLASSES