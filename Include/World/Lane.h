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
        const std::string& animalTexture = "");


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