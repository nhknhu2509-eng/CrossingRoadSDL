#include "Objects/Player.h"

#include "Managers/InputManager.h"
#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"


Player::Player()
{
    rect.x = Config::PLAYER_START_X;
    rect.y = Config::PLAYER_START_Y;

    rect.w = Config::PLAYER_WIDTH;
    rect.h = Config::PLAYER_HEIGHT;

    speed = Config::PLAYER_SPEED;


    currentTextureId = "player_idle";

    currentFrame = 0;

    lastFrameTime = SDL_GetTicks();

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


    // ==============================
    // UP
    // ==============================

    if (InputManager::IsKeyDown(SDL_SCANCODE_W) ||
        InputManager::IsKeyDown(SDL_SCANCODE_UP))
    {
        rect.y -= speed;

        moving = true;

        movingHorizontal = false;
    }


    // ==============================
    // DOWN
    // ==============================

    if (InputManager::IsKeyDown(SDL_SCANCODE_S) ||
        InputManager::IsKeyDown(SDL_SCANCODE_DOWN))
    {
        rect.y += speed;

        moving = true;

        movingHorizontal = false;
    }


    // ==============================
    // LEFT
    // ==============================

    if (InputManager::IsKeyDown(SDL_SCANCODE_A) ||
        InputManager::IsKeyDown(SDL_SCANCODE_LEFT))
    {
        rect.x -= speed;

        moving = true;

        movingHorizontal = true;

        facingLeft = true;
    }


    // ==============================
    // RIGHT
    // ==============================

    if (InputManager::IsKeyDown(SDL_SCANCODE_D) ||
        InputManager::IsKeyDown(SDL_SCANCODE_RIGHT))
    {
        rect.x += speed;

        moving = true;

        movingHorizontal = true;

        facingLeft = false;
    }


    // ==============================
    // ANIMATION
    // ==============================

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


        if (now - lastFrameTime >= frameDuration)
        {
            currentFrame =
                (currentFrame + 1) % 2;

            lastFrameTime = now;
        }


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


    // ==============================
    // SCREEN LIMIT
    // ==============================

    if (rect.x < 0)
    {
        rect.x = 0;
    }


    if (rect.y < 0)
    {
        rect.y = 0;
    }


    if (rect.x + rect.w >
        Config::WINDOW_WIDTH)
    {
        rect.x =
            Config::WINDOW_WIDTH -
            rect.w;
    }


    if (rect.y + rect.h >
        Config::WINDOW_HEIGHT)
    {
        rect.y =
            Config::WINDOW_HEIGHT -
            rect.h;
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


    if (texture != nullptr &&
        texture->GetTexture() != nullptr)
    {
        SDL_RendererFlip flip =
            SDL_FLIP_NONE;


        if (movingHorizontal &&
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


    // ==============================
    // DEBUG HITBOX
    // ==============================

    SDL_Rect hitbox =
        GetHitbox();


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
    rect.x =
        Config::PLAYER_START_X;

    rect.y =
        Config::PLAYER_START_Y;


    currentTextureId =
        "player_idle";

    currentFrame = 0;
}


// ==================================================
// POSITION
// ==================================================

void Player::SetPosition(
    int x,
    int y)
{
    rect.x = x;
    rect.y = y;
}


// ==================================================
// GET RECT
// ==================================================

const SDL_Rect& Player::GetRect() const
{
    return rect;
}


// ==================================================
// HITBOX
// ==================================================

SDL_Rect Player::GetHitbox() const
{
    SDL_Rect hitbox;


    // Giữ nguyên hitbox hiện tại của bạn
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