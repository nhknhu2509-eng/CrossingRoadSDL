#pragma once

#include "Objects/Animal.h"


class Deer : public Animal
{
public:

    Deer();


    void SetLaneHeight(
        int height) override;
};