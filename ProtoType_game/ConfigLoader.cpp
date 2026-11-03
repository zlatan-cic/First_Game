#include "ConfigLoader.h"
#include "json.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace
{
    // Pretvara jedan objekat iz JSON-a u PlayerConfig.
    // value("kljuc", podrazumevano) vraca podrazumevanu vrednost ako kljuc ne postoji.
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
            // Boja je niz od tri broja: [crvena, zelena, plava], svaki od 0 do 255
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
}

std::vector<PlayerConfig> loadPlayerConfigs(const std::string& path, std::size_t playerCount)
{
    std::vector<PlayerConfig> configs;

    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Ne mogu da otvorim " << path << ", koristim podrazumevane igrace.\n";
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
            std::cerr << "Greska u " << path << ": " << error.what() << '\n';
            configs.clear();
        }
    }

    if (configs.size() < playerCount)
    {
        std::cerr << "U " << path << " ima " << configs.size()
                  << " igraca, a treba " << playerCount
                  << ". Ostali dobijaju podrazumevane vrednosti.\n";
    }

    // Uvek tacno playerCount igraca, da Game nikad ne trazi igraca koji ne postoji
    configs.resize(playerCount);

    return configs;
}
