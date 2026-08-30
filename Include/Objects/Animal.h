#pragma once

#include <SDL.h>
#include <string>
#include <vector>

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

    void SetHitboxMargins(
        int left,
        int top,
        int right,
        int bottom);

    SDL_Rect GetRect() const;

    SDL_Rect GetHitbox() const;


private:

    void UpdateHitbox();

    void UpdateAnimation();


private:

    // ==============================
    // Sprite
    // ==============================

    SDL_Rect rect;


    // ==============================
    // Collision
    // ==============================

    SDL_Rect hitbox;


    // ==============================
    // Movement
    // ==============================

    int speed;

    int direction;


    // ==============================
    // Animal type
    // ==============================

    std::string textureId;


    // ==============================
    // Animation
    // ==============================

    std::vector<std::string> animationFrames;

    int currentFrame;

    Uint32 lastFrameTime;

    Uint32 frameDuration;


    // ==============================
    // Hitbox margins
    // ==============================

    int hitboxLeft;

    int hitboxTop;

    int hitboxRight;

    int hitboxBottom;
};