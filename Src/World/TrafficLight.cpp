#include "World/TrafficLight.h"

#include "Graphics/TextureManager.h"

TrafficLight::TrafficLight(int x, int y)
{
    // Giữ nguyên tâm của đèn cũ:
    // đèn cũ có x = 760, width = 20 -> tâm x = 770.
    // Sprite mới rộng 60 -> x = 740.
    // y cũ bắt đầu tại đầu lane -> căn giữa theo chiều cao lane 82.
    rect.x = x - 20;
    rect.y = y + 11;
    rect.w = 60;
    rect.h = 60;

    state = LightState::Green;

    lastSwitchTime = SDL_GetTicks();
}

void TrafficLight::Update()
{
    Uint32 now = SDL_GetTicks();

    switch (state)
    {
    case LightState::Green:
        if (now - lastSwitchTime >= greenTime)
        {
            state = LightState::Yellow;
            lastSwitchTime = now;
        }
        break;

    case LightState::Yellow:
        if (now - lastSwitchTime >= yellowTime)
        {
            state = LightState::Red;
            lastSwitchTime = now;
        }
        break;

    case LightState::Red:
        if (now - lastSwitchTime >= redTime)
        {
            state = LightState::Green;
            lastSwitchTime = now;
        }
        break;

    default:
        break;
    }
}

bool TrafficLight::CanMove() const
{
    return state != LightState::Red;
}

void TrafficLight::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    const char* textureId = "lantern_green";

    if (state == LightState::Red)
    {
        textureId = "lantern_red";
    }

    Texture* texture =
        textureManager.GetTexture(textureId);

    if (texture == nullptr ||
        texture->GetTexture() == nullptr)
    {
        // Fallback nếu sprite không load được.
        switch (state)
        {
        case LightState::Green:
            SDL_SetRenderDrawColor(
                renderer, 0, 255, 0, 255);
            break;

        case LightState::Yellow:
            SDL_SetRenderDrawColor(
                renderer, 255, 255, 0, 255);
            break;

        case LightState::Red:
            SDL_SetRenderDrawColor(
                renderer, 255, 0, 0, 255);
            break;

        default:
            break;
        }

        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    // Repo hiện có sprite xanh và đỏ.
    // Khi vàng, tạm tint sprite xanh thành vàng.
    if (state == LightState::Yellow)
    {
        SDL_SetTextureColorMod(
            texture->GetTexture(),
            255, 255, 0);
    }

    SDL_RenderCopy(
        renderer,
        texture->GetTexture(),
        nullptr,
        &rect);

    // Trả màu texture về bình thường.
    SDL_SetTextureColorMod(
        texture->GetTexture(),
        255, 255, 255);
}
