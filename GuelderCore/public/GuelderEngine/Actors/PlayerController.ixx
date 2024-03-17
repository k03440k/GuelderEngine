module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include <glfw/glfw3.h>
export module GuelderEngine.Actors:PlayerController;

import GuelderEngine.Core.Types;
import :Actor;
import :World;
import :CameraComponent;

import <unordered_map>;

export namespace GuelderEngine
{
    //temp
    using Key = int;
    class PlayerController
    {
    public:
        using KeyCallback = void(*);
    public:
        PlayerController() = default;
        virtual ~PlayerController() = default;
        DECLARE_DEFAULT_COPYING_AND_MOVING(PlayerController);

        //void BindAction(Key key, const KeyCallback& func);
        //virtual void SetupInput();
        virtual void Update(GLFWwindow* window, float deltaTime);

        CameraComponent* camera;
    private:
        //friend class GEApplication;
        //std::unordered_map<Key, KeyCallback> m_KeyActionsMap;
    };
}