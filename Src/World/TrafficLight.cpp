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

    state =
        LightState::Green;


    // =====================================
    // THỜI GIAN RIÊNG CỦA TỪNG ĐÈN
    // =====================================

    greenTime =
        greenDuration;

    yellowTime =
        yellowDuration;

    redTime =
        redDuration;


    lastSwitchTime =
        SDL_GetTicks();
}


// ==================================================
// UPDATE
// ==================================================

void TrafficLight::Update()
{
    Uint32 now =
        SDL_GetTicks();


    switch (state)
    {
    case LightState::Green:

        if (
            now - lastSwitchTime >=
            greenTime)
        {
            state =
                LightState::Yellow;


            lastSwitchTime =
                now;
        }

        break;


    case LightState::Yellow:

        if (
            now - lastSwitchTime >=
            yellowTime)
        {
            state =
                LightState::Red;


            lastSwitchTime =
                now;
        }

        break;


    case LightState::Red:

        if (
            now - lastSwitchTime >=
            redTime)
        {
            state =
                LightState::Green;


            lastSwitchTime =
                now;
        }

        break;


    default:

        break;
    }
}


// ==================================================
// CAN MOVE
// ==================================================

bool TrafficLight::CanMove() const
{
    return
        state !=
        LightState::Red;
}


// ==================================================
// GET SAVE STATE
// ==================================================

TrafficLightSaveState
TrafficLight::GetSaveState() const
{
    TrafficLightSaveState
        saveState;


    saveState.state =
        state;


    saveState.elapsedTime =
        SDL_GetTicks() -
        lastSwitchTime;


    return saveState;
}


// ==================================================
// RESTORE SAVE STATE
// ==================================================

void TrafficLight::RestoreSaveState(
    const TrafficLightSaveState& saveState)
{
    state =
        saveState.state;


    Uint32 maxElapsed =
        0;


    switch (state)
    {
    case LightState::Green:

        maxElapsed =
            greenTime;

        break;


    case LightState::Yellow:

        maxElapsed =
            yellowTime;

        break;


    case LightState::Red:

        maxElapsed =
            redTime;

        break;


    default:

        state =
            LightState::Green;

        maxElapsed =
            greenTime;

        break;
    }


    // ==========================================
    // KHÔNG CHO ELAPSED VƯỢT QUÁ DURATION
    // ==========================================

    Uint32 elapsed =
        saveState.elapsedTime;


    if (
        elapsed >
        maxElapsed)
    {
        elapsed =
            maxElapsed;
    }


    // ==========================================
    // TẠO LẠI MỐC THỜI GIAN
    //
    // Ví dụ:
    //
    // elapsed = 3000
    //
    // thì coi như trạng thái hiện tại
    // đã bắt đầu cách đây 3000 ms.
    // ==========================================

    Uint32 now =
        SDL_GetTicks();


    if (
        elapsed >
        now)
    {
        elapsed =
            now;
    }


    lastSwitchTime =
        now -
        elapsed;
}


// ==================================================
// DRAW
// ==================================================

void TrafficLight::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    const char* textureId =
        "lantern_green";


    // =====================================
    // CHỌN SPRITE THEO MÀU ĐÈN
    // =====================================

    if (
        state ==
        LightState::Red)
    {
        textureId =
            "lantern_red";
    }


    Texture* texture =
        textureManager.GetTexture(
            textureId);


    // =====================================
    // FALLBACK
    // =====================================

    if (
        texture == nullptr ||
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
    // YELLOW
    // =====================================

    if (
        state ==
        LightState::Yellow)
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
    // RESET TEXTURE COLOR
    // =====================================

    SDL_SetTextureColorMod(
        texture->GetTexture(),
        255,
        255,
        255);
}