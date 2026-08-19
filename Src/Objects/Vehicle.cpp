#include "Objects/Vehicle.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"


// ==================================================
// DEBUG HITBOX
// ==================================================

constexpr bool DEBUG_HITBOX = true;


// ==================================================
// CONSTRUCTOR
// ==================================================

Vehicle::Vehicle()
{
    // ==========================================
    // Sprite
    // ==========================================

    rect.x = 0;
    rect.y = 0;

    rect.w = Config::VEHICLE_WIDTH;
    rect.h = Config::VEHICLE_HEIGHT;


    // ==========================================
    // Hitbox
    // ==========================================

    hitbox.x = 0;
    hitbox.y = 0;
    hitbox.w = 0;
    hitbox.h = 0;


    // ==========================================
    // Movement
    // ==========================================

    speed = Config::VEHICLE_SPEED;

    direction = 1;


    // ==========================================
    // Texture
    // ==========================================

    textureId = "wagon_01";


    // ==========================================
    // Lane
    // ==========================================

    laneHeight = Config::LANE_HEIGHT;


    // ==========================================
    // Default hitbox
    // ==========================================
    //
    // Wagon mặc định lấy phần dưới.
    //

    hitboxBottomHeight =
        laneHeight / 2;


    UpdateHitbox();
}


// ==================================================
// UPDATE
// ==================================================

void Vehicle::Update()
{
    rect.x += speed * direction;


    // ==========================================
    // Wrap right
    // ==========================================

    if (rect.x > Config::WINDOW_WIDTH)
    {
        rect.x = -rect.w;
    }


    // ==========================================
    // Wrap left
    // ==========================================

    if (rect.x + rect.w < 0)
    {
        rect.x = Config::WINDOW_WIDTH;
    }


    UpdateHitbox();
}


// ==================================================
// DRAW
// ==================================================

void Vehicle::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    Texture* texture =
        textureManager.GetTexture(textureId);


    if (texture != nullptr &&
        texture->GetTexture() != nullptr)
    {
        if (direction > 0)
        {
            SDL_RenderCopy(
                renderer,
                texture->GetTexture(),
                nullptr,
                &rect);
        }
        else
        {
            SDL_RenderCopyEx(
                renderer,
                texture->GetTexture(),
                nullptr,
                &rect,
                0.0,
                nullptr,
                SDL_FLIP_HORIZONTAL);
        }
    }
    else
    {
        SDL_SetRenderDrawColor(
            renderer,
            Config::VEHICLE_COLOR.r,
            Config::VEHICLE_COLOR.g,
            Config::VEHICLE_COLOR.b,
            Config::VEHICLE_COLOR.a);

        SDL_RenderFillRect(
            renderer,
            &rect);
    }


    // ==========================================
    // DEBUG HITBOX
    // ==========================================

    if (DEBUG_HITBOX)
    {
        SDL_SetRenderDrawColor(
            renderer,
            255,
            0,
            0,
            255);

        SDL_RenderDrawRect(
            renderer,
            &hitbox);
    }
}


// ==================================================
// SET SPEED
// ==================================================

void Vehicle::SetSpeed(int value)
{
    speed = value;
}


// ==================================================
// SET DIRECTION
// ==================================================

void Vehicle::SetDirection(int value)
{
    direction = value;
}


// ==================================================
// SET POSITION
// ==================================================

void Vehicle::SetPosition(
    int x,
    int y)
{
    rect.x = x;
    rect.y = y;

    UpdateHitbox();
}


// ==================================================
// SET LANE HEIGHT
// ==================================================

void Vehicle::SetLaneHeight(
    int height)
{
    laneHeight = height;

    UpdateHitbox();
}


// ==================================================
// SET TEXTURE
// ==================================================

void Vehicle::SetTexture(
    const std::string& id)
{
    textureId = id;
}


// ==================================================
// SET SPRITE SIZE
// ==================================================

void Vehicle::SetSpriteSize(
    int width,
    int height)
{
    rect.w = width;
    rect.h = height;

    UpdateHitbox();
}


// ==================================================
// SET HITBOX BOTTOM HEIGHT
// ==================================================
//
// Đây là thông số quan trọng.
//
// Ví dụ:
//
// Wagon:
//     41
//
// Deer:
//     35
//
// Hitbox luôn nằm sát đáy sprite.
//

void Vehicle::SetHitboxBottomHeight(
    int height)
{
    hitboxBottomHeight = height;

    UpdateHitbox();
}


// ==================================================
// GET RECT
// ==================================================

SDL_Rect Vehicle::GetRect() const
{
    return rect;
}


// ==================================================
// GET HITBOX
// ==================================================

SDL_Rect Vehicle::GetHitbox() const
{
    return hitbox;
}


// ==================================================
// UPDATE HITBOX
// ==================================================

void Vehicle::UpdateHitbox()
{
    // ==========================================
    // Hai bên
    // ==========================================
    //
    // Chừa một chút phần trong suốt bên trái
    // và bên phải.
    //

    constexpr int SIDE_MARGIN = 5;


    hitbox.x =
        rect.x + SIDE_MARGIN;


    hitbox.w =
        rect.w - SIDE_MARGIN * 2;


    // ==========================================
    // Phần dưới
    // ==========================================
    //
    // Đây là điểm khác biệt quan trọng:
    //
    // Hitbox KHÔNG bao toàn bộ sprite.
    //
    // Nó chỉ lấy phần dưới.
    //

    hitbox.h =
        hitboxBottomHeight;


    hitbox.y =
        rect.y
        + rect.h
        - hitboxBottomHeight;


    // ==========================================
    // Safety
    // ==========================================

    if (hitbox.w < 0)
    {
        hitbox.w = 0;
    }


    if (hitbox.h < 0)
    {
        hitbox.h = 0;
    }
}