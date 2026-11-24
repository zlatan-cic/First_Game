#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include "PlayerConfig.h"
#include "LevelConfig.h"

// Reads players from a JSON file.
// Always returns exactly playerCount players: if the file is missing or has fewer,
// it prints a warning and fills the list with default values from PlayerConfig.
std::vector<PlayerConfig> loadPlayerConfigs(const std::string& path, std::size_t playerCount);

// Reads levels from a JSON file.
// Always returns at least one level: if the file is missing or broken,
// it returns one default level, so the game can always start.
std::vector<LevelConfig> loadLevelConfigs(const std::string& path);