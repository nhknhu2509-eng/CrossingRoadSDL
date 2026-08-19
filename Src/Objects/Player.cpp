#include "Objects/Player.h"
#include "Managers/InputManager.h"
#include "Config/GameConfig.h"

Player::Player()
{
    rect.x = Config::PLAYER_START_X;
    rect.y = Config::PLAYER_START_Y;

    rect.w = Config::PLAYER_WIDTH;
    rect.h = Config::PLAYER_HEIGHT;

    speed = Config::PLAYER_SPEED;
}

void Player::Update()
{
    if (InputManager::IsKeyDown(SDL_SCANCODE_W) ||
        InputManager::IsKeyDown(SDL_SCANCODE_UP))
    {
        rect.y -= speed;
    }

    if (InputManager::IsKeyDown(SDL_SCANCODE_S) ||
        InputManager::IsKeyDown(SDL_SCANCODE_DOWN))
    {
        rect.y += speed;
    }

    if (InputManager::IsKeyDown(SDL_SCANCODE_A) ||
        InputManager::IsKeyDown(SDL_SCANCODE_LEFT))
    {
        rect.x -= speed;
    }

    if (InputManager::IsKeyDown(SDL_SCANCODE_D) ||
        InputManager::IsKeyDown(SDL_SCANCODE_RIGHT))
    {
        rect.x += speed;
    }

    if (rect.x < 0)
        rect.x = 0;

    if (rect.y < 0)
        rect.y = 0;

    if (rect.x + rect.w > Config::WINDOW_WIDTH)
        rect.x = Config::WINDOW_WIDTH - rect.w;

    if (rect.y + rect.h > Config::WINDOW_HEIGHT)
        rect.y = Config::WINDOW_HEIGHT - rect.h;
}

void Player::Draw(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(
        renderer,
        Config::PLAYER_COLOR.r,
        Config::PLAYER_COLOR.g,
        Config::PLAYER_COLOR.b,
        Config::PLAYER_COLOR.a);

    SDL_RenderFillRect(renderer, &rect);
}

void Player::Reset()
{
    rect.x = Config::PLAYER_START_X;
    rect.y = Config::PLAYER_START_Y;
}

void Player::SetPosition(int x, int y)
{
    rect.x = x;
    rect.y = y;
}

SDL_Rect Player::GetRect() const
{
    return rect;
}