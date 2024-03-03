module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include <vulkan/vulkan.hpp>
export module GuelderEngine.Actors:GameMode;

import GuelderEngine.Core.Types;
import :Actor;
import :PlayerController;

export namespace GuelderEngine
{
    class GameMode
    {
    public:
        GameMode(const PlayerController& playerController = {});
        virtual ~GameMode() = default;
        DECLARE_DEFAULT_COPYING_AND_MOVING(GameMode);
		
		virtual void BeginPlay();
		
        const UniquePtr<PlayerController>& GetPlayerController() const;

    private:
        UniquePtr<PlayerController> m_PlayerController;
    };
}