#pragma once

#include <string>

#include "Objects/Obstacle.h"


class Vehicle : public Obstacle
{
public:

    Vehicle();


    void SetLaneHeight(
        int height);


    void SetTexture(
        const std::string& textureId);


    void SetSpriteSize(
        int width,
        int height);


protected:

    void UpdateHitbox() override;


private:

    int laneHeight;

    std::string textureId;
};