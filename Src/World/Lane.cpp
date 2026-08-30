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


        for (int i = 0; i < vehicleCount; i++)
        {
            Vehicle vehicle;


            int vehicleY =
                y
                + height
                - Config::VEHICLE_HEIGHT;


            vehicle.SetPosition(
                i * spacing,
                vehicleY);


            vehicle.SetLaneHeight(
                height);


            vehicle.SetSpeed(
                speed);


            vehicle.SetDirection(
                direction);


            // ==================================
            // CHỌN LOẠI WAGON
            // ==================================
            //
            // Mỗi loại wagon có 3 frame:
            //
            // wagon1_01 -> wagon1_03
            // wagon2_01 -> wagon2_03
            // wagon3_01 -> wagon3_03
            // wagon4_01 -> wagon4_03
            //
            // Vehicle::SetTexture()
            // sẽ tự thiết lập các frame.
            // ==================================

            switch (i % 4)
            {
            case 0:

                vehicle.SetTexture(
                    "wagon1");

                break;


            case 1:

                vehicle.SetTexture(
                    "wagon2");

                break;


            case 2:

                vehicle.SetTexture(
                    "wagon3");

                break;


            case 3:

                vehicle.SetTexture(
                    "wagon4");

                break;


            default:

                break;
            }


            vehicles.push_back(
                vehicle);
        }
    }


    // ==========================================
    // ANIMALS
    // ==========================================

    if (!animalTexture.empty())
    {
        int spacing = 250;


        for (int i = 0; i < vehicleCount; i++)
        {
            Animal animal;


            // ==================================
            // CHỌN LOẠI ANIMAL
            // ==================================
            //
            // deer:
            // deer_01 -> deer_05
            //
            // squirrel:
            // squirrel_01 -> squirrel_04
            //
            // rabbit:
            // rabbit_01 -> rabbit_03
            //
            // Animal::SetTexture()
            // sẽ tự thiết lập animation.
            // ==================================

            animal.SetTexture(
                animalTexture);


            SDL_Rect animalRect =
                animal.GetRect();


            int animalY =
                y
                + height
                - animalRect.h;


            animal.SetPosition(
                i * spacing,
                animalY);


            // ==================================
            // DEER HITBOX
            // ==================================
            //
            // Giữ nguyên hitbox nai
            // mà bạn đã căn trước đó.
            // ==================================

            if (animalTexture == "deer")
            {
                SDL_Rect deerRect =
                    animal.GetRect();


                constexpr float SIDE_REDUCTION =
                    0.05f;


                int sideOffset =
                    static_cast<int>(
                        deerRect.w
                        * SIDE_REDUCTION);


                int hitboxHeight =
                    height / 2;


                int topMargin =
                    deerRect.h
                    - hitboxHeight;


                if (topMargin < 0)
                {
                    topMargin = 0;
                }


                animal.SetHitboxMargins(
                    sideOffset,
                    topMargin,
                    sideOffset,
                    6);
            }


            animal.SetSpeed(
                speed);


            animal.SetDirection(
                direction);


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
    // Đèn của mỗi lane tự update
    trafficLight.Update();


    // Chỉ cho vehicle / animal chạy
    // khi đèn cho phép
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
    // ==========================================
    // VEHICLES
    // ==========================================

    for (Vehicle& vehicle : vehicles)
    {
        vehicle.Draw(
            renderer,
            textureManager);
    }


    // ==========================================
    // ANIMALS
    // ==========================================

    for (Animal& animal : animals)
    {
        animal.Draw(
            renderer,
            textureManager);
    }


    // ==========================================
    // TRAFFIC LIGHT
    // ==========================================

    trafficLight.Draw(
        renderer,
        textureManager);
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