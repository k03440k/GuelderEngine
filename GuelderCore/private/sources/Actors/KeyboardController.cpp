module;
#include <glfw/glfw3.h>
module GuelderEngine.Actors;
import :KeyboardController;

import <map>;

namespace GuelderEngine
{
    bool KeyboardController::IsKeyDown(GLFWwindow* window, const Keys& key)
    {
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
        return glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS;
    }
    bool KeyboardController::IsKeyReleased(GLFWwindow* window, const Keys& key)
    {
        //not done yet
        return false;
    }
    bool KeyboardController::IsKeyCombinationPressed(GLFWwindow* window, const Keys& keyToDown, const Keys& keyToPress)
    {
        if(IsKeyPressed(window, keyToPress))
        {
            if(IsKeyDown(window, keyToDown))
                return true;
            else
                return false;
        }
        return false;
    }
}