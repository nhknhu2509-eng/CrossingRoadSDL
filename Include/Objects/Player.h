#pragma once

#include <SDL.h>
#include <string>

#include "Objects/GameObject.h"


class TextureManager;


// ==================================================
// PLAYER
// ==================================================
//
// Player là một GameObject.
//
// Player kế thừa:
// - rect
// - GetRect()
//
// Player tự cài đặt:
// - Update()
// - Draw()
// - GetHitbox()
//
// ==================================================

class Player : public GameObject
{
public:

    Player();

    virtual ~Player() = default;


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
    // PLAYER
    // ==========================================

    void Reset();


private:

    int speed;


    // ==========================================
    // ANIMATION
    // ==========================================

    std::string currentTextureId;


    int currentFrame;


    Uint32 lastFrameTime;


    Uint32 frameDuration;


    bool moving;


    bool movingHorizontal;


    bool facingLeft;
};