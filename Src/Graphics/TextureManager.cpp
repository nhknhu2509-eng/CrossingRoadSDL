#include "Graphics/TextureManager.h"

#include <SDL_image.h>

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    Destroy();
}

bool TextureManager::LoadTexture(
    SDL_Renderer* renderer,
    const std::string& id,
    const std::string& filePath)
{
    SDL_Texture* texture =
        IMG_LoadTexture(renderer, filePath.c_str());

    if (texture == nullptr)
    {
        SDL_Log(
            "Failed to load texture %s : %s",
            filePath.c_str(),
            IMG_GetError());

        return false;
    }

    textures[id] = Texture(texture);

    return true;
}

Texture* TextureManager::GetTexture(const std::string& id)
{
    auto it = textures.find(id);

    if (it == textures.end())
    {
        return nullptr;
    }

    return &(it->second);
}

void TextureManager::Destroy()
{
    for (auto& pair : textures)
    {
        if (pair.second.GetTexture() != nullptr)
        {
            SDL_DestroyTexture(pair.second.GetTexture());
        }
    }

    textures.clear();
}