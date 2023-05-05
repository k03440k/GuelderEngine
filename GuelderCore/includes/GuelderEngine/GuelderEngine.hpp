#pragma once

//#define GLFW_INCLUDE_VULKAN
//#include <glfw/glfw3.h>
//#include <vulkan/vulkan.hpp>
#include "Utils/Debug.hpp"
#include "Utils/Utils.hpp"

#define GE_USING_NAMESPACES_AND_CLASSES

#ifdef GE_USING_NAMESPACES_AND_CLASSES

using namespace GuelderEngine;
using namespace Utils;
using Debug::Logger;
using Debug::LogLevel;

#endif // GE_USING_NAMESPACES_AND_CLASSES