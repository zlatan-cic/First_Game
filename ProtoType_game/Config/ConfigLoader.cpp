#include "ConfigLoader.h"
#include "json.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace
{
    // Turns one JSON object into a PlayerConfig.
    // value("key", default) returns the default if the key doesn't exist.
    PlayerConfig parsePlayer(const json& data)
    {
        PlayerConfig config;

        config.name = data.value("name", config.name);

        if (data.contains("startPosition"))
        {
            const json& position = data.at("startPosition");
            config.startPosition = {
                position.at("x").get<float>(),
                position.at("y").get<float>()
            };
        }

        if (data.contains("color"))
        {
            // Color is an array of three numbers: [red, green, blue], each 0 to 255
            const json& color = data.at("color");
            config.color = sf::Color(
                static_cast<std::uint8_t>(color.at(0).get<int>()),
                static_cast<std::uint8_t>(color.at(1).get<int>()),
                static_cast<std::uint8_t>(color.at(2).get<int>())
            );
        }

        config.speed = data.value("speed", config.speed);
        config.jumpForce = data.value("jumpForce", config.jumpForce);
        config.lives = data.value("lives", config.lives);
        config.maxHealth = data.value("maxHealth", config.maxHealth);

        return config;
    }

    // Color is an array of three numbers: [red, green, blue], each 0 to 255
    sf::Color parseColor(const json& color)
    {
        return sf::Color(
            static_cast<std::uint8_t>(color.at(0).get<int>()),
            static_cast<std::uint8_t>(color.at(1).get<int>()),
            static_cast<std::uint8_t>(color.at(2).get<int>())
        );
    }

    LevelConfig parseLevel(const json& data)
    {
        LevelConfig level;

        level.name = data.value("name", level.name);

        if (data.contains("background"))
        {
            level.background = parseColor(data.at("background"));
        }

        if (data.contains("platformColor"))
        {
            level.platformColor = parseColor(data.at("platformColor"));
        }

        for (const json& platformData : data.at("platforms"))
        {
            PlatformConfig platform;
            platform.position = { platformData.at("x").get<float>(), platformData.at("y").get<float>() };
            platform.size = { platformData.at("width").get<float>(), platformData.at("height").get<float>() };
            level.platforms.push_back(platform);
        }

        return level;
    }

    // Used when levels.json can't be read: the same platforms as the old game
    LevelConfig makeDefaultLevel()
    {
        LevelConfig level;
        level.platforms = {
            { { 100.f, 500.f }, { 600.f, 40.f } },
            { { 100.f, 380.f }, { 250.f, 25.f } },
            { { 450.f, 260.f }, { 250.f, 25.f } }
        };
        return level;
    }
}

std::vector<PlayerConfig> loadPlayerConfigs(const std::string& path, std::size_t playerCount)
{
    std::vector<PlayerConfig> configs;

    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Can't open " << path << ", using default players.\n";
    }
    else
    {
        try
        {
            const json data = json::parse(file);

            for (const json& playerData : data.at("players"))
            {
                configs.push_back(parsePlayer(playerData));
            }
        }
        catch (const json::exception& error)
        {
            std::cerr << "Error in " << path << ": " << error.what() << '\n';
            configs.clear();
        }
    }

    if (configs.size() < playerCount)
    {
        std::cerr << path << " has " << configs.size()
            << " players, but " << playerCount
            << " are needed. The rest get default values.\n";
    }

    // Always exactly playerCount players, so the game never asks for a player that doesn't exist
    configs.resize(playerCount);

    return configs;
}

std::vector<LevelConfig> loadLevelConfigs(const std::string& path)
{
    std::vector<LevelConfig> levels;

    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Can't open " << path << ", using the default level.\n";
    }
    else
    {
        try
        {
            const json data = json::parse(file);

            for (const json& levelData : data.at("levels"))
            {
                levels.push_back(parseLevel(levelData));
            }
        }
        catch (const json::exception& error)
        {
            std::cerr << "Error in " << path << ": " << error.what() << '\n';
            levels.clear();
        }
    }

    // Always at least one level, so the menu and PlayState never get an empty list
    if (levels.empty())
    {
        levels.push_back(makeDefaultLevel());
    }

    return levels;
}