#include "World/TrafficLight.h"

#include "Graphics/TextureManager.h"


TrafficLight::TrafficLight(
    int x,
    int y,
    Uint32 greenDuration,
    Uint32 yellowDuration,
    Uint32 redDuration)
{
    // =====================================
    // VỊ TRÍ VÀ KÍCH THƯỚC ĐÈN
    // =====================================

    rect.x = x - 20;
    rect.y = y + 11;

    rect.w = 60;
    rect.h = 60;


    // =====================================
    // TRẠNG THÁI BAN ĐẦU
    // =====================================

    state = LightState::Green;


    // =====================================
    // THỜI GIAN RIÊNG CỦA TỪNG ĐÈN
    // =====================================

    greenTime = greenDuration;
    yellowTime = yellowDuration;
    redTime = redDuration;


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
    const char* textureId =
        "lantern_green";


    // =====================================
    // CHỌN SPRITE THEO MÀU ĐÈN
    // =====================================

    if (state == LightState::Red)
    {
        textureId =
            "lantern_red";
    }


    Texture* texture =
        textureManager.GetTexture(
            textureId);


    // =====================================
    // FALLBACK NẾU KHÔNG LOAD ĐƯỢC SPRITE
    // =====================================

    if (texture == nullptr ||
        texture->GetTexture() == nullptr)
    {
        switch (state)
        {
        case LightState::Green:

            SDL_SetRenderDrawColor(
                renderer,
                0,
                255,
                0,
                255);

            break;


        case LightState::Yellow:

            SDL_SetRenderDrawColor(
                renderer,
                255,
                255,
                0,
                255);

            break;


        case LightState::Red:

            SDL_SetRenderDrawColor(
                renderer,
                255,
                0,
                0,
                255);

            break;


        default:

            break;
        }


        SDL_RenderFillRect(
            renderer,
            &rect);

        return;
    }


    // =====================================
    // ĐÈN VÀNG
    // =====================================
    //
    // Hiện tại project có:
    //
    // lantern_green.png
    // lantern_red.png
    //
    // Chưa có lantern_yellow.png
    //
    // Vì vậy khi vàng sẽ lấy sprite xanh
    // rồi đổi màu thành vàng.
    // =====================================

    if (state == LightState::Yellow)
    {
        SDL_SetTextureColorMod(
            texture->GetTexture(),
            255,
            255,
            0);
    }


    // =====================================
    // DRAW
    // =====================================

    SDL_RenderCopy(
        renderer,
        texture->GetTexture(),
        nullptr,
        &rect);


    // =====================================
    // RESET MÀU TEXTURE
    // =====================================

    SDL_SetTextureColorMod(
        texture->GetTexture(),
        255,
        255,
        255);
}