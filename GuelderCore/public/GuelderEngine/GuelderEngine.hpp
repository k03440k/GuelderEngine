#pragma once

//NOTE: Every time when you see "#include <compare>" this means that some stupid bug has occured with either with modules or vulkan that "looks for" header "compare"

#include "Utils/Debug.hpp"
#include "../private/headers/Core/GObject/GClass.hpp"
#include <glm/glm.hpp>

import GuelderEngine;

#ifdef GE_USING_NAMESPACES
using namespace GuelderEngine;
using namespace Debug;
using namespace Utils;
using namespace Events;
using namespace Vulkan;
#endif