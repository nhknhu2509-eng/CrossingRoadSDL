#pragma once

#include <SDL.h>
#include <string>

#include "Objects/GameObject.h"

class TextureManager;

class Animal : public GameObject
{
public:
    Animal();

    void Update();

    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);

    void SetSpeed(int value);

    void SetDirection(int value);

    void SetPosition(
        int x,
        int y);

    void SetTexture(
        const std::string& textureId);

    // Thiết lập phần transparent cần bỏ
    // ở 4 phía của sprite
    void SetHitboxMargins(
        int left,
        int top,
        int right,
        int bottom);

    SDL_Rect GetRect() const;

    SDL_Rect GetHitbox() const;

private:
    void UpdateHitbox();

private:
    // ==============================
    // Sprite
    // ==============================

    SDL_Rect rect;


    // ==============================
    // Collision hitbox
    // ==============================

    SDL_Rect hitbox;


    // ==============================
    // Movement
    // ==============================

    int speed;

    int direction;


    // ==============================
    // Texture
    // ==============================

    std::string textureId;


    // ==============================
    // Transparent margins
    // ==============================

    int hitboxLeft;

    int hitboxTop;

    int hitboxRight;

    int hitboxBottom;
};