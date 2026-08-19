#pragma once

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

class TextRenderer
{
public:
    void Draw(
        SDL_Renderer* renderer,
        TTF_Font* font,
        const std::string& text,
        int x,
        int y,
        SDL_Color color = { 255,255,255,255 });
};