#include "Objects/Lane.h"

#include "Graphics/TextureManager.h"
#include "Config/GameConfig.h"


// ==================================================
// CONSTRUCTOR
// ==================================================

Lane::Lane(
    int y,
    int height,
    int direction,
    int speed,
    int vehicleCount,
    const std::string& vehicleTexture,
    const std::string& animalTexture)
{
    this->y = y;
    this->height = height;
    this->direction = direction;
    this->speed = speed;


    // ==================================================
    // VEHICLE LANE
    // ==================================================

    if (!vehicleTexture.empty())
    {
        for (int i = 0; i < vehicleCount; i++)
        {
            Vehicle vehicle;


            // ------------------------------------------
            // Position
            // ------------------------------------------

            int x =
                i * (Config::WINDOW_WIDTH / vehicleCount);


            vehicle.SetPosition(
                x,
                y);


            // ------------------------------------------
            // Movement
            // ------------------------------------------

            vehicle.SetSpeed(
                speed);


            vehicle.SetDirection(
                direction);


            // ------------------------------------------
            // Lane height
            // ------------------------------------------

            vehicle.SetLaneHeight(
                height);


            // ------------------------------------------
            // Texture
            // ------------------------------------------

            vehicle.SetTexture(
                vehicleTexture);


            // ------------------------------------------
            // Deer
            // ------------------------------------------
            //
            // Deer vẫn là Vehicle.
            //
            // Vì vậy deer dùng cùng hệ hitbox
            // với wagon.
            //
            // Chỉ thay texture + kích thước sprite.
            //

            if (vehicleTexture == "deer")
            {
                vehicle.SetSpriteSize(
                    Config::DEER_WIDTH,
                    Config::DEER_HEIGHT);
            }


            vehicles.push_back(
                vehicle);
        }
    }


    // ==================================================
    // ANIMAL LANE
    // ==================================================

    if (!animalTexture.empty())
    {
        for (int i = 0; i < vehicleCount; i++)
        {
            Animal animal;


            // ------------------------------------------
            // Position
            // ------------------------------------------

            int x =
                i * (Config::WINDOW_WIDTH / vehicleCount);


            animal.SetPosition(
                x,
                y);


            // ------------------------------------------
            // Movement
            // ------------------------------------------

            animal.SetSpeed(
                speed);


            animal.SetDirection(
                direction);


            // ------------------------------------------
            // Lane height
            // ------------------------------------------

            animal.SetLaneHeight(
                height);


            // ------------------------------------------
            // Texture
            // ------------------------------------------

            animal.SetTexture(
                animalTexture);


            // ------------------------------------------
            // Individual animal size
            // ------------------------------------------

            if (animalTexture == "squirrel")
            {
                animal.SetSpriteSize(
                    Config::SQUIRREL_WIDTH,
                    Config::SQUIRREL_HEIGHT);
            }
            else if (animalTexture == "rabbit")
            {
                animal.SetSpriteSize(
                    Config::RABBIT_WIDTH,
                    Config::RABBIT_HEIGHT);
            }


            animals.push_back(
                animal);
        }
    }
}


// ==================================================
// UPDATE
// ==================================================

void Lane::Update()
{
    // ------------------------------------------
    // Vehicles
    // ------------------------------------------

    for (Vehicle& vehicle : vehicles)
    {
        vehicle.Update();
    }


    // ------------------------------------------
    // Animals
    // ------------------------------------------

    for (Animal& animal : animals)
    {
        animal.Update();
    }
}


// ==================================================
// DRAW
// ==================================================

void Lane::Draw(
    SDL_Renderer* renderer,
    TextureManager& textureManager)
{
    // ------------------------------------------
    // Vehicles
    // ------------------------------------------

    for (Vehicle& vehicle : vehicles)
    {
        vehicle.Draw(
            renderer,
            textureManager);
    }


    // ------------------------------------------
    // Animals
    // ------------------------------------------

    for (Animal& animal : animals)
    {
        animal.Draw(
            renderer,
            textureManager);
    }
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