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
        DECLARE_DEFAULT_COPYING_AND_MOVING(GameMode);

        GameMode(const PlayerController& playerController = {});
        virtual ~GameMode() = default;
		
		virtual void BeginPlay();

        void SetPlayerController(UniquePtr<PlayerController>&& playerController);
        const UniquePtr<PlayerController>& GetPlayerController() const;

    protected:
        UniquePtr<PlayerController> m_PlayerController;
    };
}