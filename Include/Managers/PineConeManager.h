#pragma once

#include <vector>

#include <SDL.h>

#include "Objects/PineCone.h"
#include "Graphics/TextureManager.h"


class PineConeManager
{
public:

    PineConeManager()
    {
        textureLoadAttempted =
            false;


        // ==========================================
        // PINE CONE POSITIONS
        // ==========================================

        pineCones.emplace_back(
            160,
            615,
            100);


        pineCones.emplace_back(
            1080,
            540,
            100);


        pineCones.emplace_back(
            170,
            455,
            100);


        pineCones.emplace_back(
            1090,
            365,
            100);


        pineCones.emplace_back(
            180,
            285,
            100);


        pineCones.emplace_back(
            1030,
            205,
            100);
    }


    // ==========================================
    // CHECK COLLECTION
    // ==========================================

    int CheckCollection(
        const SDL_Rect& playerHitbox)
    {
        int addedScore =
            0;


        for (
            PineCone& pineCone :
            pineCones)
        {
            if (
                pineCone.CheckCollected(
                    playerHitbox))
            {
                addedScore +=
                    pineCone.GetValue();
            }
        }


        return addedScore;
    }


    // ==========================================
    // DRAW
    // ==========================================

    void Draw(
        SDL_Renderer* renderer,
        TextureManager& textureManager)
    {
        if (!textureLoadAttempted)
        {
            textureLoadAttempted =
                true;


            if (
                textureManager.GetTexture(
                    "pinecone") == nullptr)
            {
                textureManager.LoadTexture(
                    renderer,
                    "pinecone",
                    "Assets/Images/Collectibles/pinecone.png");
            }
        }


        for (
            PineCone& pineCone :
            pineCones)
        {
            pineCone.Draw(
                renderer,
                textureManager);
        }
    }


    // ==========================================
    // RESET
    // ==========================================

    void Reset()
    {
        for (
            PineCone& pineCone :
            pineCones)
        {
            pineCone.Reset();
        }
    }


    // ==========================================
    // GET COLLECTED STATES
    // ==========================================

    std::vector<bool>
        GetCollectedStates() const
    {
        std::vector<bool>
            states;


        for (
            const PineCone& pineCone :
            pineCones)
        {
            states.push_back(
                pineCone.IsCollected());
        }


        return states;
    }


    // ==========================================
    // RESTORE COLLECTED STATES
    // ==========================================

    void SetCollectedStates(
        const std::vector<bool>& states)
    {
        size_t count =
            states.size();

 
        if (
            count >
            pineCones.size())
        {
            count =
                pineCones.size();
        }


        for (
            size_t i = 0;
            i < count;
            i++)
        {
            pineCones[i].SetCollected(
                states[i]);
        }
    }


private:

    std::vector<PineCone>
        pineCones;


    bool textureLoadAttempted;
};