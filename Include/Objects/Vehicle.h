#pragma once

#include <SDL.h>
#include <string>
#include <vector>


class TextureManager;


class Vehicle
{
public:

    Vehicle();

    void Update();

    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);

    void SetSpeed(
        int value);

    void SetDirection(
        int value);

    void SetPosition(
        int x,
        int y);

    void SetLaneHeight(
        int height);

    void SetTexture(
        const std::string& textureId);

    void SetSpriteSize(
        int width,
        int height);

    SDL_Rect GetRect() const;

    SDL_Rect GetHitbox() const;


private:

    void UpdateHitbox();

    void UpdateAnimation();


private:

    SDL_Rect rect;

    SDL_Rect hitbox;


    int speed;

    int direction;

    int laneHeight;


    // Loại wagon:
    // wagon1, wagon2, wagon3, wagon4
    std::string textureId;


    // ==============================
    // Animation
    // ==============================

    std::vector<std::string> animationFrames;

    int currentFrame;

    Uint32 lastFrameTime;

    Uint32 frameDuration;
};