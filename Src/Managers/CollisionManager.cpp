#include "Managers/CollisionManager.h"

bool CollisionManager::CheckCollision(const SDL_Rect& a,
    const SDL_Rect& b)
{
    return !(a.x + a.w <= b.x ||
        a.x >= b.x + b.w ||
        a.y + a.h <= b.y ||
        a.y >= b.y + b.h);
}