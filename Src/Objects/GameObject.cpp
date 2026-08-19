#include "Objects/GameObject.h"

GameObject::GameObject()
{
    x = 0.0f;
    y = 0.0f;

    width = 0;
    height = 0;
}

GameObject::GameObject(float x, float y, int width, int height)
{
    this->x = x;
    this->y = y;

    this->width = width;
    this->height = height;
}

void GameObject::Update(float deltaTime)
{

}

void GameObject::Render(SDL_Renderer* renderer)
{

}

void GameObject::SetPosition(float x, float y)
{
    this->x = x;
    this->y = y;
}

float GameObject::GetX() const
{
    return x;
}

float GameObject::GetY() const
{
    return y;
}

int GameObject::GetWidth() const
{
    return width;
}

int GameObject::GetHeight() const
{
    return height;
}