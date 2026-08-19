#pragma once

#include <SDL.h>

class Player
{
public:
    Player();

    void Update();
    void Draw(SDL_Renderer* renderer);

    void Reset();
    void SetPosition(int x, int y);

    SDL_Rect GetRect() const;

private:
    SDL_Rect rect;
    int speed;
};