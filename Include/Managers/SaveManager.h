#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <SDL.h>

#include "Core/GameState.h"
#include "World/TrafficLight.h"


struct SaveData
{
    GameState state;

    int score;

    int playerX;
    int playerY;

    std::vector<bool>
        pineConeCollected;

    std::vector<SDL_Point>
        obstaclePositions;

    std::vector<TrafficLightSaveState>
        trafficLights;
};


class SaveManager
{
public:

    // ==========================================
    // SAVE FILE PATH
    // ==========================================

    static const char* GetSavePath()
    {
        return "savegame.txt";
    }


    // ==========================================
    // SAVE
    // ==========================================

    static bool Save(
        const SaveData& data)
    {
        std::ofstream file(
            GetSavePath());


        if (!file.is_open())
        {
            return false;
        }


        // ======================================
        // VERSION
        // ======================================

        file << "VERSION 2\n";


        // ======================================
        // GAME STATE
        // ======================================

        file
            << "STATE "
            << StateToInt(
                data.state)
            << "\n";


        // ======================================
        // SCORE
        // ======================================

        file
            << "SCORE "
            << data.score
            << "\n";


        // ======================================
        // PLAYER
        // ======================================

        file
            << "PLAYER "
            << data.playerX
            << " "
            << data.playerY
            << "\n";


        // ======================================
        // PINE CONES
        // ======================================

        file
            << "PINECONES "
            << data.pineConeCollected.size()
            << "\n";


        for (
            bool collected :
        data.pineConeCollected)
        {
            file
                << (collected ? 1 : 0)
                << "\n";
        }


        // ======================================
        // OBSTACLES
        // ======================================

        file
            << "OBSTACLES "
            << data.obstaclePositions.size()
            << "\n";


        for (
            const SDL_Point& position :
            data.obstaclePositions)
        {
            file
                << position.x
                << " "
                << position.y
                << "\n";
        }


        // ======================================
        // TRAFFIC LIGHTS
        // ======================================

        file
            << "LIGHTS "
            << data.trafficLights.size()
            << "\n";


        for (
            const TrafficLightSaveState& light :
            data.trafficLights)
        {
            file
                << LightStateToInt(
                    light.state)
                << " "
                << light.elapsedTime
                << "\n";
        }


        file.close();


        return true;
    }


    // ==========================================
    // LOAD
    // ==========================================

    static bool Load(
        SaveData& data)
    {
        std::ifstream file(
            GetSavePath());


        if (!file.is_open())
        {
            return false;
        }


        std::string label;


        int version =
            0;


        // ======================================
        // VERSION
        // ======================================

        file
            >> label
            >> version;


        if (
            label != "VERSION" ||
            version != 2)
        {
            return false;
        }


        // ======================================
        // STATE
        // ======================================

        int stateValue =
            0;


        file
            >> label
            >> stateValue;


        if (label != "STATE")
        {
            return false;
        }


        data.state =
            IntToState(
                stateValue);


        // ======================================
        // SCORE
        // ======================================

        file
            >> label
            >> data.score;


        if (label != "SCORE")
        {
            return false;
        }


        // ======================================
        // PLAYER
        // ======================================

        file
            >> label
            >> data.playerX
            >> data.playerY;


        if (label != "PLAYER")
        {
            return false;
        }


        // ======================================
        // PINE CONES
        // ======================================

        int pineConeCount =
            0;


        file
            >> label
            >> pineConeCount;


        if (
            label != "PINECONES" ||
            pineConeCount < 0)
        {
            return false;
        }


        data.pineConeCollected.clear();


        for (
            int i = 0;
            i < pineConeCount;
            i++)
        {
            int collected =
                0;


            file >> collected;


            if (!file)
            {
                return false;
            }


            data.pineConeCollected.push_back(
                collected != 0);
        }


        // ======================================
        // OBSTACLES
        // ======================================

        int obstacleCount =
            0;


        file
            >> label
            >> obstacleCount;


        if (
            label != "OBSTACLES" ||
            obstacleCount < 0)
        {
            return false;
        }


        data.obstaclePositions.clear();


        for (
            int i = 0;
            i < obstacleCount;
            i++)
        {
            SDL_Point position;


            file
                >> position.x
                >> position.y;


            if (!file)
            {
                return false;
            }


            data.obstaclePositions.push_back(
                position);
        }


        // ======================================
        // TRAFFIC LIGHTS
        // ======================================

        int lightCount =
            0;


        file
            >> label
            >> lightCount;


        if (
            label != "LIGHTS" ||
            lightCount < 0)
        {
            return false;
        }


        data.trafficLights.clear();


        for (
            int i = 0;
            i < lightCount;
            i++)
        {
            int stateValue =
                0;


            Uint32 elapsedTime =
                0;


            file
                >> stateValue
                >> elapsedTime;


            if (!file)
            {
                return false;
            }


            TrafficLightSaveState
                light;


            light.state =
                IntToLightState(
                    stateValue);


            light.elapsedTime =
                elapsedTime;


            data.trafficLights.push_back(
                light);
        }


        file.close();


        return true;
    }


    // ==========================================
    // CHECK SAVE FILE
    // ==========================================

    static bool HasSaveFile()
    {
        std::ifstream file(
            GetSavePath());


        return file.good();
    }


private:

    // ==========================================
    // GAME STATE -> INT
    // ==========================================

    static int StateToInt(
        GameState state)
    {
        switch (state)
        {
        case GameState::Paused:
            return 1;


        case GameState::GameOver:
            return 2;


        case GameState::LevelComplete:
            return 3;


        default:
            return 0;
        }
    }


    // ==========================================
    // INT -> GAME STATE
    // ==========================================

    static GameState IntToState(
        int value)
    {
        switch (value)
        {
        case 1:
            return GameState::Paused;


        case 2:
            return GameState::GameOver;


        case 3:
            return GameState::LevelComplete;


        default:
            return GameState::Paused;
        }
    }


    // ==========================================
    // LIGHT STATE -> INT
    // ==========================================

    static int LightStateToInt(
        LightState state)
    {
        switch (state)
        {
        case LightState::Green:
            return 0;


        case LightState::Yellow:
            return 1;


        case LightState::Red:
            return 2;


        default:
            return 0;
        }
    }


    // ==========================================
    // INT -> LIGHT STATE
    // ==========================================

    static LightState IntToLightState(
        int value)
    {
        switch (value)
        {
        case 0:
            return LightState::Green;


        case 1:
            return LightState::Yellow;


        case 2:
            return LightState::Red;


        default:
            return LightState::Green;
        }
    }
};