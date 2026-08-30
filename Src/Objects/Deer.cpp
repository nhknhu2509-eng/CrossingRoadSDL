#include "Objects/Deer.h"

#include "Config/GameConfig.h"


// ==================================================
// CONSTRUCTOR
// ==================================================

Deer::Deer()
{
    rect.w =
        Config::DEER_WIDTH;

    rect.h =
        Config::DEER_HEIGHT;


    SetAnimationFrames(
        {
            "deer_01",
            "deer_02",
            "deer_03",
            "deer_04",
            "deer_05"
        });


    UpdateHitbox();
}


// ==================================================
// DEER LANE HITBOX
// ==================================================

void Deer::SetLaneHeight(
    int height)
{
    // ==========================================
    // GIỮ NGUYÊN 100% CÔNG THỨC HIỆN TẠI
    // TRONG Lane.cpp
    // ==========================================

    constexpr float SIDE_REDUCTION =
        0.05f;


    int sideOffset =
        static_cast<int>(
            rect.w *
            SIDE_REDUCTION);


    int hitboxHeight =
        height / 2;


    int topMargin =
        rect.h -
        hitboxHeight;


    if (topMargin < 0)
    {
        topMargin = 0;
    }


    SetHitboxMargins(
        sideOffset,
        topMargin,
        sideOffset,
        6);
}