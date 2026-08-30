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
        //
        // Có thể thay đổi các tọa độ này sau.
        //
        // Mỗi quả thông = 100 điểm.
        //
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
    //
    // Trả về số điểm vừa nhặt được.
    //
    // Nếu không nhặt được gì:
    // return 0.
    //
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
        // ======================================
        // LOAD TEXTURE
        //
        // Chỉ thử load 1 lần.
        // Không cần sửa Application.cpp.
        // ======================================

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


        // ======================================
        // DRAW ALL AVAILABLE PINE CONES
        // ======================================

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


private:

    std::vector<PineCone>
        pineCones;


    bool textureLoadAttempted;
};