#pragma once

#include <SDL.h>
#include <string>

#include "Objects/GameObject.h"

class TextureManager;

class Vehicle : public GameObject
{
public:
    Vehicle();

    void Update();

    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);

    void SetSpeed(int value);
    void SetDirection(int value);
    void SetPosition(int x, int y);

    void SetLaneHeight(int height);

    void SetTexture(const std::string& textureId);

    SDL_Rect GetRect() const;
    SDL_Rect GetHitbox() const;

private:
    void UpdateHitbox();

private:
    // Sprite
    SDL_Rect rect;

    // Collision hitbox
    SDL_Rect hitbox;

    int speed;
    int direction;

    // Chiều cao phần đường mà xe chiếm
    int laneHeight;

    std::string textureId;
};