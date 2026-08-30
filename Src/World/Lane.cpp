#include "World/Lane.h"

#include "Graphics/TextureManager.h"

#include "Config/GameConfig.h"

#include "Objects/Vehicle.h"
#include "Objects/Animal.h"
#include "Objects/Deer.h"
#include "Objects/Squirrel.h"
#include "Objects/Rabbit.h"


Lane::Lane(
    int y,
    int height,
    int direction,
    int speed,
    int vehicleCount,
    const std::string& vehicleTexture,
    const std::string& animalTexture,
    int lightX,
    int lightY,
    Uint32 greenTime,
    Uint32 yellowTime,
    Uint32 redTime)
    : trafficLight(
        lightX,
        lightY,
        greenTime,
        yellowTime,
        redTime)
{
    // ==========================================
    // VEHICLES
    // ==========================================

    if (!vehicleTexture.empty())
    {
        int spacing = 250;


        for (int i = 0;
            i < vehicleCount;
            i++)
        {
            std::unique_ptr<Vehicle>
                vehicle =
                std::make_unique<Vehicle>();


            int vehicleY =
                y
                + height
                - Config::VEHICLE_HEIGHT;


            vehicle->SetPosition(
                i * spacing,
                vehicleY);


            vehicle->SetLaneHeight(
                height);


            vehicle->SetSpeed(
                speed);


            vehicle->SetDirection(
                direction);


            // Giữ nguyên thứ tự wagon hiện tại
            switch (i % 4)
            {
            case 0:

                vehicle->SetTexture(
                    "wagon1");

                break;


            case 1:

                vehicle->SetTexture(
                    "wagon2");

                break;


            case 2:

                vehicle->SetTexture(
                    "wagon3");

                break;


            case 3:

                vehicle->SetTexture(
                    "wagon4");

                break;


            default:

                break;
            }


            obstacles.push_back(
                std::move(vehicle));
        }
    }


    // ==========================================
    // ANIMALS
    // ==========================================

    if (!animalTexture.empty())
    {
        int spacing = 250;


        for (int i = 0;
            i < vehicleCount;
            i++)
        {
            std::unique_ptr<Animal>
                animal;


            // ==================================
            // FACTORY LOGIC
            // ==================================

            if (animalTexture == "deer")
            {
                animal =
                    std::make_unique<Deer>();
            }

            else if (
                animalTexture ==
                "squirrel")
            {
                animal =
                    std::make_unique<Squirrel>();
            }

            else if (
                animalTexture ==
                "rabbit")
            {
                animal =
                    std::make_unique<Rabbit>();
            }


            if (animal == nullptr)
            {
                continue;
            }


            SDL_Rect animalRect =
                animal->GetRect();


            int animalY =
                y
                + height
                - animalRect.h;


            animal->SetPosition(
                i * spacing,
                animalY);


            // Deer dùng hàm override.
            // Squirrel/Rabbit không làm gì.
            animal->SetLaneHeight(
                height);


            animal->SetSpeed(
                speed);


            animal->SetDirection(
                direction);


            obstacles.push_back(
                std::move(animal));
        }
    }
}


// ==================================================
// UPDATE
// ==================================================

void Lane::Update()
{
    trafficLight.Update();


    // Logic đèn giữ nguyên:
    // Red -> không Update obstacle
    if (trafficLight.CanMove())
    {
        for (
            std::unique_ptr<Obstacle>& obstacle :
            obstacles)
        {
            obstacle->Update();
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
    for (
        std::unique_ptr<Obstacle>& obstacle :
        obstacles)
    {
        obstacle->Draw(
            renderer,
            textureManager);
    }


    trafficLight.Draw(
        renderer,
        textureManager);
}


// ==================================================
// GET OBSTACLES
// ==================================================

const std::vector<
    std::unique_ptr<Obstacle>>&
    Lane::GetObstacles() const
{
    return obstacles;
}