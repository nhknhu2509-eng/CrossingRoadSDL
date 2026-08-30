#pragma once

#include <SDL.h>

class TextureManager;


enum class LightState
{
    Green,
    Yellow,
    Red
};


class TrafficLight
{
public:

    TrafficLight(
        int x,
        int y,
        Uint32 greenDuration = 5000,
        Uint32 yellowDuration = 1000,
        Uint32 redDuration = 5000);


    void Update();


    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);


    bool CanMove() const;


private:

    SDL_Rect rect;

    LightState state;

    Uint32 lastSwitchTime;

    Uint32 greenTime;

    Uint32 yellowTime;

    Uint32 redTime;
};