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
    : trafficLight(760, y)
{
    // ==================================================
    // WAGON / VEHICLE LANE
    // ==================================================
    //
    // Chỉ xử lý Vehicle khi vehicleTexture khác rỗng.
    //
    // Deer KHÔNG đi vào đây.
    //
    // ==================================================

    if (!vehicleTexture.empty())
    {
        int spacing = 250;

        for (int i = 0; i < vehicleCount; i++)
        {
            Vehicle vehicle;


            // ==========================================
            // SPRITE
            // ==========================================
            //
            // Đáy sprite trùng đáy lane.
            //
            // spriteY + spriteHeight
            //      =
            // laneY + laneHeight
            //
            // ==========================================

            int vehicleY =
                y
                + height
                - Config::VEHICLE_HEIGHT;


            vehicle.SetPosition(
                i * spacing,
                vehicleY);


            // ==========================================
            // HITBOX WAGON
            // ==========================================
            //
            // Giữ nguyên cơ chế của bản
            // "Fix hitbox okay".
            //
            vehicle.SetLaneHeight(
                height);


            // ==========================================
            // MOVEMENT
            // ==========================================

            vehicle.SetSpeed(
                speed);

            vehicle.SetDirection(
                direction);


            // ==========================================
            // TEXTURE
            // ==========================================
            //
            // Wagon vẫn dùng 4 texture như cũ.
            //
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


            // ==========================================
            // ADD WAGON
            // ==========================================

            vehicles.push_back(
                vehicle);
        }
    }


    // ==================================================
    // ANIMAL LANE
    // ==================================================
    //
    // Deer / squirrel / rabbit đều đi vào Animal.
    //
    // Điều này tách hoàn toàn hitbox Animal
    // khỏi hitbox Vehicle.
    //
    // ==================================================

    if (!animalTexture.empty())
    {
        int spacing = 250;

        for (int i = 0; i < vehicleCount; i++)
        {
            Animal animal;


            // ==========================================
            // TEXTURE
            // ==========================================

            animal.SetTexture(
                animalTexture);


            // ==========================================
            // POSITION
            // ==========================================
            //
            // Đáy sprite trùng đáy lane.
            //
            // Giống cách căn wagon.
            //
            // ==========================================

            SDL_Rect animalRect =
                animal.GetRect();


            int animalY =
                y
                + height
                - animalRect.h;


            animal.SetPosition(
                i * spacing,
                animalY);


            // ==========================================
            // DEER HITBOX
            // ==========================================
            //
            // Deer hiện tại trước đây đang dùng:
            //
            // hitbox.h = laneHeight / 2
            //
            // và đáy hitbox trùng đáy sprite.
            //
            // Ta giữ nguyên TÍNH CHẤT đó,
            // nhưng chuyển nó sang Animal.
            //
            // Với:
            //
            // Deer height = 100
            // Lane height = 82
            //
            // hitbox height = 41
            //
            // top margin = 100 - 41 = 59
            //
            // => hitbox chỉ nằm ở phần dưới Deer.
            //
            // ==========================================

            if (animalTexture == "deer")
            {
                SDL_Rect deerRect =
                    animal.GetRect();


                constexpr float SIDE_REDUCTION = 0.05f;


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
                    0);
            }


            // ==========================================
            // MOVEMENT
            // ==========================================

            animal.SetSpeed(
                speed);

            animal.SetDirection(
                direction);


            // ==========================================
            // ADD ANIMAL
            // ==========================================

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
    // Traffic light vẫn hoạt động.
    trafficLight.Update();


    // ==========================================
    // VEHICLES
    // ==========================================

    if (trafficLight.CanMove())
    {
        for (Vehicle& vehicle : vehicles)
        {
            vehicle.Update();
        }


        // ==========================================
        // ANIMALS
        // ==========================================

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
    // WAGON
    // ==========================================

    for (Vehicle& vehicle : vehicles)
    {
        vehicle.Draw(
            renderer,
            textureManager);
    }


    // ==========================================
    // ANIMAL
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
        renderer);
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