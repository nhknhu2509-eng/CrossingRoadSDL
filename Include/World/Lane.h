#pragma once

#include <vector>
#include <memory>
#include <string>
#include <SDL.h>

#include "Objects/Obstacle.h"
#include "World/TrafficLight.h"


class TextureManager;


class Lane
{
public:

    Lane(
        int y,
        int height,
        int direction,
        int speed,
        int vehicleCount,
        const std::string& vehicleTexture = "",
        const std::string& animalTexture = "",
        int lightX = 760,
        int lightY = 0,
        Uint32 greenTime = 5000,
        Uint32 yellowTime = 1000,
        Uint32 redTime = 5000);


    void Update();


    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);


    // ==========================================
    // OBSTACLES
    // ==========================================

    const std::vector<
        std::unique_ptr<Obstacle>>&
        GetObstacles() const;


    // ==========================================
    // TRAFFIC LIGHT
    // ==========================================

    const TrafficLight&
        GetTrafficLight() const;


    TrafficLight&
        GetMutableTrafficLight();


private:

    std::vector<
        std::unique_ptr<Obstacle>>
        obstacles;


    TrafficLight trafficLight;
};