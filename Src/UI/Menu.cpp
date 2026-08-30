#include "UI/Menu.h"

#include "Managers/InputManager.h"

#include "Graphics/TextRenderer.h"
#include "Graphics/FontManager.h"

#include "Config/GameConfig.h"


// ==================================================
// CONSTRUCTOR
// ==================================================

Menu::Menu()
{
    items =
    {
        "NEW GAME",
        "LOAD GAME",
        "LEADERBOARD",
        "SETTINGS",
        "EXIT"
    };


    selectedIndex =
        0;
}


// ==================================================
// UPDATE
// ==================================================

void Menu::Update()
{
    // ==========================================
    // MOVE UP
    // ==========================================

    if (
        InputManager::IsKeyPressed(
            SDL_SCANCODE_UP) ||
        InputManager::IsKeyPressed(
            SDL_SCANCODE_W))
    {
        MoveUp();
    }


    // ==========================================
    // MOVE DOWN
    // ==========================================

    if (
        InputManager::IsKeyPressed(
            SDL_SCANCODE_DOWN) ||
        InputManager::IsKeyPressed(
            SDL_SCANCODE_S))
    {
        MoveDown();
    }
}


// ==================================================
// MOVE UP
// ==================================================

void Menu::MoveUp()
{
    selectedIndex--;


    if (selectedIndex < 0)
    {
        selectedIndex =
            static_cast<int>(
                items.size()) - 1;
    }
}


// ==================================================
// MOVE DOWN
// ==================================================

void Menu::MoveDown()
{
    selectedIndex++;


    if (
        selectedIndex >=
        static_cast<int>(
            items.size()))
    {
        selectedIndex =
            0;
    }
}


// ==================================================
// GET SELECTED INDEX
// ==================================================

int Menu::GetSelectedIndex() const
{
    return selectedIndex;
}


// ==================================================
// DRAW
// ==================================================

void Menu::Draw(
    SDL_Renderer* renderer,
    TextRenderer& textRenderer,
    FontManager& fontManager)
{
    TTF_Font* font =
        fontManager.GetFont(
            "default");


    if (font == nullptr)
    {
        return;
    }


    // ==========================================
    // DARK OVERLAY
    // ==========================================

    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_BLEND);


    SDL_SetRenderDrawColor(
        renderer,
        0,
        0,
        0,
        150);


    SDL_Rect overlay =
    {
        0,
        0,
        Config::WINDOW_WIDTH,
        Config::WINDOW_HEIGHT
    };


    SDL_RenderFillRect(
        renderer,
        &overlay);


    // ==========================================
    // TITLE
    // ==========================================

    textRenderer.Draw(
        renderer,
        font,
        "CROSSING ROAD",
        500,
        130,
        { 255, 255, 255, 255 });


    // ==========================================
    // MENU ITEMS
    // ==========================================

    const int startX =
        520;

    const int startY =
        250;

    const int spacing =
        55;


    for (
        int i = 0;
        i < static_cast<int>(
            items.size());
        i++)
    {
        SDL_Color color;


        std::string text =
            items[i];


        if (i ==
            selectedIndex)
        {
            color =
            {
                255,
                220,
                120,
                255
            };


            text =
                "> " + text;
        }
        else
        {
            color =
            {
                230,
                230,
                230,
                255
            };


            text =
                "  " + text;
        }


        textRenderer.Draw(
            renderer,
            font,
            text,
            startX,
            startY +
            i * spacing,
            color);
    }


    // ==========================================
    // CONTROL HINT
    // ==========================================

    textRenderer.Draw(
        renderer,
        font,
        "W/S or Arrow Keys - ENTER to select",
        390,
        590,
        { 200, 200, 200, 255 });
}