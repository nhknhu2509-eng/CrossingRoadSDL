#include "Managers/VehicleManager.h"

#include "Graphics/TextureManager.h"


VehicleManager::VehicleManager()
{
    // ==========================================
    // LANE 1 - WAGON
    // ==========================================

    lanes.emplace_back(
        185,
        82,
        1,
        3,
        3,
        "wagon",
        "");


    // ==========================================
    // LANE 2 - WAGON
    // ==========================================

    lanes.emplace_back(
        267,
        82,
        -1,
        4,
        3,
        "wagon",
        "");


    // ==========================================
    // LANE 3 - DEER
    // ==========================================
    //
    // QUAN TRỌNG:
    //
    // Deer nằm ở animalTexture,
    // KHÔNG nằm ở vehicleTexture.
    //
    // ==========================================

    lanes.emplace_back(
        315,
        82,
        1,
        3,
        3,
        "",
        "deer");


    // ==========================================
    // LANE 4 - SQUIRREL
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
    // LANE 5 - RABBIT
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


// ==================================================
// UPDATE
// ==================================================

void VehicleManager::Update()
{
    for (Lane& lane : lanes)
    {
        lane.Update();
    }
}


// ==================================================
// DRAW
// ==================================================

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


// ==================================================
// GET VEHICLES
// ==================================================

std::vector<Vehicle>
VehicleManager::GetVehicles() const
{
    std::vector<Vehicle> result;


    for (const Lane& lane : lanes)
    {
        for (const Vehicle& vehicle :
            lane.GetVehicles())
        {
            result.push_back(
                vehicle);
        }
    }


    return result;
}


// ==================================================
// GET ANIMALS
// ==================================================

std::vector<Animal>
VehicleManager::GetAnimals() const
{
    std::vector<Animal> result;


    for (const Lane& lane : lanes)
    {
        for (const Animal& animal :
            lane.GetAnimals())
        {
            result.push_back(
                animal);
        }
    }


    return result;
}