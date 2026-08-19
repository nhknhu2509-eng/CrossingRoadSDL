#include "Managers/VehicleManager.h"

#include "Graphics/TextureManager.h"


VehicleManager::VehicleManager()
{
    // ==========================================
    // Lane 1 - VEHICLE
    // ==========================================

    lanes.emplace_back(
        185,
        82,
        1,
        3,
        3);


    // ==========================================
    // Lane 2 - VEHICLE
    // ==========================================

    lanes.emplace_back(
        267,
        82,
        -1,
        4,
        3);


    // ==========================================
    // Lane 3 - DEER
    // ==========================================

    lanes.emplace_back(
        347,
        82,
        1,
        3,
        3,
        "deer");


    // ==========================================
    // Lane 4 - SQUIRREL
    // ==========================================

    lanes.emplace_back(
        434,
        82,
        -1,
        5,
        3,
        "squirrel");


    // ==========================================
    // Lane 5 - RABBIT
    // ==========================================

    lanes.emplace_back(
        531,
        82,
        1,
        4,
        3,
        "rabbit");
}


void VehicleManager::Update()
{
    for (Lane& lane : lanes)
    {
        lane.Update();
    }
}


void VehicleManager::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    for (Lane& lane : lanes)
    {
        lane.Draw(
            renderer,
            textureManager);
    }
}


std::vector<Vehicle>
VehicleManager::GetVehicles() const
{
    std::vector<Vehicle> result;


    for (const Lane& lane : lanes)
    {
        for (const Vehicle& vehicle :
            lane.GetVehicles())
        {
            result.push_back(vehicle);
        }
    }


    return result;
}


std::vector<Animal>
VehicleManager::GetAnimals() const
{
    std::vector<Animal> result;


    for (const Lane& lane : lanes)
    {
        for (const Animal& animal :
            lane.GetAnimals())
        {
            result.push_back(animal);
        }
    }


    return result;
}