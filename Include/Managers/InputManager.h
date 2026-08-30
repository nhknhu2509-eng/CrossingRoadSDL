#pragma once

#include <SDL.h>


class InputManager
{
public:

    // Cập nhật trạng thái input mỗi frame
    static void Update();


    // Phím đang được giữ
    static bool IsKeyDown(
        SDL_Scancode key);


    // Phím vừa được nhấn trong frame hiện tại
    static bool IsKeyPressed(
        SDL_Scancode key);


    // Người dùng đóng cửa sổ
    static bool QuitRequested();


private:

    static const Uint8* keyboardState;

    static Uint8 previousKeyboardState[
        SDL_NUM_SCANCODES];

    static bool quit;
};