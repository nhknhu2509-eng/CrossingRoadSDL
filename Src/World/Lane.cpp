#include "World/Lane.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"


Lane::Lane(
    int y,
    int height,
    int direction,
    int speed,
    int vehicleCount,
    const std::string& vehicleTexture,
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


            // ==============================
            // TEXTURE
            // ==============================

            animal.SetTexture(
                animalTexture);


            // ==============================
            // POSITION
            // ==============================

            int animalY =
                y + height
                - animal.GetRect().h;


            animal.SetPosition(
                i * spacing,
                animalY);


            // ==============================
            // MOVEMENT
            // ==============================

            animal.SetSpeed(speed);

            animal.SetDirection(direction);


            // ==============================
            // HITBOX
            // ==============================

            if (animalTexture == "squirrel")
            {
                animal.SetHitboxMargins(
                    5,
                    8,
                    6,
                    8);
            }
            else if (animalTexture == "rabbit")
            {
                animal.SetHitboxMargins(
                    5,
                    8,
                    6,
                    9);
            }
            else
            {
                animal.SetHitboxMargins(
                    5,
                    8,
                    6,
                    8);
            }


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


        // ==============================
        // TEXTURE
        // ==============================

        if (!vehicleTexture.empty())
        {
            vehicle.SetTexture(
                vehicleTexture);


            // ==========================================
            // DEER SPRITE SIZE
            // ==========================================

            if (vehicleTexture == "deer")
            {
                vehicle.SetSpriteSize(
                    Config::DEER_WIDTH,
                    Config::DEER_HEIGHT);
            }
        }
        else
        {
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
            }
        }


        // ==============================
        // POSITION
        // ==============================

        int vehicleY =
            y + height
            - vehicle.GetRect().h;


        vehicle.SetPosition(
            i * spacing,
            vehicleY);


        // ==============================
        // HITBOX
        // ==============================

        vehicle.SetLaneHeight(height);


        // ==============================
        // MOVEMENT
        // ==============================

        vehicle.SetSpeed(speed);

        vehicle.SetDirection(direction);


        vehicles.push_back(vehicle);
    }
}


// ==================================================
// UPDATE
// ==================================================

void Lane::Update()
{
    trafficLight.Update();


    if (trafficLight.CanMove())
    {
        for (Vehicle& vehicle : vehicles)
        {
            vehicle.Update();
        }


        for (Animal& animal : animals)
        {
            animal.Update();
        }
    }
}


// ==================================================
// DRAW
// ==================================================

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


    for (Animal& animal : animals)
    {
        animal.Draw(
            renderer,
            textureManager);
    }


    trafficLight.Draw(renderer);
}


// ==================================================
// GET VEHICLES
// ==================================================

const std::vector<Vehicle>&
Lane::GetVehicles() const
{
    return vehicles;
}


// ==================================================
// GET ANIMALS
// ==================================================

const std::vector<Animal>&
Lane::GetAnimals() const
{
    return animals;
}