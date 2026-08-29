#include "Managers/VehicleManager.h"

#include "Graphics/TextureManager.h"


VehicleManager::VehicleManager()
{
    // ==========================================
    // LANE 1 - WAGON
    // ==========================================

    lanes.emplace_back(
        185,        // Y của lane
        82,
        1,
        3,
        3,
        "wagon",
        "",
        960,        // X của đèn 1
        165);       // Y của đèn 1


    // ==========================================
    // LANE 2 - WAGON
    // ==========================================

    lanes.emplace_back(
        267,        // Y của lane
        82,
        -1,
        4,
        3,
        "wagon",
        "",
        320,        // X của đèn 2
        255);       // Y của đèn 2


    // ==========================================
    // LANE 3 - DEER
    // ==========================================

    lanes.emplace_back(
        315,        // Y của lane
        82,
        1,
        3,
        3,
        "",
        "deer",
        960,        // X của đèn 3
        340);       // Y của đèn 3


    // ==========================================
    // LANE 4 - SQUIRREL
    // ==========================================

    lanes.emplace_back(
        410,        // Y của lane
        82,
        -1,
        5,
        3,
        "",
        "squirrel",
        320,        // X của đèn 4
        430);       // Y của đèn 4


    // ==========================================
    // LANE 5 - RABBIT
    // ==========================================

    lanes.emplace_back(
        505,        // Y của lane
        82,
        1,
        4,
        3,
        "",
        "rabbit",
        960,        // X của đèn 5
        530);       // Y của đèn 5
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