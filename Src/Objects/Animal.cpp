#include "Objects/Animal.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"


// ==========================================
// DEBUG HITBOX
// ==========================================
//
// true  -> hiện hitbox màu xanh
// false -> ẩn hitbox
//
// Trong giai đoạn căn chỉnh nên để true.
// Sau khi hoàn tất đổi thành false.
//

constexpr bool DEBUG_ANIMAL_HITBOX = true;


Animal::Animal()
{
    // ==============================
    // SPRITE
    // ==============================

    rect.x = 0;
    rect.y = 0;

    rect.w = Config::ANIMAL_WIDTH;
    rect.h = Config::ANIMAL_HEIGHT;


    // ==============================
    // HITBOX
    // ==============================

    hitbox.x = 0;
    hitbox.y = 0;
    hitbox.w = 0;
    hitbox.h = 0;


    // ==============================
    // MOVEMENT
    // ==============================

    speed = Config::ANIMAL_SPEED;

    direction = 1;


    // ==============================
    // TEXTURE
    // ==============================

    textureId = "deer";


    // ==============================
    // DEFAULT HITBOX
    // ==============================
    //
    // Giá trị mặc định.
    // Lane sẽ thay bằng giá trị
    // chính xác cho từng animal.
    //

    hitboxLeft = 5;
    hitboxTop = 8;
    hitboxRight = 6;
    hitboxBottom = 8;


    UpdateHitbox();
}


void Animal::Update()
{
    // ==============================
    // DI CHUYỂN
    // ==============================

    rect.x += speed * direction;


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


    // ==============================
    // UPDATE HITBOX
    // ==============================

    UpdateHitbox();
}


void Animal::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    Texture* texture =
        textureManager.GetTexture(textureId);


    // ==============================
    // DRAW SPRITE
    // ==============================

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
        // ==============================
        // DEBUG TEXTURE ERROR
        // ==============================

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


void Animal::SetSpeed(int value)
{
    speed = value;
}


void Animal::SetDirection(int value)
{
    direction = value;
}


void Animal::SetPosition(
    int x,
    int y)
{
    rect.x = x;
    rect.y = y;

    UpdateHitbox();
}


void Animal::SetTexture(
    const std::string& textureId)
{
    this->textureId = textureId;
}


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


SDL_Rect Animal::GetRect() const
{
    return rect;
}


SDL_Rect Animal::GetHitbox() const
{
    return hitbox;
}


void Animal::UpdateHitbox()
{
    // ==============================
    // LEFT
    // ==============================

    hitbox.x =
        rect.x + hitboxLeft;


    // ==============================
    // TOP
    // ==============================

    hitbox.y =
        rect.y + hitboxTop;


    // ==============================
    // WIDTH
    // ==============================

    hitbox.w =
        rect.w
        - hitboxLeft
        - hitboxRight;


    // ==============================
    // HEIGHT
    // ==============================

    hitbox.h =
        rect.h
        - hitboxTop
        - hitboxBottom;
}