module;
#include <vulkan/vulkan.hpp>
module GuelderEngine.Actors;
import :GameMode;

import GuelderEngine.Core.Types;
import :Actor;
import :World;
import :PlayerController;

namespace GuelderEngine
{
    GameMode::GameMode(const PlayerController& playerController)
        : m_PlayerController(std::make_unique<PlayerController>(playerController))
    {
    }
}
namespace GuelderEngine
{
    void GameMode::BeginPlay()
    {
        m_PlayerController->BeginPlay();
    }
}
namespace GuelderEngine
{
    void GameMode::SetPlayerController(UniquePtr<PlayerController>&& playerController)
    {
        m_PlayerController = std::move(playerController);
    }
    const UniquePtr<PlayerController>& GameMode::GetPlayerController() const
    {
        return m_PlayerController;
    }
}