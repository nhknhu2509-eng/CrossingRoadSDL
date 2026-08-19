#pragma once

#include "Core/Game.h"
#include "Graphics/FontManager.h"
#include "Graphics/TextRenderer.h"
#include "Graphics/Window.h"
#include "Utils/Timer.h"
#include "Graphics/TextureManager.h"

class Application
{
public:
    bool Initialize();
    void Run();
    void Shutdown();

private:
    Window window;

    Timer timer;

    Game game;

    FontManager fontManager;

    TextRenderer textRenderer;
   
    TextureManager textureManager;


};