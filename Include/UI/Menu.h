#pragma once

#include <SDL.h>

#include <string>
#include <vector>


class TextRenderer;
class FontManager;


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
        TextRenderer& textRenderer,
        FontManager& fontManager);


private:

    std::vector<std::string> items;

    int selectedIndex;
};