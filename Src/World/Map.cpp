#include "World/Map.h"
#include "Graphics/TextureManager.h"

Map::Map()
{
}

void Map::Update()
{
    vehicleManager.Update();
}

void Map::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    goal.Draw(renderer);

    vehicleManager.Draw(
        renderer,
        textureManager);
}

Goal& Map::GetGoal()
{
    return goal;
}

VehicleManager& Map::GetVehicleManager()
{
    return vehicleManager;
}