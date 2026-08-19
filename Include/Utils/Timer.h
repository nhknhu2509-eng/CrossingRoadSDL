#pragma once

class Timer
{
public:
    Timer();

    void Tick();

    float GetDeltaTime() const;

private:
    unsigned int lastTick;
    float deltaTime;
};