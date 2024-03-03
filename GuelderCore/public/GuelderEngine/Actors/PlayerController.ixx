module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
export module GuelderEngine.Actors:PlayerController;

import GuelderEngine.Core.Types;
import :Actor;
import :World;
import :CameraComponent;

export namespace GuelderEngine
{
    class PlayerController
    {
    public:
        PlayerController() = default;
        virtual ~PlayerController() = default;
        DECLARE_DEFAULT_COPYING_AND_MOVING(PlayerController);

        CameraComponent* camera;
    };
}