#include "Objects/Squirrel.h"

#include "Config/GameConfig.h"


Squirrel::Squirrel()
{
    rect.w =
        Config::SQUIRREL_WIDTH;

    rect.h =
        Config::SQUIRREL_HEIGHT;


    SetAnimationFrames(
        {
            "squirrel_01",
            "squirrel_02",
            "squirrel_03",
            "squirrel_04"
        });


    UpdateHitbox();
}