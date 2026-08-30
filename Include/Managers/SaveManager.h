#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <SDL.h>

#include "Core/GameState.h"
#include "World/TrafficLight.h"


// ==================================================
// SAVE DATA
// ==================================================

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


// ==================================================
// SAVE MANAGER
// ==================================================

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
            GetSavePath(),
            std::ios::out |
            std::ios::trunc);


        if (!file.is_open())
        {
            return false;
        }


        // ======================================
        // VERSION
        // ======================================

        file
            << "VERSION 2\n";


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


        // ======================================
        // CHECK WRITE RESULT
        // ======================================

        file.flush();


        bool writeSucceeded =
            file.good();


        file.close();


        return writeSucceeded;
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

        if (!(
            file
            >> label
            >> version))
        {
            return false;
        }


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


        if (!(
            file
            >> label
            >> stateValue))
        {
            return false;
        }


        if (
            label != "STATE" ||
            !IsValidStateValue(
                stateValue))
        {
            return false;
        }


        data.state =
            IntToState(
                stateValue);


        // ======================================
        // SCORE
        // ======================================

        if (!(
            file
            >> label
            >> data.score))
        {
            return false;
        }


        if (
            label != "SCORE" ||
            data.score < 0)
        {
            return false;
        }


        // ======================================
        // PLAYER
        // ======================================

        if (!(
            file
            >> label
            >> data.playerX
            >> data.playerY))
        {
            return false;
        }


        if (label != "PLAYER")
        {
            return false;
        }


        // ======================================
        // PINE CONES
        // ======================================

        int pineConeCount =
            0;


        if (!(
            file
            >> label
            >> pineConeCount))
        {
            return false;
        }


        if (
            label != "PINECONES" ||
            pineConeCount < 0 ||
            pineConeCount > 1000)
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


            if (!(file >> collected))
            {
                return false;
            }


            if (
                collected != 0 &&
                collected != 1)
            {
                return false;
            }


            data.pineConeCollected.push_back(
                collected == 1);
        }


        // ======================================
        // OBSTACLES
        // ======================================

        int obstacleCount =
            0;


        if (!(
            file
            >> label
            >> obstacleCount))
        {
            return false;
        }


        if (
            label != "OBSTACLES" ||
            obstacleCount < 0 ||
            obstacleCount > 10000)
        {
            return false;
        }


        data.obstaclePositions.clear();


        for (
            int i = 0;
            i < obstacleCount;
            i++)
        {
            SDL_Point position =
            {
                0,
                0
            };


            if (!(
                file
                >> position.x
                >> position.y))
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


        if (!(
            file
            >> label
            >> lightCount))
        {
            return false;
        }


        if (
            label != "LIGHTS" ||
            lightCount < 0 ||
            lightCount > 1000)
        {
            return false;
        }


        data.trafficLights.clear();


        for (
            int i = 0;
            i < lightCount;
            i++)
        {
            int lightStateValue =
                0;


            Uint32 elapsedTime =
                0;


            if (!(
                file
                >> lightStateValue
                >> elapsedTime))
            {
                return false;
            }


            if (
                lightStateValue < 0 ||
                lightStateValue > 2)
            {
                return false;
            }


            TrafficLightSaveState
                light;


            light.state =
                IntToLightState(
                    lightStateValue);


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
    // VALID SAVE STATE
    // ==========================================

    static bool IsValidStateValue(
        int value)
    {
        return
            value == 1 ||
            value == 2 ||
            value == 3;
    }


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
            return 1;
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
