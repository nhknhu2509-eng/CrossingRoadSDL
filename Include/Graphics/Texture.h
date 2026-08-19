#pragma once

#include <SDL.h>

class Texture
{
public:
    Texture();

    Texture(SDL_Texture* texture);

    void SetTexture(SDL_Texture* texture);

    SDL_Texture* GetTexture() const;

    int GetWidth() const;
    int GetHeight() const;

private:
    SDL_Texture* texture;

    int width;
    int height;
};