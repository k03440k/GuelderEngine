module;
#include <Windows.h>
#include "Utils/Debug.hpp"
export module GuelderEngine;

import <mutex>;
import <memory>;

export import GuelderEngine.Core;
export import GuelderEngine.Utils;
export import GuelderEngine.Core.Types;
export import GuelderEngine.Debug;
export import GuelderEngine.Actors;
export import GuelderEngine.Vulkan;
export import GuelderEngine.Events;
export import GuelderEngine.UserInterface;

std::mutex GuelderEngine::Debug::Logger::logMutex;
HANDLE GuelderEngine::Debug::Logger::console = GetStdHandle(STD_OUTPUT_HANDLE);
namespace GuelderEngine::Vulkan { DEFINE_LOG_CATEGORY(VulkanCore); }
const GuelderEngine::Vulkan::VulkanManager GuelderEngine::GEApplication::m_VulkanManager = GuelderEngine::Vulkan::VulkanManager{ "Guelder Engine Editor" };