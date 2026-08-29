#include "World/Goal.h"


Goal::Goal()
{
    // =====================================
    // GOAL AREA
    // =====================================
    //
    // Vẫn giữ vùng Goal để kiểm tra
    // người chơi đã tới đích hay chưa.
    //
    // Nhưng KHÔNG vẽ hình chữ nhật xanh.
    // =====================================

    rect.x = 0;
    rect.y = 0;

    rect.w = 800;
    rect.h = 60;
}


void Goal::Draw(SDL_Renderer* renderer)
{
    // Không vẽ Goal.
    //
    // Goal chỉ còn là vùng logic dùng
    // để kiểm tra chiến thắng.

    (void)renderer;
}


bool Goal::Reached(
    const SDL_Rect& playerRect) const
{
    return SDL_HasIntersection(
        &rect,
        &playerRect);
}