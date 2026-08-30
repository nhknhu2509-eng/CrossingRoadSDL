#include "Objects/Player.h"

#include "Managers/InputManager.h"
#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"


// ==================================================
// CONSTRUCTOR
// ==================================================

Player::Player()
{
    // ==========================================
    // RECT
    // ==========================================
    //
    // rect được kế thừa từ GameObject.
    //
    // Giữ nguyên toàn bộ thông số hiện tại.
    // ==========================================

    rect.x =
        Config::PLAYER_START_X;

    rect.y =
        Config::PLAYER_START_Y;


    rect.w =
        Config::PLAYER_WIDTH;

    rect.h =
        Config::PLAYER_HEIGHT;


    // ==========================================
    // SPEED
    // ==========================================

    speed =
        Config::PLAYER_SPEED;


    // ==========================================
    // ANIMATION
    // ==========================================

    currentTextureId =
        "player_idle";


    currentFrame = 0;


    lastFrameTime =
        SDL_GetTicks();


    // Giữ nguyên:
    // 130 ms / frame
    frameDuration = 130;


    moving = false;


    movingHorizontal = false;


    facingLeft = false;
}


// ==================================================
// UPDATE
// ==================================================

void Player::Update()
{
    moving = false;


    movingHorizontal = false;


    // ==========================================
    // UP
    // ==========================================

    if (
        InputManager::IsKeyDown(
            SDL_SCANCODE_W)
        ||
        InputManager::IsKeyDown(
            SDL_SCANCODE_UP))
    {
        rect.y -=
            speed;


        moving = true;


        movingHorizontal = false;
    }


    // ==========================================
    // DOWN
    // ==========================================

    if (
        InputManager::IsKeyDown(
            SDL_SCANCODE_S)
        ||
        InputManager::IsKeyDown(
            SDL_SCANCODE_DOWN))
    {
        rect.y +=
            speed;


        moving = true;


        movingHorizontal = false;
    }


    // ==========================================
    // LEFT
    // ==========================================

    if (
        InputManager::IsKeyDown(
            SDL_SCANCODE_A)
        ||
        InputManager::IsKeyDown(
            SDL_SCANCODE_LEFT))
    {
        rect.x -=
            speed;


        moving = true;


        movingHorizontal = true;


        facingLeft = true;
    }


    // ==========================================
    // RIGHT
    // ==========================================

    if (
        InputManager::IsKeyDown(
            SDL_SCANCODE_D)
        ||
        InputManager::IsKeyDown(
            SDL_SCANCODE_RIGHT))
    {
        rect.x +=
            speed;


        moving = true;


        movingHorizontal = true;


        facingLeft = false;
    }


    // ==========================================
    // ANIMATION
    // ==========================================

    if (!moving)
    {
        currentTextureId =
            "player_idle";


        currentFrame = 0;
    }
    else
    {
        Uint32 now =
            SDL_GetTicks();


        if (
            now - lastFrameTime >=
            frameDuration)
        {
            currentFrame =
                (currentFrame + 1) % 2;


            lastFrameTime =
                now;
        }


        // ======================================
        // HORIZONTAL
        // ======================================

        if (movingHorizontal)
        {
            if (currentFrame == 0)
            {
                currentTextureId =
                    "player_walkright1";
            }
            else
            {
                currentTextureId =
                    "player_walkright2";
            }
        }


        // ======================================
        // VERTICAL
        // ======================================

        else
        {
            if (currentFrame == 0)
            {
                currentTextureId =
                    "player_walkupdown1";
            }
            else
            {
                currentTextureId =
                    "player_walkupdown2";
            }
        }
    }


    // ==========================================
    // SCREEN LIMIT - LEFT
    // ==========================================

    if (rect.x < 0)
    {
        rect.x = 0;
    }


    // ==========================================
    // SCREEN LIMIT - TOP
    // ==========================================

    if (rect.y < 0)
    {
        rect.y = 0;
    }


    // ==========================================
    // SCREEN LIMIT - RIGHT
    // ==========================================

    if (
        rect.x + rect.w >
        Config::WINDOW_WIDTH)
    {
        rect.x =
            Config::WINDOW_WIDTH
            - rect.w;
    }


    // ==========================================
    // SCREEN LIMIT - BOTTOM
    // ==========================================

    if (
        rect.y + rect.h >
        Config::WINDOW_HEIGHT)
    {
        rect.y =
            Config::WINDOW_HEIGHT
            - rect.h;
    }
}


// ==================================================
// DRAW
// ==================================================

void Player::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    Texture* texture =
        textureManager.GetTexture(
            currentTextureId);


    if (
        texture != nullptr
        &&
        texture->GetTexture() != nullptr)
    {
        SDL_RendererFlip flip =
            SDL_FLIP_NONE;


        // ======================================
        // LEFT FLIP
        // ==========================================

        if (
            movingHorizontal
            &&
            facingLeft)
        {
            flip =
                SDL_FLIP_HORIZONTAL;
        }


        SDL_RenderCopyEx(
            renderer,
            texture->GetTexture(),
            nullptr,
            &rect,
            0.0,
            nullptr,
            flip);
    }
    else
    {
        // ======================================
        // FALLBACK
        // ==========================================

        SDL_SetRenderDrawColor(
            renderer,
            Config::PLAYER_COLOR.r,
            Config::PLAYER_COLOR.g,
            Config::PLAYER_COLOR.b,
            Config::PLAYER_COLOR.a);


        SDL_RenderFillRect(
            renderer,
            &rect);
    }


    // ==========================================
    // DEBUG HITBOX
    // ==========================================

    SDL_Rect hitbox =
        GetHitbox();


    // Giữ nguyên màu xanh hiện tại
    SDL_SetRenderDrawColor(
        renderer,
        0,
        0,
        255,
        255);


    SDL_RenderDrawRect(
        renderer,
        &hitbox);
}


// ==================================================
// RESET
// ==================================================

void Player::Reset()
{
    // Giữ nguyên vị trí reset hiện tại

    rect.x =
        Config::PLAYER_START_X;


    rect.y =
        Config::PLAYER_START_Y;


    // Giữ nguyên animation reset

    currentTextureId =
        "player_idle";


    currentFrame = 0;
}


// ==================================================
// SET POSITION
// ==================================================

void Player::SetPosition(
    int x,
    int y)
{
    // Dùng implementation chung của GameObject.
    //
    // Kết quả vẫn chính xác:
    //
    // rect.x = x;
    // rect.y = y;

    GameObject::SetPosition(
        x,
        y);
}


// ==================================================
// HITBOX
// ==================================================

SDL_Rect
Player::GetHitbox() const
{
    SDL_Rect hitbox;


    // ==========================================
    // GIỮ NGUYÊN 100% HITBOX HIỆN TẠI
    // ==========================================

    hitbox.x =
        rect.x + 15;


    hitbox.w =
        rect.w - 30;


    hitbox.y =
        rect.y + 8;


    hitbox.h =
        rect.h - 16;


    return hitbox;
}