#include "World/Lane.h"

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


            // ==================================================
            // X POSITION
            // ==================================================

            int x =
                i * (Config::WINDOW_WIDTH / vehicleCount);


            // ==================================================
            // TEXTURE
            // ==================================================

            vehicle.SetTexture(
                vehicleTexture);


            // ==================================================
            // DEER SIZE
            // ==================================================

            if (vehicleTexture == "deer")
            {
                vehicle.SetSpriteSize(
                    Config::DEER_WIDTH,
                    Config::DEER_HEIGHT);
            }


            // ==================================================
            // LANE HEIGHT
            // ==================================================

            vehicle.SetLaneHeight(
                height);


            // ==================================================
            // POSITION
            // ==================================================
            //
            // QUAN TRỌNG:
            //
            // Đáy sprite phải trùng với đáy lane.
            //
            // spriteY + spriteHeight
            //      =
            // laneY + laneHeight
            //
            // => spriteY =
            //    laneY + laneHeight - spriteHeight
            //
            // Điều này giữ wagon và deer
            // có cùng tính chất hitbox.
            //
            // ==================================================

            SDL_Rect rect =
                vehicle.GetRect();


            int vehicleY =
                y
                + height
                - rect.h;


            vehicle.SetPosition(
                x,
                vehicleY);


            // ==================================================
            // MOVEMENT
            // ==================================================

            vehicle.SetSpeed(
                speed);

            vehicle.SetDirection(
                direction);


            // ==================================================
            // ADD VEHICLE
            // ==================================================

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


            // ==================================================
            // X POSITION
            // ==================================================

            int x =
                i * (Config::WINDOW_WIDTH / vehicleCount);


            // ==================================================
            // TEXTURE
            // ==================================================

            animal.SetTexture(
                animalTexture);


            // ==================================================
            // SIZE
            // ==================================================

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


            // ==================================================
            // LANE HEIGHT
            // ==================================================

            animal.SetLaneHeight(
                height);


            // ==================================================
            // POSITION
            // ==================================================

            animal.SetPosition(
                x,
                y);


            // ==================================================
            // MOVEMENT
            // ==================================================

            animal.SetSpeed(
                speed);

            animal.SetDirection(
                direction);


            // ==================================================
            // ADD ANIMAL
            // ==================================================

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
    for (Vehicle& vehicle : vehicles)
    {
        vehicle.Update();
    }


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