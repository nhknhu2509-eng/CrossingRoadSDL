#include "Objects/Vehicle.h"

#include "Config/GameConfig.h"


// ==================================================
// CONSTRUCTOR
// ==================================================

Vehicle::Vehicle()
{
    rect.x = 0;
    rect.y = 0;


    rect.w =
        Config::VEHICLE_WIDTH;

    rect.h =
        Config::VEHICLE_HEIGHT;


    speed =
        Config::VEHICLE_SPEED;

    direction = 1;


    laneHeight =
        Config::LANE_HEIGHT;


    textureId =
        "wagon1";


    SetAnimationFrames(
        {
            "wagon1_01",
            "wagon1_02",
            "wagon1_03"
        });


    // Giữ nguyên 130 ms/frame
    SetFrameDuration(
        130);


    // Giữ nguyên fallback Vehicle
    SetFallbackColor(
        Config::VEHICLE_COLOR);


    // Giữ nguyên debug hitbox màu đỏ
    SetHitboxDebugColor(
        {
            255,
            0,
            0,
            255
        });


    UpdateHitbox();
}


// ==================================================
// LANE HEIGHT
// ==================================================

void Vehicle::SetLaneHeight(
    int height)
{
    laneHeight =
        height;


    UpdateHitbox();
}


// ==================================================
// WAGON TYPE
// ==================================================

void Vehicle::SetTexture(
    const std::string& id)
{
    textureId =
        id;


    if (textureId == "wagon1")
    {
        SetAnimationFrames(
            {
                "wagon1_01",
                "wagon1_02",
                "wagon1_03"
            });
    }

    else if (textureId == "wagon2")
    {
        SetAnimationFrames(
            {
                "wagon2_01",
                "wagon2_02",
                "wagon2_03"
            });
    }

    else if (textureId == "wagon3")
    {
        SetAnimationFrames(
            {
                "wagon3_01",
                "wagon3_02",
                "wagon3_03"
            });
    }

    else if (textureId == "wagon4")
    {
        SetAnimationFrames(
            {
                "wagon4_01",
                "wagon4_02",
                "wagon4_03"
            });
    }
}


// ==================================================
// SPRITE SIZE
// ==================================================

void Vehicle::SetSpriteSize(
    int width,
    int height)
{
    rect.w =
        width;

    rect.h =
        height;


    UpdateHitbox();
}


// ==================================================
// UPDATE HITBOX
// ==================================================

void Vehicle::UpdateHitbox()
{
    // ==========================================
    // GIỮ NGUYÊN HITBOX WAGON HIỆN TẠI
    // ==========================================

    constexpr float SIDE_REDUCTION =
        0.05f;


    int sideOffset =
        static_cast<int>(
            rect.w *
            SIDE_REDUCTION);


    hitbox.x =
        rect.x +
        sideOffset;


    hitbox.w =
        rect.w -
        (sideOffset * 2);


    int spriteBottom =
        rect.y +
        rect.h;


    // Không thay đổi thứ tự
    hitbox.h =
        laneHeight;


    hitbox.y =
        spriteBottom -
        hitbox.h;


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