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

    // Hitbox dùng để kiểm tra va chạm
    SDL_Rect GetHitbox() const;

private:
    void UpdateHitbox();

private:
    // Kích thước hình ảnh xe
    SDL_Rect rect;

    // Hitbox riêng của xe
    SDL_Rect hitbox;

    int speed;
    int direction;

    // Chiều cao của lane mà xe đang chạy
    int laneHeight;

    std::string textureId;
};