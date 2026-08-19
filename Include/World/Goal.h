#pragma once

#include <SDL.h>

class Goal
{
public:
    Goal();

    void Draw(SDL_Renderer* renderer);

    bool Reached(const SDL_Rect& playerRect) const;

private:
    SDL_Rect rect;
};