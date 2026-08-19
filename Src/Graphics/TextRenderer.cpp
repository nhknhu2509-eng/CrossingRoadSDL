#include "Graphics/TextRenderer.h"

void TextRenderer::Draw(
    SDL_Renderer* renderer,
    TTF_Font* font,
    const std::string& text,
    int x,
    int y,
    SDL_Color color)
{
    if (renderer == nullptr || font == nullptr)
        return;

    SDL_Surface* surface =
        TTF_RenderUTF8_Blended(
            font,
            text.c_str(),
            color);

    if (surface == nullptr)
        return;

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface);

    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_FreeSurface(surface);

    SDL_RenderCopy(
        renderer,
        texture,
        nullptr,
        &rect);

    SDL_DestroyTexture(texture);
}