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
    laneManager.Update();
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


    laneManager.Draw(
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
// GET LANE MANAGER
// ==================================================

LaneManager&
Map::GetLaneManager()
{
    return laneManager;
}


const LaneManager&
Map::GetLaneManager() const
{
    return laneManager;
}