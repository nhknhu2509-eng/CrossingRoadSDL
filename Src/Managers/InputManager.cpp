#include "Managers/InputManager.h"

#include <cstring>


const Uint8*
InputManager::keyboardState =
nullptr;


Uint8
InputManager::previousKeyboardState[
    SDL_NUM_SCANCODES] = { 0 };


    bool
        InputManager::quit =
        false;


    // ==================================================
    // UPDATE
    // ==================================================

    void InputManager::Update()
    {
        // ==========================================
        // LƯU TRẠNG THÁI FRAME TRƯỚC
        // ==========================================

        if (keyboardState != nullptr)
        {
            std::memcpy(
                previousKeyboardState,
                keyboardState,
                SDL_NUM_SCANCODES);
        }


        // ==========================================
        // SDL EVENTS
        // ==========================================

        SDL_Event event;


        while (SDL_PollEvent(&event))
        {
            if (event.type ==
                SDL_QUIT)
            {
                quit =
                    true;
            }
        }


        // ==========================================
        // KEYBOARD STATE
        // ==========================================

        SDL_PumpEvents();


        keyboardState =
            SDL_GetKeyboardState(
                nullptr);
    }


    // ==================================================
    // KEY DOWN
    // ==================================================

    bool InputManager::IsKeyDown(
        SDL_Scancode key)
    {
        if (keyboardState ==
            nullptr)
        {
            return false;
        }


        return keyboardState[key] != 0;
    }


    // ==================================================
    // KEY PRESSED
    // ==================================================

    bool InputManager::IsKeyPressed(
        SDL_Scancode key)
    {
        if (keyboardState ==
            nullptr)
        {
            return false;
        }


        return
            keyboardState[key] != 0 &&
            previousKeyboardState[key] == 0;
    }


    // ==================================================
    // QUIT
    // ==================================================

    bool InputManager::QuitRequested()
    {
        return quit;
    }