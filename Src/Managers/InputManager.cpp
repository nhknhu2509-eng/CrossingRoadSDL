#include "Managers/InputManager.h"

const Uint8* InputManager::keyboardState = nullptr;
bool InputManager::quit = false;

void InputManager::Update()
{
    SDL_Event event;

    quit = false;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            quit = true;
        }
    }

    keyboardState = SDL_GetKeyboardState(nullptr);
}

bool InputManager::IsKeyDown(SDL_Scancode key)
{
    if (keyboardState == nullptr)
        return false;

    return keyboardState[key];
}

bool InputManager::QuitRequested()
{
    return quit;
}