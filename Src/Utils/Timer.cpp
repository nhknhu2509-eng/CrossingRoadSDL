#include "Utils/Timer.h"

#include <SDL.h>

Timer::Timer()
{
    lastTick = SDL_GetTicks();
    deltaTime = 0.0f;
}

void Timer::Tick()
{
    unsigned int currentTick = SDL_GetTicks();

    deltaTime = (currentTick - lastTick) / 1000.0f;

    lastTick = currentTick;
}

float Timer::GetDeltaTime() const
{
    return deltaTime;
}