#pragma once

#include <SDL.h>
#include <string>


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


    // ==========================================
    // Sprite size
    // ==========================================

    void SetSpriteSize(
        int width,
        int height);


    SDL_Rect GetRect() const;


    SDL_Rect GetHitbox() const;


private:

    void UpdateHitbox();


private:

    // ==========================================
    // Sprite
    // ==========================================

    SDL_Rect rect;


    // ==========================================
    // Hitbox
    // ==========================================

    SDL_Rect hitbox;


    // ==========================================
    // Movement
    // ==========================================

    int speed;

    int direction;


    // ==========================================
    // Lane
    // ==========================================

    int laneHeight;


    // ==========================================
    // Texture
    // ==========================================

    std::string textureId;
};