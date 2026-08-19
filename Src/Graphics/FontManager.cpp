#include "Graphics/FontManager.h"

#include <iostream>

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
    Destroy();
}

bool FontManager::LoadFont(
    const std::string& id,
    const std::string& filePath,
    int fontSize)
{
    if (fonts.find(id) != fonts.end())
    {
        return true;
    }

    TTF_Font* font = TTF_OpenFont(
        filePath.c_str(),
        fontSize);

    if (font == nullptr)
    {
        std::cout
            << "Cannot load font: "
            << filePath
            << std::endl;

        std::cout
            << TTF_GetError()
            << std::endl;

        return false;
    }

    fonts[id] = font;

    return true;
}

TTF_Font* FontManager::GetFont(
    const std::string& id) const
{
    auto it = fonts.find(id);

    if (it == fonts.end())
    {
        return nullptr;
    }

    return it->second;
}

void FontManager::Destroy()
{
    for (auto& pair : fonts)
    {
        if (pair.second != nullptr)
        {
            TTF_CloseFont(pair.second);
        }
    }

    fonts.clear();
}