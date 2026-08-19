#pragma once
#pragma once

#include <SDL.h>

enum class LightState
{
    Green,
    Yellow,
    Red
};

class TrafficLight
{
public:
    TrafficLight(int x, int y);

    void Update();

    void Draw(SDL_Renderer* renderer);

    bool CanMove() const;

private:
    SDL_Rect rect;

    LightState state;

    Uint32 lastSwitchTime;

    Uint32 greenTime = 5000;
    Uint32 yellowTime = 1000;
    Uint32 redTime = 5000;
};