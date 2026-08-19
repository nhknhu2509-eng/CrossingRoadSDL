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
            // ANIMAL POSITION
            // ==========================================
            //
            // Animal cao đúng bằng lane.
            //
            // Vì:
            //
            // ANIMAL_HEIGHT = 82
            // LANE_HEIGHT   = 82
            //
            // nên đáy Animal trùng đáy lane.
            //

            int animalY =
                y + height
                - Config::ANIMAL_HEIGHT;


            animal.SetPosition(
                i * spacing,
                animalY);


            // ==========================================
            // MOVEMENT
            // ==========================================

            animal.SetSpeed(speed);

            animal.SetDirection(direction);


            // ==========================================
            // TEXTURE
            // ==========================================

            animal.SetTexture(
                animalTexture);


            // ==========================================
            // HITBOX
            // ==========================================
            //
            // Các PNG đều là 512x512.
            //
            // Sau khi scale xuống 82x82,
            // vùng transparent được tính từ
            // alpha bbox của từng PNG.
            //
            // ------------------------------------------
            //
            // DEER:
            // left   = 5
            // top    = 8
            // right  = 6
            // bottom = 7
            //
            // RABBIT:
            // left   = 5
            // top    = 8
            // right  = 6
            // bottom = 9
            //
            // SQUIRREL:
            // left   = 5
            // top    = 8
            // right  = 6
            // bottom = 8
            //
            // ==========================================


            if (animalTexture == "deer")
            {
                animal.SetHitboxMargins(
                    5,      // left
                    8,      // top
                    6,      // right
                    7);     // bottom
            }
            else if (animalTexture == "rabbit")
            {
                animal.SetHitboxMargins(
                    5,      // left
                    8,      // top
                    6,      // right
                    9);     // bottom
            }
            else if (animalTexture == "squirrel")
            {
                animal.SetHitboxMargins(
                    5,      // left
                    8,      // top
                    6,      // right
                    8);     // bottom
            }
            else
            {
                // Giá trị mặc định
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


        // ==========================================
        // VEHICLE POSITION
        // ==========================================

        int vehicleY =
            y + height
            - Config::VEHICLE_HEIGHT;


        vehicle.SetPosition(
            i * spacing,
            vehicleY);


        // ==========================================
        // VEHICLE HITBOX
        // ==========================================
        //
        // GIỮ NGUYÊN LOGIC VEHICLE.
        //
        // Không áp dụng hitbox Animal
        // cho Vehicle.
        //

        vehicle.SetLaneHeight(height);


        // ==========================================
        // MOVEMENT
        // ==========================================

        vehicle.SetSpeed(speed);

        vehicle.SetDirection(direction);


        // ==========================================
        // TEXTURE
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