#pragma once

#include <SDL.h>

#include "Graphics/TextureManager.h"


class PineCone
{
public:

    PineCone(
        int x,
        int y,
        int pointValue = 100)
    {
        rect =
        {
            x,
            y,
            44,
            44
        };


        value =
            pointValue;


        collected =
            false;
    }


    // ==========================================
    // DRAW
    // ==========================================

    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager)
    {
        if (collected)
        {
            return;
        }


        Texture* texture =
            textureManager.GetTexture(
                "pinecone");


        if (
            texture != nullptr &&
            texture->GetTexture() != nullptr)
        {
            SDL_RenderCopy(
                renderer,
                texture->GetTexture(),
                nullptr,
                &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(
                renderer,
                130,
                80,
                40,
                255);


            SDL_RenderFillRect(
                renderer,
                &rect);
        }
    }


    // ==========================================
    // CHECK COLLECTION
    // ==========================================

    bool CheckCollected(
        const SDL_Rect& playerHitbox)
    {
        if (collected)
        {
            return false;
        }


        SDL_Rect pineConeHitbox =
            GetHitbox();


        if (
            SDL_HasIntersection(
                &playerHitbox,
                &pineConeHitbox))
        {
            collected =
                true;


            return true;
        }


        return false;
    }


    // ==========================================
    // HITBOX
    // ==========================================

    SDL_Rect GetHitbox() const
    {
        SDL_Rect hitbox;


        hitbox.x =
            rect.x + 6;

        hitbox.y =
            rect.y + 6;

        hitbox.w =
            rect.w - 12;

        hitbox.h =
            rect.h - 12;


        return hitbox;
    }


    // ==========================================
    // VALUE
    // ==========================================

    int GetValue() const
    {
        return value;
    }


    // ==========================================
    // RESET
    // ==========================================

    void Reset()
    {
        collected =
            false;
    }


    // ==========================================
    // COLLECTED
    // ==========================================

    bool IsCollected() const
    {
        return collected;
    }


    // ==========================================
    // RESTORE COLLECTED STATE
    // ==========================================

    void SetCollected(
        bool value)
    {
        collected =
            value;
    }


private:

    SDL_Rect rect;


    int value;


    bool collected;
};