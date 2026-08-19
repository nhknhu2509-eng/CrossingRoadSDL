#include "Managers/VehicleManager.h"
#include "Graphics/TextureManager.h"

VehicleManager::VehicleManager()
{
    // Lane 1 - gần đích nhất
    lanes.emplace_back(185, 1, 3, 3);

    // Lane 2
    lanes.emplace_back(267, -1, 4, 3);

    // Lane 3
    lanes.emplace_back(347, 1, 3, 3);

    // Lane 4
    lanes.emplace_back(434, -1, 5, 3);

    // Lane 5 - gần nhân vật nhất
    lanes.emplace_back(531, 1, 4, 3);
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

std::vector<Vehicle> VehicleManager::GetVehicles() const
{
    std::vector<Vehicle> result;

    for (const Lane& lane : lanes)
    {
        for (const Vehicle& vehicle : lane.GetVehicles())
        {
            result.push_back(vehicle);
        }
    }

    return result;
}