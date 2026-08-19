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

    // ==============================
    // Vehicles
    // ==============================

    std::vector<Vehicle> vehicles;


    // ==============================
    // Animals
    // ==============================

    std::vector<Animal> animals;


    // ==============================
    // Traffic light
    // ==============================

    TrafficLight trafficLight;
};