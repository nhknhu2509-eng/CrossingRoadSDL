#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <string>


class TextureManager;


class Player
{
private:

    SDL_Rect rect;

    int speed;


    // ==============================
    // Animation
    // ==============================

    std::string currentTextureId;

    int currentFrame;

    Uint32 lastFrameTime;

    Uint32 frameDuration;

    bool moving;

    bool movingHorizontal;

    bool facingLeft;


public:

    Player();


    void Update();


    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager);


    const SDL_Rect& GetRect() const;


    SDL_Rect GetHitbox() const;


    void Reset();


    void SetPosition(
        int x,
        int y);
};


#endif