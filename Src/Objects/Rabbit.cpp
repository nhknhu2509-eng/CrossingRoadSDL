#include "Objects/Rabbit.h"

#include "Config/GameConfig.h"


Rabbit::Rabbit()
{
    rect.w =
        Config::RABBIT_WIDTH;

    rect.h =
        Config::RABBIT_HEIGHT;


    SetAnimationFrames(
        {
            "rabbit_01",
            "rabbit_02",
            "rabbit_03"
        });


    UpdateHitbox();
}