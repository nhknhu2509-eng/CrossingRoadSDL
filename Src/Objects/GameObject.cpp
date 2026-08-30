#include "Objects/GameObject.h"


// ==================================================
// CONSTRUCTOR
// ==================================================

GameObject::GameObject()
{
    rect.x = 0;
    rect.y = 0;

    rect.w = 0;
    rect.h = 0;
}


// ==================================================
// SET POSITION
// ==================================================

void GameObject::SetPosition(
    int x,
    int y)
{
    rect.x = x;
    rect.y = y;
}


// ==================================================
// GET RECT
// ==================================================

const SDL_Rect&
GameObject::GetRect() const
{
    return rect;
}