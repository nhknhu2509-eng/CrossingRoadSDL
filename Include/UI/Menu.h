#pragma once

#include <SDL.h>


class TextureManager;


class Menu
{
public:

    Menu();


    // ==========================================
    // INPUT
    // ==========================================

    void Update();

    void MoveUp();

    void MoveDown();


    // ==========================================
    // SELECTION
    // ==========================================

    int GetSelectedIndex() const;


    // ==========================================
    // DRAW
    // ==========================================

    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);


private:

    // ==========================================
    // MENU INFORMATION
    // ==========================================

    static const int MENU_ITEM_COUNT = 2;

    int selectedIndex;


    // ==========================================
    // DRAW HELPERS
    // ==========================================

    void DrawTexture(
        SDL_Renderer* renderer,
        TextureManager& textureManager,
        const char* textureId,
        int x,
        int y,
        int width,
        int height);
};
