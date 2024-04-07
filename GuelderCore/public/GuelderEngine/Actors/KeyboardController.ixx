module;
#include <glfw/glfw3.h>
export module GuelderEngine.Actors:KeyboardController;

import <unordered_set>;

export namespace GuelderEngine
{
    enum class Keys
    {
        None = 0,
        A = GLFW_KEY_A,
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z,
        Up = GLFW_KEY_UP,
        Down = GLFW_KEY_DOWN,
        Right = GLFW_KEY_RIGHT,
        Left = GLFW_KEY_LEFT,
        _1 = GLFW_KEY_1,
        _2 = GLFW_KEY_2,
        _3 = GLFW_KEY_3,
        _4 = GLFW_KEY_4,
        _5 = GLFW_KEY_5,
        _6 = GLFW_KEY_6,
        _7 = GLFW_KEY_7,
        _8 = GLFW_KEY_8,
        _9 = GLFW_KEY_9,
        _0 = GLFW_KEY_0,
        LeftShift = GLFW_KEY_LEFT_SHIFT,
        Space = GLFW_KEY_SPACE
    };
    struct KeyCombination
    {
        Keys keyToDown;
        std::unordered_set<Keys> keysToPress;
    };

    class KeyboardController
    {
    public:
        //All function that are without window param don't check whether the GuelderEngine's window is on Foreground
        static bool IsKeyDown(GLFWwindow* window, const Keys& key);
        static bool IsKeyDown(const Keys& key);

        static bool IsKeyPressed(GLFWwindow* window, const Keys& key);
        static bool IsKeyPressed(const Keys& key);

        static bool IsKeyReleased(GLFWwindow* window, const Keys& key);
        static bool IsKeyReleased(const Keys& key);

        static bool IsKeyCombinationDown(GLFWwindow* window, const KeyCombination& keyCombination);
        static bool IsKeyCombinationDown(const KeyCombination& keyCombination);

        static bool IsKeyCombinationPressed(GLFWwindow* window, const KeyCombination& keyCombination);
        static bool IsKeyCombinationPressed(const KeyCombination& keyCombination);
    private:
    };
}