#include "World/Lane.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"


Lane::Lane(
    int y,
    int height,
    int direction,
    int speed,
    int vehicleCount,
    const std::string& animalTexture)
    : trafficLight(760, y)
{
    int spacing = 250;


    // ==================================================
    // ANIMAL LANE
    // ==================================================

    if (!animalTexture.empty())
    {
        for (int i = 0;
            i < vehicleCount;
            i++)
        {
            Animal animal;


            // ==========================================
            // Animal nằm gọn trong lane
            // ==========================================
            //
            // Lane:
            //
            // y
            // ↓
            // ┌──────────────────────────┐
            // │          ANIMAL          │
            // └──────────────────────────┘
            //
            // Animal height = lane height
            //
            // ==========================================

            int animalY =
                y + height
                - Config::ANIMAL_HEIGHT;


            animal.SetPosition(
                i * spacing,
                animalY);


            animal.SetSpeed(speed);


            animal.SetDirection(direction);


            animal.SetTexture(
                animalTexture);


            // ==========================================
            // HITBOX
            // ==========================================
            //
            // Animal nằm hoàn toàn trong lane.
            //
            // Không bỏ phần trên vì góc nhìn
            // như Vehicle.
            //
            // Chỉ bỏ phần transparent
            // ở 4 phía.
            //
            // ==========================================

            animal.SetHitboxMargins(
                8,      // left
                8,      // top
                8,      // right
                8);     // bottom


            animals.push_back(animal);
        }


        return;
    }


    // ==================================================
    // VEHICLE LANE
    // ==================================================

    for (int i = 0;
        i < vehicleCount;
        i++)
    {
        Vehicle vehicle;


        // ==========================================
        // CĂN SPRITE XE
        // ==========================================

        int vehicleY =
            y + height
            - Config::VEHICLE_HEIGHT;


        vehicle.SetPosition(
            i * spacing,
            vehicleY);


        // Giữ nguyên cách hitbox
        // của Vehicle hiện tại.

        vehicle.SetLaneHeight(height);


        vehicle.SetSpeed(speed);


        vehicle.SetDirection(direction);


        // ==========================================
        // TEXTURE XE
        // ==========================================

        switch (i % 4)
        {
        case 0:

            vehicle.SetTexture(
                "wagon_01");

            break;


        case 1:

            vehicle.SetTexture(
                "wagon_02");

            break;


        case 2:

            vehicle.SetTexture(
                "wagon_03");

            break;


        case 3:

            vehicle.SetTexture(
                "wagon_04");

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
        // ==============================
        // VEHICLE
        // ==============================

        for (Vehicle& vehicle : vehicles)
        {
            vehicle.Update();
        }


        // ==============================
        // ANIMAL
        // ==============================

        for (Animal& animal : animals)
        {
            animal.Update();
        }
    }
}


void Lane::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    // ==============================
    // VEHICLE
    // ==============================

    for (Vehicle& vehicle : vehicles)
    {
        vehicle.Draw(
            renderer,
            textureManager);
    }


    // ==============================
    // ANIMAL
    // ==============================

    for (Animal& animal : animals)
    {
        animal.Draw(
            renderer,
            textureManager);
    }


    // ==============================
    // TRAFFIC LIGHT
    // ==============================

    trafficLight.Draw(renderer);
}


const std::vector<Vehicle>&
Lane::GetVehicles() const
{
    return vehicles;
}


const std::vector<Animal>&
Lane::GetAnimals() const
{
    return animals;
}