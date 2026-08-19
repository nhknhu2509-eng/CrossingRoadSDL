#include "Objects/Vehicle.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"

// Bật để nhìn thấy hitbox.
// Khi căn chỉnh xong có thể đổi thành false.
constexpr bool DEBUG_HITBOX = true;

Vehicle::Vehicle()
{
    rect.x = 0;
    rect.y = 0;

    rect.w = Config::VEHICLE_WIDTH;
    rect.h = Config::VEHICLE_HEIGHT;

    hitbox.x = 0;
    hitbox.y = 0;
    hitbox.w = 0;
    hitbox.h = 0;

    speed = Config::VEHICLE_SPEED;
    direction = 1;

    textureId = "wagon_01";

    laneY = 0;
    laneHeight = Config::LANE_HEIGHT;

    UpdateHitbox();
}

void Vehicle::Update()
{
    rect.x += speed * direction;

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

void Vehicle::Draw(
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

void Vehicle::SetLaneY(int y)
{
    laneY = y;

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

void Vehicle::UpdateHitbox()
{
    /*
        ============================
        HITBOX
        ============================

        Hitbox được xác định độc lập
        với vị trí sprite.

        Y của hitbox = Y của lane.
        Chiều cao hitbox = chiều cao lane.

        Như vậy khi sprite thay đổi
        kích thước hoặc vị trí, hitbox
        vẫn nằm đúng trong lane.
    */

    constexpr float SIDE_REDUCTION = 0.05f;

    // ----------------------------
    // Chiều ngang
    // ----------------------------

    int sideOffset =
        static_cast<int>(
            rect.w * SIDE_REDUCTION);

    hitbox.x =
        rect.x + sideOffset;

    hitbox.w =
        rect.w - (sideOffset * 2);

    // ----------------------------
    // Chiều dọc
    // ----------------------------

    hitbox.y = laneY;
    hitbox.h = laneHeight;
}