#pragma once

#include <string>
#include <unordered_map>

#include <SDL.h>

#include "Graphics/Texture.h"

class TextureManager
{
public:
    TextureManager();

    ~TextureManager();

    bool LoadTexture(
        SDL_Renderer* renderer,
        const std::string& id,
        const std::string& filePath);

    Texture* GetTexture(const std::string& id);

    void Destroy();

private:
    std::unordered_map<std::string, Texture> textures;
};