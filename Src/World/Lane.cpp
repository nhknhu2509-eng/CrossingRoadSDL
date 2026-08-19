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


        // ====================================================
        // CĂN SPRITE VỚI LANE
        // ====================================================
        //
        // Lane:
        //
        //     y
        //     ↓
        //     ┌─────────────────────┐
        //     │                     │
        //     │        LANE         │  height = 82
        //     │                     │
        //     └─────────────────────┘
        //                         ↑
        //                     lane bottom
        //
        // Sprite cao 150 px.
        //
        // Ta đặt cạnh dưới sprite trùng
        // cạnh dưới lane.
        //
        // spriteBottom = y + height
        //
        // => spriteY + VEHICLE_HEIGHT
        //        = y + height
        //
        // => spriteY
        //        = y + height - VEHICLE_HEIGHT
        //
        // ====================================================

        int vehicleY =
            y + height - Config::VEHICLE_HEIGHT;


        vehicle.SetPosition(
            i * spacing,
            vehicleY);


        // Hitbox cao bằng lane
        vehicle.SetLaneHeight(height);


        vehicle.SetSpeed(speed);

        vehicle.SetDirection(direction);


        // ====================================================
        // PHÂN BỐ CÁC LOẠI XE
        // ====================================================

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

        default:
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