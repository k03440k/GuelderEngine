module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
module GuelderEngine.Actors;
import :GameMode;

import GuelderEngine.Core.Types;
import :Actor;
import :World;
import :PlayerController;

namespace GuelderEngine
{
    const UniquePtr<PlayerController>& GameMode::GetPlayerController() const
    {
        return m_PlayerController;
    }
    GameMode::GameMode(const PlayerController& playerController)
        : m_PlayerController(std::make_unique<PlayerController>(playerController))
    {
    }
    void GameMode::BeginPlay()
    {

    }
}