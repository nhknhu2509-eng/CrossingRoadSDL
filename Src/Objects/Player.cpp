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
}


void Player::Update()
{
    if (InputManager::IsKeyDown(SDL_SCANCODE_W) ||
        InputManager::IsKeyDown(SDL_SCANCODE_UP))
    {
        rect.y -= speed;
    }

    if (InputManager::IsKeyDown(SDL_SCANCODE_S) ||
        InputManager::IsKeyDown(SDL_SCANCODE_DOWN))
    {
        rect.y += speed;
    }

    if (InputManager::IsKeyDown(SDL_SCANCODE_A) ||
        InputManager::IsKeyDown(SDL_SCANCODE_LEFT))
    {
        rect.x -= speed;
    }

    if (InputManager::IsKeyDown(SDL_SCANCODE_D) ||
        InputManager::IsKeyDown(SDL_SCANCODE_RIGHT))
    {
        rect.x += speed;
    }


    // =====================================
    // GIỚI HẠN PLAYER TRONG MÀN HÌNH
    // =====================================

    if (rect.x < 0)
    {
        rect.x = 0;
    }

    if (rect.y < 0)
    {
        rect.y = 0;
    }

    if (rect.x + rect.w > Config::WINDOW_WIDTH)
    {
        rect.x =
            Config::WINDOW_WIDTH - rect.w;
    }

    if (rect.y + rect.h > Config::WINDOW_HEIGHT)
    {
        rect.y =
            Config::WINDOW_HEIGHT - rect.h;
    }
}


void Player::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    Texture* texture =
        textureManager.GetTexture("player");


    // =====================================
    // DRAW PLAYER SPRITE
    // =====================================

    if (texture != nullptr &&
        texture->GetTexture() != nullptr)
    {
        SDL_RenderCopy(
            renderer,
            texture->GetTexture(),
            nullptr,
            &rect);
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


    // =====================================
    // DEBUG PLAYER HITBOX
    // =====================================

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


void Player::Reset()
{
    rect.x = Config::PLAYER_START_X;
    rect.y = Config::PLAYER_START_Y;
}


void Player::SetPosition(
    int x,
    int y)
{
    rect.x = x;
    rect.y = y;
}


const SDL_Rect& Player::GetRect() const
{
    return rect;
}


SDL_Rect Player::GetHitbox() const
{
    SDL_Rect hitbox;


    // =====================================
    // PLAYER HITBOX
    // =====================================
    //
    // Sprite hiện tại: 70 x 70
    //
    // Hitbox:
    // - rộng khoảng 40% sprite
    // - cao khoảng 45% sprite
    // - nằm giữa theo chiều ngang
    // - nằm ở phần dưới cơ thể
    //
    // Dùng % nên nếu sau này đổi kích thước
    // Player thì hitbox tự thay đổi theo.
    // =====================================

    constexpr float HITBOX_WIDTH_RATIO =
        0.40f;

    constexpr float HITBOX_HEIGHT_RATIO =
        0.45f;

    constexpr float BOTTOM_MARGIN_RATIO =
        0.05f;


    hitbox.w =
        static_cast<int>(
            rect.w * HITBOX_WIDTH_RATIO);

    hitbox.h =
        static_cast<int>(
            rect.h * HITBOX_HEIGHT_RATIO);
/*

    // Căn giữa theo chiều ngang
    hitbox.x =
        rect.x +
        (rect.w - hitbox.w) / 2;


    // Đặt hitbox ở phần dưới nhân vật
    int bottomMargin =
        static_cast<int>(
            rect.h * BOTTOM_MARGIN_RATIO);

    hitbox.y =
        rect.y +
        rect.h -
        hitbox.h -
        bottomMargin; 
    */
    // =====================================
   // PLAYER HITBOX
   // =====================================
   // Giảm nhiều ở hai bên
    hitbox.x = rect.x + 15;
    hitbox.w = rect.w - 30;

    // Giảm nhẹ phía trên và phía dưới
    hitbox.y = rect.y + 8;
    hitbox.h = rect.h - 16;

    return hitbox;
    
  /* hitbox.x =
   rect.x + hitboxLeft;


   hitbox.y =
       rect.y + hitboxTop;


   hitbox.w =
       rect.w
       - hitboxLeft
       - hitboxRight;


   hitbox.h =
       rect.h
       - hitboxTop
       - hitboxBottom;
   

    return hitbox;*/
}