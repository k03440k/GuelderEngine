module;
#include "../../../private/headers/Core/GObject/GClass.hpp"
#include <glfw/glfw3.h>
#include "../Utils/Debug.hpp"
export module GuelderEngine.Actors:PlayerController;

import GuelderEngine.Core.Types;
import :Actor;
import :World;
import :CameraComponent;
import :KeyboardController;

import <unordered_map>;
import <string_view>;
import <string>;
import <functional>;
import <type_traits>;

export namespace GuelderEngine
{
    enum class InputKeyState
    {
        Down,
        Pressed,
        Released
    };
    struct InputKeyAction
    {
        Keys key;
        InputKeyState state;
        std::function<void(float)> callback;
    };
    struct InputKeyCombinationAction
    {
        KeyCombination keyCombination;
        InputKeyState state;
        std::function<void(float)> callback;
    };
    class PlayerController
    {
    public:
        template<typename _Actor>
        using KeyCallback = void (_Actor::*)(float);
    public:
        PlayerController() = default;
        virtual ~PlayerController() = default;
        DECLARE_DEFAULT_COPYING_AND_MOVING(PlayerController);

        template<typename UserClass>
        void BindKey(const Keys& key, const InputKeyState& inputType, const std::string_view& name, UserClass* user, const KeyCallback<UserClass>& callback)
        {
            m_KeyActionsMap[name.data()] = {key, inputType, [user, callback](float deltaTime) { (user->*callback)(deltaTime); }};
        }
        template<typename UserClass>
        void BindKeyCombination(const KeyCombination& keyCombination, const InputKeyState& inputType, const std::string_view& name, UserClass* user, const KeyCallback<UserClass>& callback)
        {
            m_KeyCombinationActionsMap[name.data()] = { keyCombination, inputType, [user, callback](float deltaTime) { (user->*callback)(deltaTime); } };
        }
        virtual void SetupInput() {}
        virtual void BeginPlay() {}
        /*virtual*/ void Update(GLFWwindow* window, float deltaTime);
        virtual void Update(float deltaTime) {}

        CameraComponent* camera;
    private:
        static bool GetKeyValue(GLFWwindow* window, const Keys& key, const InputKeyState& state);
        static bool GetKeyCombinationValue(GLFWwindow* window, const KeyCombination& keyCombination, const InputKeyState& state);

        std::unordered_map<std::string, InputKeyAction> m_KeyActionsMap;
        std::unordered_map<std::string, InputKeyCombinationAction> m_KeyCombinationActionsMap;
    };
}