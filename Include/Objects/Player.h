#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

#include "Graphics/TextureManager.h"

class Player
{
private:
    SDL_Rect rect;
    int speed;

public:
    Player();

    void Update();

    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager
    );

    const SDL_Rect& GetRect() const;

    // Hitbox riêng của Player
    SDL_Rect GetHitbox() const;

    void Reset();

    void SetPosition(
        int x,
        int y
    );
};

#endif