#pragma once

#include <string>
#include <unordered_map>

#include <SDL_ttf.h>

class FontManager
{
public:
    FontManager();
    ~FontManager();

    bool LoadFont(
        const std::string& id,
        const std::string& filePath,
        int fontSize);

    TTF_Font* GetFont(
        const std::string& id) const;

    void Destroy();

private:
    std::unordered_map<std::string, TTF_Font*> fonts;
};