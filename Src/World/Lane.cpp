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
            ============================
            CĂN SPRITE THEO LANE
            ============================

            Sprite cao hơn lane.

            VEHICLE_HEIGHT = 150
            LANE_HEIGHT    = 82

            Phần chiều cao dư:

            150 - 82 = 68

            Không dùng toàn bộ 68 px
            để đẩy sprite lên nữa.

            Chỉ dùng 30% phần dư.
        */

        constexpr float SPRITE_VERTICAL_RATIO = 0.30f;

        int spriteOffset =
            static_cast<int>(
                (Config::VEHICLE_HEIGHT - height)
                * SPRITE_VERTICAL_RATIO);

        int vehicleY =
            y - spriteOffset;

        vehicle.SetPosition(
            i * spacing,
            vehicleY);

        /*
            Hitbox phải biết lane thật sự
            nằm ở đâu.
        */
        vehicle.SetLaneY(y);

        vehicle.SetLaneHeight(height);

        vehicle.SetSpeed(speed);

        vehicle.SetDirection(direction);

        // ============================
        // PHÂN BỐ CÁC LOẠI XE
        // ============================

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