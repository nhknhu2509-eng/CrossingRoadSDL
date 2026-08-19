#pragma once

#include <SDL.h>

#include "Objects/GameObject.h"

class Vehicle : public GameObject
{
public:
    Vehicle();

    void Update();
    void Draw(SDL_Renderer* renderer);

    void SetSpeed(int value);
    void SetDirection(int value);
    void SetPosition(int x, int y);

    SDL_Rect GetRect() const;

private:
    SDL_Rect rect;

    int speed;
    int direction;
};