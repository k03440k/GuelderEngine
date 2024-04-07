module;
#include <glfw/glfw3.h>
#include <windows.h>
#include "../../../public/GuelderEngine/Utils/Debug.hpp"
module GuelderEngine.Actors;
import :KeyboardController;

import <map>;
import <unordered_map>;

namespace GuelderEngine
{
    bool KeyboardController::IsKeyDown(const Keys& key)
    {
        if(key == Keys::None)
            return true;
        static std::map<int, bool> isKeyPressed;

        auto keyCode = static_cast<int>(key);

        if(GetKeyState(keyCode) & 0x8000 && !isKeyPressed[keyCode])
        {
            isKeyPressed[keyCode] = true;
            return true;
        }
        else if(!(GetKeyState(keyCode) & 0x8000))
        {
            isKeyPressed[keyCode] = false;
        }

        return false;
    }
    bool KeyboardController::IsKeyDown(GLFWwindow* window, const Keys& key)
    {
        if(key == Keys::None)
            return true;
        static std::map<int, bool> isKeyPressed;

        auto keyCode = static_cast<int>(key);

        if(glfwGetKey(window, keyCode) == GLFW_PRESS && !isKeyPressed[keyCode])
        {
            isKeyPressed[keyCode] = true;
            return true;
        }
        else if(!(glfwGetKey(window, keyCode) == GLFW_PRESS))
        {
            isKeyPressed[keyCode] = false;
        }

        return false;
    }
    bool KeyboardController::IsKeyPressed(GLFWwindow* window, const Keys& key)
    {
        if(key == Keys::None)
            return true;
        return glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS;
    }
    bool KeyboardController::IsKeyPressed(const Keys& key)
    {
        if(key == Keys::None)
            return true;
        return GetKeyState(static_cast<int>(key)) & 0x8000;
    }
    bool KeyboardController::IsKeyReleased(GLFWwindow* window, const Keys& key)
    {
        if(key == Keys::None)
            return true;
        static std::unordered_map<int, int> prevKeyStates;

        int state = glfwGetKey(window, static_cast<int>(key));
        if(state == GLFW_RELEASE && prevKeyStates[static_cast<int>(key)] == GLFW_PRESS) {
            prevKeyStates[static_cast<int>(key)] = state;
            return true;
        }
        prevKeyStates[static_cast<int>(key)] = state;
        return false;
    }
    bool KeyboardController::IsKeyReleased(const Keys& key)
    {
        //not sure if it works correctly
        if(key == Keys::None)
            return true;
        static std::unordered_map<int, int> prevKeyStates;

        int state = GetKeyState(static_cast<int>(key));
        if(!(state & 0x800) && prevKeyStates[static_cast<int>(key)] & 0x800) {
            prevKeyStates[static_cast<int>(key)] = state;
            return true;
        }
        prevKeyStates[static_cast<int>(key)] = state;
        return false;
    }
    bool KeyboardController::IsKeyCombinationDown(GLFWwindow* window, const KeyCombination& keyCombination)
    {
        bool areAllPressed = true;
        for(auto& keyToPress : keyCombination.keysToPress)
        {
            if(!IsKeyPressed(window, keyToPress))
            {
                areAllPressed = false;
                break;
            }
        }

        if(areAllPressed)
        {
            if(IsKeyDown(window, keyCombination.keyToDown))
            {
                return true;
            }
            else
                return false;
        }
        return false;
    }
    bool KeyboardController::IsKeyCombinationDown(const KeyCombination& keyCombination)
    {
        bool areAllPressed = true;
        for(auto& keyToPress : keyCombination.keysToPress)
            if(!IsKeyPressed(keyToPress))
            {
                areAllPressed = false;
                break;
            }

        if(areAllPressed)
        {
            if(IsKeyDown(keyCombination.keyToDown))
                return true;
            else
                return false;
        }
        return false;
    }
    bool KeyboardController::IsKeyCombinationPressed(GLFWwindow* window, const KeyCombination& keyCombination)
    {
        bool areAllPressed = false;
        if(IsKeyPressed(window, keyCombination.keyToDown))
            areAllPressed = true;
        else
            return false;
        for(auto& keyToPress : keyCombination.keysToPress)
            if(!IsKeyPressed(window, keyToPress))
            {
                areAllPressed = false;
                break;
            }

        return areAllPressed;
    }
    bool KeyboardController::IsKeyCombinationPressed(const KeyCombination& keyCombination)
    {
        bool areAllPressed = false;
        if(IsKeyPressed(keyCombination.keyToDown))
            areAllPressed = true;
        else
            return false;
        for(auto& keyToPress : keyCombination.keysToPress)
            if(!IsKeyPressed(keyToPress))
            {
                areAllPressed = false;
                break;
            }

        return areAllPressed;
    }
}