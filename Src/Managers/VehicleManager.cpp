#include "Managers/VehicleManager.h"

#include "Graphics/TextureManager.h"


VehicleManager::VehicleManager()
{
    // ==========================================
    // LANE 1 - WAGON
    // ==========================================

    lanes.emplace_back(
        185,        // Y lane
        82,
        1,
        3,
        3,
        "wagon",
        "",

        960,        // X đèn
        165,        // Y đèn

        5000,       // Xanh: 5.0 giây
        1000,       // Vàng: 1.0 giây
        5000);      // Đỏ: 5.0 giây



    // ==========================================
    // LANE 2 - WAGON
    // ==========================================

    lanes.emplace_back(
        267,        // Y lane
        82,
        -1,
        4,
        3,
        "wagon",
        "",

        320,        // X đèn
        255,        // Y đèn

        6500,       // Xanh: 6.5 giây
        1200,       // Vàng: 1.2 giây
        4500);      // Đỏ: 4.5 giây



    // ==========================================
    // LANE 3 - DEER
    // ==========================================

    lanes.emplace_back(
        315,        // Y lane
        82,
        1,
        3,
        3,
        "",
        "deer",

        960,        // X đèn
        340,        // Y đèn

        4000,       // Xanh: 4.0 giây
        900,        // Vàng: 0.9 giây
        6000);      // Đỏ: 6.0 giây



    // ==========================================
    // LANE 4 - SQUIRREL
    // ==========================================

    lanes.emplace_back(
        410,        // Y lane
        82,
        -1,
        2,
        3,
        "",
        "squirrel",

        320,        // X đèn
        430,        // Y đèn

        7500,       // Xanh: 7.5 giây
        1500,       // Vàng: 1.5 giây
        4000);      // Đỏ: 4.0 giây



    // ==========================================
    // LANE 5 - RABBIT
    // ==========================================

    lanes.emplace_back(
        505,        // Y lane
        82,
        1,
        2,
        3,
        "",
        "rabbit",

        960,        // X đèn
        530,        // Y đèn

        5500,       // Xanh: 5.5 giây
        1100,       // Vàng: 1.1 giây
        7000);      // Đỏ: 7.0 giây
}


// ==================================================
// UPDATE
// ==================================================

void VehicleManager::Update()
{
    for (Lane& lane :
        lanes)
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
    for (Lane& lane :
        lanes)
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


    for (const Lane& lane :
        lanes)
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


    for (const Lane& lane :
        lanes)
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