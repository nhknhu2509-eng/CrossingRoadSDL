#pragma once

#include <SDL.h>

namespace Config
{
    // ==============================
    // Window
    // ==============================

    constexpr int WINDOW_WIDTH = 1280;
    constexpr int WINDOW_HEIGHT = 720;
    constexpr int TARGET_FPS = 60;


    // ==============================
    // Player
    // ==============================

    constexpr int PLAYER_WIDTH = 70;
    constexpr int PLAYER_HEIGHT = 55;
    constexpr int PLAYER_SPEED = 5;

    constexpr int PLAYER_START_X =
        WINDOW_WIDTH / 2
        - PLAYER_WIDTH / 2;

    constexpr int PLAYER_START_Y =
        WINDOW_HEIGHT - 80;


    // ==============================
    // Vehicle
    // ==============================

    constexpr int VEHICLE_WIDTH = 170;
    constexpr int VEHICLE_HEIGHT = 150;
    constexpr int VEHICLE_SPEED = 3;


    // ==============================
    // Animal
    // ==============================

    constexpr int ANIMAL_SPEED = 3;


    // ==============================
    // Individual Animal Sprite Size
    // ==============================
    //
    // Deer:
    // Lane 3 - Vehicle + deer.png
    //
    // Squirrel:
    // Lane 4 - Animal + squirrel.png
    //
    // Rabbit:
    // Lane 5 - Animal + rabbit.png
    //

    constexpr int DEER_WIDTH = 120;
    constexpr int DEER_HEIGHT = 100;

    constexpr int SQUIRREL_WIDTH = 55;
    constexpr int SQUIRREL_HEIGHT = 55;

    constexpr int RABBIT_WIDTH = 55;
    constexpr int RABBIT_HEIGHT = 55;


    // ==============================
    // Deer Hitbox
    // ==============================
    //
    // Deer vẫn dùng Vehicle
    // nên hitbox có tính chất giống wagon:
    //
    // - Không lấy toàn bộ sprite
    // - Chỉ lấy phần dưới
    //
    // Giá trị này có thể chỉnh trực tiếp
    // nếu muốn hitbox deer cao/thấp hơn.
    //

    constexpr int DEER_HITBOX_BOTTOM_HEIGHT = 35;


    // ==============================
    // Lane
    // ==============================

    constexpr int LANE_HEIGHT = 82;


    // ==============================
    // Colors
    // ==============================

    constexpr SDL_Color PLAYER_COLOR =
    {
        255,
        0,
        0,
        255
    };


    constexpr SDL_Color VEHICLE_COLOR =
    {
        255,
        255,
        0,
        255
    };


    constexpr SDL_Color BACKGROUND_COLOR =
    {
        0,
        120,
        215,
        255
    };
}