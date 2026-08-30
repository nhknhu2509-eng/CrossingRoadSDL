#include "Objects/Obstacle.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"


// ==================================================
// CONSTRUCTOR
// ==================================================

Obstacle::Obstacle()
{
    hitbox.x = 0;
    hitbox.y = 0;

    hitbox.w = 0;
    hitbox.h = 0;


    speed = 0;

    direction = 1;


    currentFrame = 0;


    lastFrameTime =
        SDL_GetTicks();


    frameDuration = 140;


    fallbackColor =
    {
        255,
        255,
        255,
        255
    };


    hitboxDebugColor =
    {
        255,
        255,
        255,
        255
    };


    // Không hiển thị viền hitbox.
    debugHitbox = false;
}


// ==================================================
// UPDATE
// ==================================================

void Obstacle::Update()
{
    rect.x +=
        speed * direction;


    UpdateAnimation();


    WrapScreen();


    UpdateHitbox();
}


// ==================================================
// DRAW
// ==================================================

void Obstacle::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    if (animationFrames.empty())
    {
        return;
    }


    const std::string&
        currentTextureId =
        animationFrames[currentFrame];


    Texture* texture =
        textureManager.GetTexture(
            currentTextureId);


    if (
        texture != nullptr &&
        texture->GetTexture() != nullptr)
    {
        SDL_RendererFlip flip =
            SDL_FLIP_NONE;


        // ======================================
        // FLIP SPRITE KHI ĐI SANG TRÁI
        // ======================================

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
            fallbackColor.r,
            fallbackColor.g,
            fallbackColor.b,
            fallbackColor.a);


        SDL_RenderFillRect(
            renderer,
            &rect);
    }


    // ==========================================
    // DEBUG HITBOX
    // ==========================================
    //
    // debugHitbox = false nên không xuất hiện
    // đường viền trên màn hình.
    //
    // Hitbox vẫn tồn tại để collision.
    // ==========================================

    if (debugHitbox)
    {
        SDL_SetRenderDrawColor(
            renderer,
            hitboxDebugColor.r,
            hitboxDebugColor.g,
            hitboxDebugColor.b,
            hitboxDebugColor.a);


        SDL_RenderDrawRect(
            renderer,
            &hitbox);
    }
}


// ==================================================
// UPDATE ANIMATION
// ==================================================

void Obstacle::UpdateAnimation()
{
    if (animationFrames.empty())
    {
        return;
    }


    Uint32 now =
        SDL_GetTicks();


    if (
        now - lastFrameTime >=
        frameDuration)
    {
        currentFrame++;


        if (
            currentFrame >=
            static_cast<int>(
                animationFrames.size()))
        {
            currentFrame = 0;
        }


        lastFrameTime =
            now;
    }
}


// ==================================================
// WRAP SCREEN
// ==================================================

void Obstacle::WrapScreen()
{
    if (
        rect.x >
        Config::WINDOW_WIDTH)
    {
        rect.x =
            -rect.w;
    }


    if (
        rect.x + rect.w < 0)
    {
        rect.x =
            Config::WINDOW_WIDTH;
    }
}


// ==================================================
// SET POSITION
// ==================================================

void Obstacle::SetPosition(
    int x,
    int y)
{
    GameObject::SetPosition(
        x,
        y);


    UpdateHitbox();
}


// ==================================================
// SPEED
// ==================================================

void Obstacle::SetSpeed(
    int value)
{
    speed = value;
}


// ==================================================
// DIRECTION
// ==================================================

void Obstacle::SetDirection(
    int value)
{
    direction = value;
}


// ==================================================
// GET HITBOX
// ==================================================

SDL_Rect
Obstacle::GetHitbox() const
{
    return hitbox;
}


// ==================================================
// SET ANIMATION FRAMES
// ==================================================

void Obstacle::SetAnimationFrames(
    const std::vector<std::string>& frames)
{
    animationFrames =
        frames;


    currentFrame = 0;


    lastFrameTime =
        SDL_GetTicks();
}


// ==================================================
// FRAME DURATION
// ==================================================

void Obstacle::SetFrameDuration(
    Uint32 duration)
{
    frameDuration =
        duration;
}


// ==================================================
// FALLBACK COLOR
// ==================================================

void Obstacle::SetFallbackColor(
    SDL_Color color)
{
    fallbackColor =
        color;
}


// ==================================================
// HITBOX DEBUG COLOR
// ==================================================

void Obstacle::SetHitboxDebugColor(
    SDL_Color color)
{
    hitboxDebugColor =
        color;
}