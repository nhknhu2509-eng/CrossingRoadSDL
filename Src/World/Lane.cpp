#include "World/Lane.h"
#include "Graphics/TextureManager.h"

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

        vehicle.SetPosition(
            i * spacing,
            y);

        vehicle.SetSpeed(speed);

        vehicle.SetDirection(direction);

        // Phân bố nhiều loại xe
        switch (i % 4)
        {
        case 0:
            vehicle.SetTexture("wagon_01");
            break;

        case 1:
            vehicle.SetTexture("wagon_02");
            break;

        case 2:
            vehicle.SetTexture("wagon_03");
            break;

        case 3:
            vehicle.SetTexture("wagon_04");
            break;
        }

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

void Lane::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    for (Vehicle& vehicle : vehicles)
    {
        vehicle.Draw(
            renderer,
            textureManager);
    }

    trafficLight.Draw(renderer);
}

const std::vector<Vehicle>& Lane::GetVehicles() const
{
    return vehicles;
}