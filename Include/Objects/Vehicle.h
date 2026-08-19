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
    // Lane
    // ==============================

    int laneHeight;


    // ==============================
    // Texture
    // ==============================

    std::string textureId;
};