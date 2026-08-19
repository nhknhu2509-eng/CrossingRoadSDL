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

    void SetTexture(const std::string& textureId);

    SDL_Rect GetRect() const;
    SDL_Rect GetHitbox() const;

private:
    void UpdateHitbox();

private:
    // Hình ảnh xe
    SDL_Rect rect;

    // Hitbox dùng cho collision
    SDL_Rect hitbox;
    SDL_Rect GetHitbox() const;
    int speed;
    int direction;

    // Chiều cao lane
    int laneHeight;

    std::string textureId;
};