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
    const std::string& animalTexture,
    int lightX,
    int lightY)
    : trafficLight(lightX, lightY)
{
    if (!vehicleTexture.empty())
    {
        int spacing = 250;

        for (int i = 0; i < vehicleCount; i++)
        {
            Vehicle vehicle;

            int vehicleY =
                y + height - Config::VEHICLE_HEIGHT;

            vehicle.SetPosition(
                i * spacing,
                vehicleY);

            vehicle.SetLaneHeight(height);

            vehicle.SetSpeed(speed);
            vehicle.SetDirection(direction);

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

    if (!animalTexture.empty())
    {
        int spacing = 250;

        for (int i = 0; i < vehicleCount; i++)
        {
            Animal animal;

            animal.SetTexture(animalTexture);

            SDL_Rect animalRect =
                animal.GetRect();

            int animalY =
                y + height - animalRect.h;

            animal.SetPosition(
                i * spacing,
                animalY);

            if (animalTexture == "deer")
            {
                SDL_Rect deerRect =
                    animal.GetRect();

                constexpr float SIDE_REDUCTION = 0.05f;

                int sideOffset =
                    static_cast<int>(
                        deerRect.w * SIDE_REDUCTION) ;

                int hitboxHeight =
                    height / 2 ;

                int topMargin =
                    deerRect.h - hitboxHeight;

                if (topMargin < 0)
                    topMargin = 0;

                animal.SetHitboxMargins(
                    sideOffset,
                    topMargin,
                    sideOffset,
                    6);
            }

            animal.SetSpeed(speed);
            animal.SetDirection(direction);

            animals.push_back(animal);
        }
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

    trafficLight.Draw(
        renderer,
        textureManager);
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