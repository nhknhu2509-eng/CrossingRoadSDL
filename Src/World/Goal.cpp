#include "World/Goal.h"

Goal::Goal()
{
    rect.x = 0;
    rect.y = 0;

    rect.w = 800;
    rect.h = 60;
}

void Goal::Draw(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 30, 180, 30, 255);
    SDL_RenderFillRect(renderer, &rect);
}

bool Goal::Reached(const SDL_Rect& playerRect) const
{
    return SDL_HasIntersection(&rect, &playerRect);
}