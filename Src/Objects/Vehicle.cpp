#include "Objects/Vehicle.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"

// Bật để nhìn thấy hitbox.
// Sau khi căn chỉnh xong đổi thành false.
constexpr bool DEBUG_HITBOX = true;

Vehicle::Vehicle()
{
    rect.x = 0;
    rect.y = 0;

    rect.w = Config::VEHICLE_WIDTH;
    rect.h = Config::VEHICLE_HEIGHT;

    speed = Config::VEHICLE_SPEED;
    direction = 1;

    textureId = "wagon_01";

    // Chiều cao lane mặc định.
    // Lane sẽ cập nhật lại giá trị này.
    laneHeight = 82;

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

    // ============================
    // DEBUG HITBOX
    // ============================
    if (DEBUG_HITBOX)
    {
        // Màu đỏ
        SDL_SetRenderDrawColor(
            renderer,
            255,
            0,
            0,
            255);

        // Chỉ vẽ viền, không che sprite
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

void Vehicle::UpdateHitbox()
{
    /*
        HITBOX

        - Chiều cao = chiều cao lane
        - Hai bên chừa 5% chiều rộng sprite
        - Hitbox được căn giữa theo chiều dọc sprite

        Không dùng số pixel cố định.
    */

    constexpr float SIDE_REDUCTION = 0.05f;

    // Chừa 5% mỗi bên
    int sideOffset =
        static_cast<int>(
            rect.w * SIDE_REDUCTION);

    // Vị trí ngang
    hitbox.x =
        rect.x + sideOffset;

    // Chiều rộng = 90% chiều rộng sprite
    hitbox.w =
        rect.w - (sideOffset * 2);

    // Chiều cao hitbox = chiều cao lane
    hitbox.h = laneHeight;

    // Căn giữa hitbox theo chiều dọc
    hitbox.y =
        rect.y + (rect.h - hitbox.h) / 2;
}