#include "Objects/Vehicle.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"

// Bật để nhìn thấy hitbox.
// Khi căn chỉnh xong có thể đổi thành false.
constexpr bool DEBUG_HITBOX = true;


Vehicle::Vehicle()
{
    // =========================
    // SPRITE
    // =========================

    rect.x = 0;
    rect.y = 0;

    rect.w = Config::VEHICLE_WIDTH;
    rect.h = Config::VEHICLE_HEIGHT;


    // =========================
    // HITBOX
    // =========================

    hitbox.x = 0;
    hitbox.y = 0;
    hitbox.w = 0;
    hitbox.h = 0;


    // =========================
    // VEHICLE
    // =========================

    speed = Config::VEHICLE_SPEED;
    direction = 1;

    textureId = "wagon_01";

    // Chiều cao lane mặc định
    laneHeight = Config::LANE_HEIGHT;

    UpdateHitbox();
}


void Vehicle::Update()
{
    // Xe chạy ngang
    rect.x += speed * direction;


    // Ra khỏi bên phải
    if (rect.x > Config::WINDOW_WIDTH)
    {
        rect.x = -rect.w;
    }


    // Ra khỏi bên trái
    if (rect.x + rect.w < 0)
    {
        rect.x = Config::WINDOW_WIDTH;
    }


    // Cập nhật hitbox theo sprite
    UpdateHitbox();
}


void Vehicle::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    Texture* texture =
        textureManager.GetTexture(textureId);


    // =========================
    // DRAW SPRITE
    // =========================

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
            Config::VEHICLE_COLOR.r,
            Config::VEHICLE_COLOR.g,
            Config::VEHICLE_COLOR.b,
            Config::VEHICLE_COLOR.a);

        SDL_RenderFillRect(
            renderer,
            &rect);
    }


    // =========================
    // DEBUG HITBOX
    // =========================

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


void Vehicle::SetSpeed(int value)
{
    speed = value;
}


void Vehicle::SetDirection(int value)
{
    direction = value;
}


void Vehicle::SetPosition(int x, int y)
{
    rect.x = x;
    rect.y = y;

    UpdateHitbox();
}


void Vehicle::SetLaneHeight(int height)
{
    laneHeight = height;

    UpdateHitbox();
}


void Vehicle::SetTexture(const std::string& id)
{
    textureId = id;
}


SDL_Rect Vehicle::GetRect() const
{
    return rect;
}


SDL_Rect Vehicle::GetHitbox() const
{
    return hitbox;
}


// ============================================================
// UPDATE HITBOX
// ============================================================
//
// Mục tiêu:
//
// 1. Hitbox không bao toàn bộ sprite.
// 2. Chiều cao hitbox = chiều cao lane.
// 3. Cạnh dưới hitbox = cạnh dưới sprite.
// 4. Hitbox chỉ nằm ở phần dưới của xe.
//
// ============================================================

void Vehicle::UpdateHitbox()
{
    // =========================
    // CHIỀU NGANG
    // =========================

    constexpr float SIDE_REDUCTION = 0.05f;

    int sideOffset =
        static_cast<int>(
            rect.w * SIDE_REDUCTION);

    hitbox.x =
        rect.x + sideOffset;

    hitbox.w =
        rect.w - (sideOffset * 2);


    // =========================
    // CHIỀU DỌC
    // =========================

    // Cạnh dưới của sprite
    int spriteBottom =
        rect.y + rect.h;


    // Hitbox cao đúng bằng lane
    hitbox.h =
        laneHeight;


    // Cạnh dưới hitbox trùng
    // cạnh dưới sprite
    hitbox.y =
        spriteBottom - hitbox.h;
    hitbox.h =
        laneHeight/2;
}