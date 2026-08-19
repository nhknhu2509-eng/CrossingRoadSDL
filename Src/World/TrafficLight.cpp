#include "World/TrafficLight.h"

TrafficLight::TrafficLight(int x, int y)
{
    rect.x = x;
    rect.y = y;
    rect.w = 20;
    rect.h = 50;

    state = LightState::Green;

    lastSwitchTime = SDL_GetTicks();
}

void TrafficLight::Update()
{
    Uint32 now = SDL_GetTicks();

    switch (state)
    {
    case LightState::Green:

        if (now - lastSwitchTime >= greenTime)
        {
            state = LightState::Yellow;
            lastSwitchTime = now;
        }
        break;

    case LightState::Yellow:

        if (now - lastSwitchTime >= yellowTime)
        {
            state = LightState::Red;
            lastSwitchTime = now;
        }
        break;

    case LightState::Red:

        if (now - lastSwitchTime >= redTime)
        {
            state = LightState::Green;
            lastSwitchTime = now;
        }
        break;
    }
}

bool TrafficLight::CanMove() const
{
    return state != LightState::Red;
}

void TrafficLight::Draw(SDL_Renderer* renderer)
{
    switch (state)
    {
    case LightState::Green:
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        break;

    case LightState::Yellow:
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        break;

    case LightState::Red:
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        break;
    }

    SDL_RenderFillRect(renderer, &rect);
}