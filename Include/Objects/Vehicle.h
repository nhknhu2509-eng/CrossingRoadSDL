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

private:
    SDL_Rect rect;

    int speed;
    int direction;

    std::string textureId;
};