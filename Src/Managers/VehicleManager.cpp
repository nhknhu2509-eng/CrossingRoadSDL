#include "Managers/VehicleManager.h"

#include "Graphics/TextureManager.h"


VehicleManager::VehicleManager()
{
    // ==========================================
    // Lane 1 - Vehicle / Wagon
    // ==========================================

    lanes.emplace_back(
        185,
        82,
        1,
        3,
        3);


    // ==========================================
    // Lane 2 - Vehicle / Wagon
    // ==========================================

    lanes.emplace_back(
        267,
        82,
        -1,
        4,
        3);


    // ==========================================
    // Lane 3 - Vehicle / Deer
    // ==========================================
    //
    // Vẫn là Vehicle.
    // Chỉ thay sprite thành deer.png.
    //

    lanes.emplace_back(
        315,
        82,
        1,
        3,
        3,
        "deer",
        "");


    // ==========================================
    // Lane 4 - Animal / Squirrel
    // ==========================================

    lanes.emplace_back(
        420,
        82,
        -1,
        5,
        3,
        "",
        "squirrel");


    // ==========================================
    // Lane 5 - Animal / Rabbit
    // ==========================================

    lanes.emplace_back(
        515,
        82,
        1,
        4,
        3,
        "",
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