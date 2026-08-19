#pragma once

#include <SDL.h>

class GameObject
{
public:
    GameObject();
    GameObject(float x, float y, int width, int height);

    virtual ~GameObject() = default;

    virtual void Update(float deltaTime);

    virtual void Render(SDL_Renderer* renderer);

    void SetPosition(float x, float y);

    float GetX() const;
    float GetY() const;

    int GetWidth() const;
    int GetHeight() const;

protected:
    float x;
    float y;

    int width;
    int height;
};