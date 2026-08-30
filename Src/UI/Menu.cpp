#include "UI/Menu.h"

#include "Managers/InputManager.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

#include "Config/GameConfig.h"


// ==================================================
// CONSTRUCTOR
// ==================================================

Menu::Menu()
{
    selectedIndex = 0;
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
            MENU_ITEM_COUNT - 1;
    }
}


// ==================================================
// MOVE DOWN
// ==================================================

void Menu::MoveDown()
{
    selectedIndex++;


    if (selectedIndex >=
        MENU_ITEM_COUNT)
    {
        selectedIndex = 0;
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
// DRAW TEXTURE
// ==================================================

void Menu::DrawTexture(
    SDL_Renderer* renderer,
    TextureManager& textureManager,
    const char* textureId,
    int x,
    int y,
    int width,
    int height)
{
    Texture* texture =
        textureManager.GetTexture(
            textureId);


    if (
        texture == nullptr ||
        texture->GetTexture() == nullptr)
    {
        return;
    }


    SDL_Rect destination =
    {
        x,
        y,
        width,
        height
    };


    SDL_RenderCopy(
        renderer,
        texture->GetTexture(),
        nullptr,
        &destination);
}


// ==================================================
// DRAW
// ==================================================

void Menu::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    // ==========================================
    // MENU BACKGROUND
    // ==========================================

    DrawTexture(
        renderer,
        textureManager,
        "menu_background",
        0,
        0,
        Config::WINDOW_WIDTH,
        Config::WINDOW_HEIGHT);


    // ==========================================
    // LOGO
    // ==========================================

    const int logoX = 70;
    const int logoY = 30;

    const int logoWidth = 500;
    const int logoHeight = 120;


    DrawTexture(
        renderer,
        textureManager,
        "menu_logo",
        logoX,
        logoY,
        logoWidth,
        logoHeight);


    // ==========================================
    // BUTTON CONFIGURATION
    // ==========================================

    const int buttonX = 100;

    const int buttonWidth = 380;
    const int buttonHeight = 60;

    const int startY = 220;

    const int spacing = 70;


    // ==========================================
    // START GAME
    // ==========================================

    DrawTexture(
        renderer,
        textureManager,
        selectedIndex == 0
        ? "startgame_choose"
        : "startgame_normal",
        buttonX,
        startY,
        buttonWidth,
        buttonHeight);


    // ==========================================
    // LOAD GAME
    // ==========================================

    DrawTexture(
        renderer,
        textureManager,
        selectedIndex == 1
        ? "loadgame_choose"
        : "loadgame_normal",
        buttonX,
        startY + spacing,
        buttonWidth,
        buttonHeight);
}
