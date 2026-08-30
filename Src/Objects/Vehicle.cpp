#include "Objects/Vehicle.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"


constexpr bool DEBUG_HITBOX = true;


// ==================================================
// CONSTRUCTOR
// ==================================================

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


    laneHeight = Config::LANE_HEIGHT;


    textureId = "wagon1";


    // ==============================
    // Animation
    // ==============================

    animationFrames =
    {
        "wagon1_01",
        "wagon1_02",
        "wagon1_03"
    };


    currentFrame = 0;

    lastFrameTime = SDL_GetTicks();

    // 130 ms / frame
    frameDuration = 130;


    UpdateHitbox();
}


// ==================================================
// UPDATE
// ==================================================

void Vehicle::Update()
{
    rect.x += speed * direction;


    UpdateAnimation();


    // ==============================
    // WRAP RIGHT
    // ==============================

    if (rect.x > Config::WINDOW_WIDTH)
    {
        rect.x = -rect.w;
    }


    // ==============================
    // WRAP LEFT
    // ==============================

    if (rect.x + rect.w < 0)
    {
        rect.x = Config::WINDOW_WIDTH;
    }


    UpdateHitbox();
}


// ==================================================
// ANIMATION
// ==================================================

void Vehicle::UpdateAnimation()
{
    if (animationFrames.empty())
    {
        return;
    }


    Uint32 now =
        SDL_GetTicks();


    if (now - lastFrameTime >= frameDuration)
    {
        currentFrame++;


        if (currentFrame >=
            static_cast<int>(animationFrames.size()))
        {
            currentFrame = 0;
        }


        lastFrameTime = now;
    }
}


// ==================================================
// DRAW
// ==================================================

void Vehicle::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    if (animationFrames.empty())
    {
        return;
    }


    const std::string& currentTextureId =
        animationFrames[currentFrame];


    Texture* texture =
        textureManager.GetTexture(
            currentTextureId);


    if (texture != nullptr &&
        texture->GetTexture() != nullptr)
    {
        SDL_RendererFlip flip =
            SDL_FLIP_NONE;


        if (direction < 0)
        {
            flip =
                SDL_FLIP_HORIZONTAL;
        }


        SDL_RenderCopyEx(
            renderer,
            texture->GetTexture(),
            nullptr,
            &rect,
            0.0,
            nullptr,
            flip);
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


    // ==============================
    // DEBUG HITBOX
    // ==============================

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
// LANE HEIGHT
// ==================================================

void Vehicle::SetLaneHeight(
    int height)
{
    laneHeight = height;

    UpdateHitbox();
}


// ==================================================
// SET WAGON TYPE
// ==================================================

void Vehicle::SetTexture(
    const std::string& id)
{
    textureId = id;

    animationFrames.clear();

    currentFrame = 0;

    lastFrameTime = SDL_GetTicks();


    if (textureId == "wagon1")
    {
        animationFrames =
        {
            "wagon1_01",
            "wagon1_02",
            "wagon1_03"
        };
    }

    else if (textureId == "wagon2")
    {
        animationFrames =
        {
            "wagon2_01",
            "wagon2_02",
            "wagon2_03"
        };
    }

    else if (textureId == "wagon3")
    {
        animationFrames =
        {
            "wagon3_01",
            "wagon3_02",
            "wagon3_03"
        };
    }

    else if (textureId == "wagon4")
    {
        animationFrames =
        {
            "wagon4_01",
            "wagon4_02",
            "wagon4_03"
        };
    }
}


// ==================================================
// SPRITE SIZE
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

void Vehicle::UpdateHitbox()
{
    constexpr float SIDE_REDUCTION =
        0.05f;


    int sideOffset =
        static_cast<int>(
            rect.w * SIDE_REDUCTION);


    hitbox.x =
        rect.x + sideOffset;


    hitbox.w =
        rect.w - (sideOffset * 2);


    int spriteBottom =
        rect.y + rect.h;


    // Giữ nguyên công thức hitbox hiện tại
    hitbox.h =
        laneHeight;


    hitbox.y =
        spriteBottom - hitbox.h;


    hitbox.h =
        laneHeight / 2;


    if (hitbox.w < 0)
    {
        hitbox.w = 0;
    }


    if (hitbox.h < 0)
    {
        hitbox.h = 0;
    }
}