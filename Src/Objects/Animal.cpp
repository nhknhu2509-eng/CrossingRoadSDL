#include "Objects/Animal.h"

#include "Config/GameConfig.h"


// ==================================================
// CONSTRUCTOR
// ==================================================

Animal::Animal()
{
    // Giá trị mặc định hiện tại của Animal
    rect.x = 0;
    rect.y = 0;

    rect.w =
        Config::SQUIRREL_WIDTH;

    rect.h =
        Config::SQUIRREL_HEIGHT;


    speed =
        Config::ANIMAL_SPEED;

    direction = 1;


    // Giữ nguyên hitbox margin mặc định
    hitboxLeft = 5;

    hitboxTop = 8;

    hitboxRight = 6;

    hitboxBottom = 8;


    // Giữ nguyên tốc độ animation Animal
    SetFrameDuration(
        140);


    // Giữ nguyên fallback màu xanh
    SetFallbackColor(
        {
            0,
            255,
            0,
            255
        });


    // Giữ nguyên debug hitbox màu xanh
    SetHitboxDebugColor(
        {
            0,
            255,
            0,
            255
        });


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
    hitboxLeft =
        left;

    hitboxTop =
        top;

    hitboxRight =
        right;

    hitboxBottom =
        bottom;


    UpdateHitbox();
}


// ==================================================
// LANE HEIGHT
// ==================================================

void Animal::SetLaneHeight(
    int height)
{
    // Animal bình thường không dùng.
    // Deer override.
    (void)height;
}


// ==================================================
// UPDATE HITBOX
// ==================================================

void Animal::UpdateHitbox()
{
    hitbox.x =
        rect.x +
        hitboxLeft;


    hitbox.y =
        rect.y +
        hitboxTop;


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