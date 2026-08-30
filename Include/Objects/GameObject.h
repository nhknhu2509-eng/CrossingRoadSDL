#pragma once

#include <SDL.h>


class TextureManager;


class GameObject
{
public:

    GameObject();

    virtual ~GameObject() = default;


    // ==========================================
    // POLYMORPHIC INTERFACE
    // ==========================================

    virtual void Update() = 0;


    virtual void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager) = 0;


    virtual SDL_Rect GetHitbox() const = 0;


    // ==========================================
    // POSITION
    // ==========================================

    virtual void SetPosition(
        int x,
        int y);


    // ==========================================
    // RECT
    // ==========================================

    const SDL_Rect& GetRect() const;


protected:

    SDL_Rect rect;
};