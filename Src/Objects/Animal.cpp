#include "Objects/Animal.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"


constexpr bool DEBUG_ANIMAL_HITBOX = true;


// ==================================================
// CONSTRUCTOR
// ==================================================

Animal::Animal()
{
    rect.x = 0;
    rect.y = 0;

    rect.w = Config::SQUIRREL_WIDTH;
    rect.h = Config::SQUIRREL_HEIGHT;


    hitbox.x = 0;
    hitbox.y = 0;
    hitbox.w = 0;
    hitbox.h = 0;


    speed = Config::ANIMAL_SPEED;
    direction = 1;


    textureId = "deer";


    hitboxLeft = 5;
    hitboxTop = 8;
    hitboxRight = 6;
    hitboxBottom = 8;


    // ==============================
    // Animation
    // ==============================

    currentFrame = 0;

    lastFrameTime = SDL_GetTicks();

    // 140 ms / frame
    // Số càng lớn -> animation càng chậm
    frameDuration = 140;


    animationFrames =
    {
        "deer_01",
        "deer_02",
        "deer_03",
        "deer_04",
        "deer_05"
    };


    UpdateHitbox();
}


// ==================================================
// UPDATE
// ==================================================

void Animal::Update()
{
    // Di chuyển
    rect.x += speed * direction;


    // Animation chỉ chạy khi Animal Update
    UpdateAnimation();


    // ==============================
    // WRAP SCREEN
    // ==============================

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


// ==================================================
// UPDATE ANIMATION
// ==================================================

void Animal::UpdateAnimation()
{
    if (animationFrames.empty())
    {
        return;
    }


    Uint32 now = SDL_GetTicks();


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

void Animal::Draw(
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


        // ==========================================
        // HƯỚNG DI CHUYỂN
        // ==========================================
        //
        // Các sprite gốc được giữ nguyên.
        // Lane đi hướng ngược lại sẽ flip.
        // ==========================================

        if (direction < 0)
        {
            flip = SDL_FLIP_HORIZONTAL;
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
            0,
            255,
            0,
            255);

        SDL_RenderFillRect(
            renderer,
            &rect);
    }


    // ==============================
    // DEBUG HITBOX
    // ==============================

    if (DEBUG_ANIMAL_HITBOX)
    {
        SDL_SetRenderDrawColor(
            renderer,
            0,
            255,
            0,
            255);

        SDL_RenderDrawRect(
            renderer,
            &hitbox);
    }
}


// ==================================================
// SPEED
// ==================================================

void Animal::SetSpeed(int value)
{
    speed = value;
}


// ==================================================
// DIRECTION
// ==================================================

void Animal::SetDirection(int value)
{
    direction = value;
}


// ==================================================
// POSITION
// ==================================================

void Animal::SetPosition(
    int x,
    int y)
{
    rect.x = x;
    rect.y = y;

    UpdateHitbox();
}


// ==================================================
// SET TEXTURE / ANIMAL TYPE
// ==================================================

void Animal::SetTexture(
    const std::string& id)
{
    textureId = id;

    animationFrames.clear();

    currentFrame = 0;

    lastFrameTime = SDL_GetTicks();


    // ==========================================
    // DEER
    // ==========================================

    if (textureId == "deer")
    {
        rect.w = Config::DEER_WIDTH;
        rect.h = Config::DEER_HEIGHT;


        animationFrames =
        {
            "deer_01",
            "deer_02",
            "deer_03",
            "deer_04",
            "deer_05"
        };
    }


    // ==========================================
    // SQUIRREL
    // ==========================================

    else if (textureId == "squirrel")
    {
        rect.w = Config::SQUIRREL_WIDTH;
        rect.h = Config::SQUIRREL_HEIGHT;


        animationFrames =
        {
            "squirrel_01",
            "squirrel_02",
            "squirrel_03",
            "squirrel_04"
        };
    }


    // ==========================================
    // RABBIT
    // ==========================================

    else if (textureId == "rabbit")
    {
        rect.w = Config::RABBIT_WIDTH;
        rect.h = Config::RABBIT_HEIGHT;


        animationFrames =
        {
            "rabbit_01",
            "rabbit_02",
            "rabbit_03"
        };
    }


    UpdateHitbox();
}


// ==================================================
// HITBOX MARGINS
// ==================================================

void Animal::SetHitboxMargins(
    int left,
    int top,
    int right,
    int bottom)
{
    hitboxLeft = left;
    hitboxTop = top;
    hitboxRight = right;
    hitboxBottom = bottom;

    UpdateHitbox();
}


// ==================================================
// GET RECT
// ==================================================

SDL_Rect Animal::GetRect() const
{
    return rect;
}


// ==================================================
// GET HITBOX
// ==================================================

SDL_Rect Animal::GetHitbox() const
{
    return hitbox;
}


// ==================================================
// UPDATE HITBOX
// ==================================================

void Animal::UpdateHitbox()
{
    hitbox.x =
        rect.x + hitboxLeft;


    hitbox.y =
        rect.y + hitboxTop;


    hitbox.w =
        rect.w
        - hitboxLeft
        - hitboxRight;


    hitbox.h =
        rect.h
        - hitboxTop
        - hitboxBottom;


    if (hitbox.w < 0)
    {
        hitbox.w = 0;
    }


    if (hitbox.h < 0)
    {
        hitbox.h = 0;
    }
}