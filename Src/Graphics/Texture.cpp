#include "Graphics/Texture.h"

Texture::Texture()
{
    texture = nullptr;

    width = 0;
    height = 0;
}

Texture::Texture(SDL_Texture* texture)
{
    SetTexture(texture);
}

void Texture::SetTexture(SDL_Texture* newTexture)
{
    texture = newTexture;

    if (texture != nullptr)
    {
        SDL_QueryTexture(
            texture,
            nullptr,
            nullptr,
            &width,
            &height);
    }
    else
    {
        width = 0;
        height = 0;
    }
}

SDL_Texture* Texture::GetTexture() const
{
    return texture;
}

int Texture::GetWidth() const
{
    return width;
}

int Texture::GetHeight() const
{
    return height;
}