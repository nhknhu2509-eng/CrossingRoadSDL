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
    // SPRITE
    // ==========================================

    rect.x = 0;
    rect.y = 0;

    rect.w = Config::VEHICLE_WIDTH;
    rect.h = Config::VEHICLE_HEIGHT;


    // ==========================================
    // HITBOX
    // ==========================================

    hitbox.x = 0;
    hitbox.y = 0;
    hitbox.w = 0;
    hitbox.h = 0;


    // ==========================================
    // MOVEMENT
    // ==========================================

    speed = Config::VEHICLE_SPEED;

    direction = 1;


    // ==========================================
    // TEXTURE
    // ==========================================

    textureId = "wagon_01";


    // ==========================================
    // LANE
    // ==========================================

    laneHeight = Config::LANE_HEIGHT;


    UpdateHitbox();
}


// ==================================================
// UPDATE
// ==================================================

void Vehicle::Update()
{
    rect.x += speed * direction;


    // ==========================================
    // WRAP RIGHT
    // ==========================================

    if (rect.x > Config::WINDOW_WIDTH)
    {
        rect.x = -rect.w;
    }


    // ==========================================
    // WRAP LEFT
    // ==========================================

    if (rect.x + rect.w < 0)
    {
        rect.x = Config::WINDOW_WIDTH;
    }


    // ==========================================
    // UPDATE HITBOX
    // ==========================================

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


    // ==========================================
    // DRAW SPRITE
    // ==========================================

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
//
// Đây là công thức hitbox cũ của WAGON.
//
// Deer cũng là Vehicle nên sẽ dùng chính xác
// công thức này.
//
// 1. Chừa 5% mỗi bên.
// 2. Hitbox chỉ lấy phần dưới.
// 3. Chiều cao hitbox = 1/2 chiều cao lane.
// 4. Đáy hitbox trùng đáy sprite.
//
// ==================================================

void Vehicle::UpdateHitbox()
{
    // ==========================================
    // CHIỀU NGANG
    // ==========================================

    constexpr float SIDE_REDUCTION = 0.05f;

    int sideOffset =
        static_cast<int>(
            rect.w * SIDE_REDUCTION);

    hitbox.x =
        rect.x + sideOffset;

    hitbox.w =
        rect.w - (sideOffset * 2);


    // ==========================================
    // CHIỀU DỌC
    // ==========================================

    // Cạnh dưới của sprite
    int spriteBottom =
        rect.y + rect.h;


    // ==================================================
    // QUAN TRỌNG:
    //
    // Đây là CÔNG THỨC CHÍNH XÁC của bản
    // "Fix hitbox okay".
    //
    // Không đổi thứ tự 3 dòng dưới.
    // ==================================================

    hitbox.h =
        laneHeight;


    hitbox.y =
        spriteBottom - hitbox.h;


    hitbox.h =
        laneHeight / 2;


    // ==========================================
    // SAFETY
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