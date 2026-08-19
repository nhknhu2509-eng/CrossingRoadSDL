#include "Objects/Animal.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"


// ==========================================
// DEBUG HITBOX
// ==========================================
//
// true  -> hiện hitbox màu xanh
// false -> ẩn hitbox
//

constexpr bool DEBUG_ANIMAL_HITBOX = true;


Animal::Animal()
{
    // ==============================
    // SPRITE
    // ==============================

    rect.x = 0;
    rect.y = 0;

    // Kích thước mặc định.
    // Sẽ được thay đổi khi SetTexture().
    rect.w = Config::SQUIRREL_WIDTH;
    rect.h = Config::SQUIRREL_HEIGHT;


    // ==============================
    // HITBOX
    // ==============================

    hitbox.x = 0;
    hitbox.y = 0;
    hitbox.w = 0;
    hitbox.h = 0;


    // ==============================
    // MOVEMENT
    // ==============================

    speed = Config::ANIMAL_SPEED;

    direction = 1;


    // ==============================
    // TEXTURE
    // ==============================

    textureId = "deer";


    // ==============================
    // DEFAULT HITBOX
    // ==============================

    hitboxLeft = 5;
    hitboxTop = 8;
    hitboxRight = 6;
    hitboxBottom = 8;


    UpdateHitbox();
}


// ==========================================
// UPDATE
// ==========================================

void Animal::Update()
{
    rect.x += speed * direction;


    // ==============================
    // WRAP SCREEN
    // ==============================

    if (rect.x > Config::WINDOW_WIDTH)
    {
        rect.x = -rect.w;
    }


    if (rect.x + rect.w < 0)
    {
        rect.x = Config::WINDOW_WIDTH;
    }


    UpdateHitbox();
}


// ==========================================
// DRAW
// ==========================================

void Animal::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    Texture* texture =
        textureManager.GetTexture(textureId);


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
            0,
            255,
            0,
            255);

        SDL_RenderFillRect(
            renderer,
            &rect);
    }


    // ==============================
    // DEBUG HITBOX
    // ==============================

    if (DEBUG_ANIMAL_HITBOX)
    {
        SDL_SetRenderDrawColor(
            renderer,
            0,
            255,
            0,
            255);

        SDL_RenderDrawRect(
            renderer,
            &hitbox);
    }
}


// ==========================================
// SPEED
// ==========================================

void Animal::SetSpeed(int value)
{
    speed = value;
}


// ==========================================
// DIRECTION
// ==========================================

void Animal::SetDirection(int value)
{
    direction = value;
}


// ==========================================
// POSITION
// ==========================================

void Animal::SetPosition(
    int x,
    int y)
{
    rect.x = x;
    rect.y = y;

    UpdateHitbox();
}


// ==========================================
// TEXTURE
// ==========================================

void Animal::SetTexture(
    const std::string& id)
{
    textureId = id;


    // ==========================================
    // KÍCH THƯỚC RIÊNG CHO TỪNG SPRITE
    // ==========================================

    if (textureId == "deer")
    {
        rect.w = Config::DEER_WIDTH;
        rect.h = Config::DEER_HEIGHT;
    }
    else if (textureId == "squirrel")
    {
        rect.w = Config::SQUIRREL_WIDTH;
        rect.h = Config::SQUIRREL_HEIGHT;
    }
    else if (textureId == "rabbit")
    {
        rect.w = Config::RABBIT_WIDTH;
        rect.h = Config::RABBIT_HEIGHT;
    }


    // Kích thước thay đổi
    // nên phải cập nhật hitbox.
    UpdateHitbox();
}


// ==========================================
// HITBOX MARGINS
// ==========================================

void Animal::SetHitboxMargins(
    int left,
    int top,
    int right,
    int bottom)
{
    hitboxLeft = left;
    hitboxTop = top;
    hitboxRight = right;
    hitboxBottom = bottom;

    UpdateHitbox();
}


// ==========================================
// GET RECT
// ==========================================

SDL_Rect Animal::GetRect() const
{
    return rect;
}


// ==========================================
// GET HITBOX
// ==========================================

SDL_Rect Animal::GetHitbox() const
{
    return hitbox;
}


// ==========================================
// UPDATE HITBOX
// ==========================================

void Animal::UpdateHitbox()
{
    hitbox.x =
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
}