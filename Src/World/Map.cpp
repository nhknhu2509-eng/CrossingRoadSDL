#include "World/Map.h"

Map::Map()
{
}

void Map::Update()
{
    vehicleManager.Update();
}

void Map::Draw(SDL_Renderer* renderer)
{
    goal.Draw(renderer);

    vehicleManager.Draw(renderer);
}

Goal& Map::GetGoal()
{
    return goal;
}

VehicleManager& Map::GetVehicleManager()
{
    return vehicleManager;
}   