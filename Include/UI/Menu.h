#pragma once

#include <SDL.h>
#include <string>
#include <vector>

class Menu
{
public:
    Menu();

    void MoveUp();
    void MoveDown();

    int GetSelectedIndex() const;

    void Draw(SDL_Renderer* renderer);

private:
    std::vector<std::string> items;
    int selectedIndex;
};