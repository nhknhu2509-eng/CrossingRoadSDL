#pragma once

#include <SDL.h>

class CollisionManager
{
public:
    static bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b);
};