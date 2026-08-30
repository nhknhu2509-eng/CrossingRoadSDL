#pragma once

#include <vector>
#include <SDL.h>

#include "World/Lane.h"


class TextureManager;


class LaneManager
{
public:

    LaneManager();


    void Update();


    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);


    // ==========================================
    // READ-ONLY OBSTACLES
    //
    // Dùng cho collision.
    // ==========================================

    std::vector<const Obstacle*>
        GetObstacles() const;


    // ==========================================
    // MUTABLE OBSTACLES
    //
    // Dùng khi restore Save Game.
    // ==========================================

    std::vector<Obstacle*>
        GetMutableObstacles();


private:

    std::vector<Lane> lanes;
};