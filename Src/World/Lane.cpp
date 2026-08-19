#include "World/Lane.h"

Lane::Lane(
    int y,
    int direction,
    int speed,
    int vehicleCount)
    : trafficLight(760, y)
{
    int spacing = 250;

    for (int i = 0; i < vehicleCount; i++)
    {
        Vehicle vehicle;

        vehicle.SetPosition(i * spacing, y);

        vehicle.SetSpeed(speed);

        vehicle.SetDirection(direction);

        vehicles.push_back(vehicle);
    }
}

void Lane::Update()
{
    trafficLight.Update();

    if (trafficLight.CanMove())
    {
        for (Vehicle& vehicle : vehicles)
        {
            vehicle.Update();
        }
    }
}

void Lane::Draw(SDL_Renderer* renderer)
{
    for (Vehicle& vehicle : vehicles)
    {
        vehicle.Draw(renderer);
    }

    trafficLight.Draw(renderer);
}

const std::vector<Vehicle>& Lane::GetVehicles() const
{
    return vehicles;
}