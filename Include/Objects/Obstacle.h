#pragma once

#include <SDL.h>

#include <string>
#include <vector>

#include "Objects/GameObject.h"


class TextureManager;


class Obstacle : public GameObject
{
public:

    Obstacle();

    virtual ~Obstacle() = default;


    // ==========================================
    // GAME OBJECT
    // ==========================================

    void Update() override;


    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager) override;


    SDL_Rect GetHitbox() const override;


    void SetPosition(
        int x,
        int y) override;


    // ==========================================
    // MOVEMENT
    // ==========================================

    void SetSpeed(
        int value);


    void SetDirection(
        int value);


protected:

    // ==========================================
    // ANIMATION CONFIG
    // ==========================================

    void SetAnimationFrames(
        const std::vector<std::string>& frames);


    void SetFrameDuration(
        Uint32 duration);


    // ==========================================
    // DRAW CONFIG
    // ==========================================

    void SetFallbackColor(
        SDL_Color color);


    void SetHitboxDebugColor(
        SDL_Color color);


    // ==========================================
    // HITBOX
    // ==========================================

    virtual void UpdateHitbox() = 0;


protected:

    SDL_Rect hitbox;


    int speed;

    int direction;


    std::vector<std::string>
        animationFrames;


    int currentFrame;

    Uint32 lastFrameTime;

    Uint32 frameDuration;


    SDL_Color fallbackColor;

    SDL_Color hitboxDebugColor;


    bool debugHitbox;


private:

    void UpdateAnimation();

    void WrapScreen();
};