#pragma once

#include <SDL.h>

class InputManager
{
public:
    // Cập nhật trạng thái bàn phím và xử lý sự kiện
    static void Update();

    // Kiểm tra phím đang được giữ
    static bool IsKeyDown(SDL_Scancode key);

    // Kiểm tra người dùng có bấm nút đóng cửa sổ hay không
    static bool QuitRequested();

private:
    static const Uint8* keyboardState;
    static bool quit;
};