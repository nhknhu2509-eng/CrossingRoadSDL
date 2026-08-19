#include "UI/Menu.h"

Menu::Menu()
{
    items =
    {
        "New Game",
        "Load Game",
        "Leaderboard",
        "Settings",
        "Exit"
    };

    selectedIndex = 0;
}

void Menu::MoveUp()
{
    if (selectedIndex > 0)
        selectedIndex--;
}

void Menu::MoveDown()
{
    if (selectedIndex < (int)items.size() - 1)
        selectedIndex++;
}

int Menu::GetSelectedIndex() const
{
    return selectedIndex;
}

void Menu::Draw(SDL_Renderer* renderer)
{
    // Tạm thời chưa render chữ.
    // Bước tiếp theo sẽ dùng SDL_ttf.
}