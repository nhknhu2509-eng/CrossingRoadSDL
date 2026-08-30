#pragma once

#include <vector>
#include <string>
#include <SDL.h>

#include "Objects/Vehicle.h"
#include "Objects/Animal.h"
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


    const std::vector<Vehicle>&
        GetVehicles() const;


    const std::vector<Animal>&
        GetAnimals() const;


private:

    std::vector<Vehicle> vehicles;

    std::vector<Animal> animals;

    TrafficLight trafficLight;
};