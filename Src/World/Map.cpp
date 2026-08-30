#include "World/Map.h"

#include "Graphics/TextureManager.h"


// ==================================================
// CONSTRUCTOR
// ==================================================

Map::Map()
{
}


// ==================================================
// UPDATE
// ==================================================

void Map::Update()
{
    vehicleManager.Update();
}


// ==================================================
// DRAW
// ==================================================

void Map::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    goal.Draw(
        renderer);


    vehicleManager.Draw(
        renderer,
        textureManager);
}


// ==================================================
// GET GOAL
// ==================================================

Goal&
Map::GetGoal()
{
    return goal;
}


const Goal&
Map::GetGoal() const
{
    return goal;
}


// ==================================================
// GET VEHICLE MANAGER
// ==================================================

VehicleManager&
Map::GetVehicleManager()
{
    return vehicleManager;
}


const VehicleManager&
Map::GetVehicleManager() const
{
    return vehicleManager;
}