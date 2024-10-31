#include "Input.h"


std::unordered_map<int, bool> Input::keyMap;


void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        keyMap[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keyMap[key] = false;
    }
}


bool Input::getKeyDown(Key key) {
    // check if this key is pressed
    int keyInt = static_cast<int>(key);

    return keyMap.contains(keyInt) && keyMap[keyInt];
}


bool Input::getKeyPressed(Key key) {
    int keyInt = static_cast<int>(key);
    bool pressed = keyMap[keyInt];
    keyMap[keyInt] = false;
    return pressed;
}
