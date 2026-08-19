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

    constexpr int PLAYER_WIDTH = 40;
    constexpr int PLAYER_HEIGHT = 40;
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
    //
    // Ảnh gốc của Deer / Rabbit /
    // Squirrel đều là 512x512.
    //
    // Giữ tỉ lệ 1:1 khi render.
    //
    // Lane height = 82
    //
    // => Animal = 82x82
    //

    constexpr int ANIMAL_WIDTH = 82;
    constexpr int ANIMAL_HEIGHT = 82;

    constexpr int ANIMAL_SPEED = 3;


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