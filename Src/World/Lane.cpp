#include "World/Lane.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"

Lane::Lane(
    int y,
    int height,
    int direction,
    int speed,
    int vehicleCount)
    : trafficLight(760, y)
{
    int spacing = 250;

    for (int i = 0; i < vehicleCount; i++)
    {
        Vehicle vehicle;

        /*
            y đại diện cho vị trí của LANE.

            Sprite cao hơn lane:
                VEHICLE_HEIGHT = 150
                lane height    = 82

            Phần chênh lệch:
                150 - 82 = 68

            Vì hitbox nằm ở đáy sprite,
            đưa sprite lên 68 px để hitbox
            nằm đúng trong lane.
        */
        int vehicleY =
            y - (Config::VEHICLE_HEIGHT - height);

        vehicle.SetPosition(
            i * spacing,
            vehicleY);

        vehicle.SetLaneHeight(height);

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