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
        "",

        960,
        165,

        5000,
        1000,
        5000);


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
        "",

        320,
        255,

        6500,
        1200,
        4500);


    // ==========================================
    // LANE 3 - DEER
    // ==========================================

    lanes.emplace_back(
        315,
        82,
        1,
        3,
        3,
        "",
        "deer",

        960,
        340,

        4000,
        900,
        6000);


    // ==========================================
    // LANE 4 - SQUIRREL
    // ==========================================

    lanes.emplace_back(
        410,
        82,
        -1,
        2,
        3,
        "",
        "squirrel",

        320,
        430,

        7500,
        1500,
        4000);


    // ==========================================
    // LANE 5 - RABBIT
    // ==========================================

    lanes.emplace_back(
        505,
        82,
        1,
        2,
        3,
        "",
        "rabbit",

        960,
        530,

        5500,
        1100,
        7000);
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
// GET OBSTACLES
// ==================================================

std::vector<const Obstacle*>
VehicleManager::GetObstacles() const
{
    std::vector<const Obstacle*>
        result;


    for (const Lane& lane :
        lanes)
    {
        for (
            const std::unique_ptr<Obstacle>& obstacle :
            lane.GetObstacles())
        {
            result.push_back(
                obstacle.get());
        }
    }


    return result;
}