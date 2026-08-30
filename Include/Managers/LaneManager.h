#pragma once

#include <vector>
#include <SDL.h>

#include "World/Lane.h"


class TextureManager;


class LaneManager
{
public:

    LaneManager();


    void Update();


    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);


    std::vector<const Obstacle*>
        GetObstacles() const;


    std::vector<Obstacle*>
        GetMutableObstacles();


    std::vector<TrafficLightSaveState>
        GetTrafficLightSaveStates() const;


    void RestoreTrafficLightSaveStates(
        const std::vector<TrafficLightSaveState>& states);


private:

    std::vector<Lane> lanes;
};