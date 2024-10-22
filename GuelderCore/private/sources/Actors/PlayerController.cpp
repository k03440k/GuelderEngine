module;
//#include "../../../private/headers/Core/GObject/GClass.hpp"
#include <glfw/glfw3.h>
module GuelderEngine.Actors;
import :PlayerController;

import GuelderEngine.Core.Types;
import :Actor;
import :World;
import :CameraComponent;
import :KeyboardController;

import <algorithm>;
import <execution>;

namespace GuelderEngine
{
    /*void PlayerController::BindAction(Key key, const KeyCallback& func)
    {
        m_KeyActionsMap[key] = func;
    }
    void PlayerController::SetupInput()
    {
    }*/
    void PlayerController::Update(GLFWwindow* window, float deltaTime)
    {
        //add multithreading?
        /*std::for_each(std::execution::par, m_KeyActionsMap.begin(), m_KeyActionsMap.end(), [&deltaTime, &window]
        (const std::pair<std::string, InputKeyAction>& action)
            {
                if(GetKeyValue(window, action.second.key, action.second.state))
                    action.second.callback(deltaTime);
            });
        std::for_each(std::execution::par, m_KeyCombinationActionsMap.begin(), m_KeyCombinationActionsMap.end(), [&deltaTime, &window]
        (const std::pair<std::string, InputKeyCombinationAction>& action)
            {
                if(GetKeyCombinationValue(window, action.second.keyCombination, action.second.state))
                    action.second.callback(deltaTime);
            });*/
        Update(deltaTime);
        for(const auto& action : m_KeyActionsMap)
        {
            if(GetKeyValue(window, action.second.key, action.second.state))
                action.second.callback(deltaTime);
        }

        for(const auto& action : m_KeyCombinationActionsMap)
        {
            if(GetKeyCombinationValue(window, action.second.keyCombination, action.second.state))
                action.second.callback(deltaTime);
        }
    }
    bool PlayerController::GetKeyValue(GLFWwindow* window, const Keys& key, const InputKeyState& state)
    {
        switch(state)
        {
        case (InputKeyState::Down):
            return KeyboardController::IsKeyDown(window, key);
            break;
        case (InputKeyState::Pressed):
            return KeyboardController::IsKeyPressed(window, key);
            break;
        case (InputKeyState::Released):
            return KeyboardController::IsKeyReleased(window, key);
            break;
        }
    }
    bool PlayerController::GetKeyCombinationValue(GLFWwindow* window, const KeyCombination& keyCombination, const InputKeyState& state)
    {
        switch(state)
        {
        case (InputKeyState::Down):
            return KeyboardController::IsKeyCombinationDown(window, keyCombination);
            break;
        case (InputKeyState::Pressed):
            return KeyboardController::IsKeyCombinationPressed(window, keyCombination);
            break;
        case (InputKeyState::Released):
            break;
        }
    }
}