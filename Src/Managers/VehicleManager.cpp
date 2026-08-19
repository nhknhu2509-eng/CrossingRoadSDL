#include "Managers/VehicleManager.h"

VehicleManager::VehicleManager()
{
    lanes.emplace_back(180, 1, 3, 3);

    lanes.emplace_back(280, -1, 5, 2);

    lanes.emplace_back(380, 1, 4, 3);
}

void VehicleManager::Update()
{
    for (Lane& lane : lanes)
    {
        lane.Update();
    }
}

void VehicleManager::Draw(SDL_Renderer* renderer)
{
    for (Lane& lane : lanes)
    {
        lane.Draw(renderer);
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