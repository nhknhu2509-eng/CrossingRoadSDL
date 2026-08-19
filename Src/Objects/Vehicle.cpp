#include "Objects/Vehicle.h"
#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"

Vehicle::Vehicle()
{
    rect.x = 0;
    rect.y = 0;

    rect.w = Config::VEHICLE_WIDTH;
    rect.h = Config::VEHICLE_HEIGHT;

    speed = Config::VEHICLE_SPEED;
    direction = 1;

    textureId = "wagon_01";
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
        // Nếu texture không load được,
        // vẫn vẽ hình chữ nhật để debug.
        SDL_SetRenderDrawColor(
            renderer,
            Config::VEHICLE_COLOR.r,
            Config::VEHICLE_COLOR.g,
            Config::VEHICLE_COLOR.b,
            Config::VEHICLE_COLOR.a);

        SDL_RenderFillRect(renderer, &rect);
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
}

void Vehicle::SetTexture(const std::string& id)
{
    textureId = id;
}

SDL_Rect Vehicle::GetRect() const
{
    return rect;
}