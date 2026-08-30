#pragma once

#include <SDL.h>

#include "Objects/Obstacle.h"


class Animal : public Obstacle
{
public:

    virtual ~Animal() = default;


    // ==========================================
    // HITBOX
    // ==========================================

    void SetHitboxMargins(
        int left,
        int top,
        int right,
        int bottom);


    // Mặc định Animal không cần lane height.
    // Deer sẽ override hàm này.
    virtual void SetLaneHeight(
        int height);


protected:

    Animal();


    void UpdateHitbox() override;


protected:

    int hitboxLeft;

    int hitboxTop;

    int hitboxRight;

    int hitboxBottom;
};