#pragma once

#include <SDL.h>

class TextureManager;


enum class LightState
{
    Green,
    Yellow,
    Red
};


// ==================================================
// TRAFFIC LIGHT SAVE STATE
// ==================================================

struct TrafficLightSaveState
{
    LightState state;

    Uint32 elapsedTime;
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


    // ==========================================
    // SAVE / LOAD
    // ==========================================

    TrafficLightSaveState
        GetSaveState() const;


    void RestoreSaveState(
        const TrafficLightSaveState& saveState);


private:

    SDL_Rect rect;


    LightState state;


    Uint32 lastSwitchTime;


    Uint32 greenTime;


    Uint32 yellowTime;


    Uint32 redTime;
};