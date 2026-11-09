#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include "PlayerConfig.h"

// Cita igrace iz JSON fajla.
// Uvek vraca tacno playerCount igraca: ako fajl ne postoji ili ih ima manje,
// ispise upozorenje i dopuni listu podrazumevanim vrednostima iz PlayerConfig.
std::vector<PlayerConfig> loadPlayerConfigs(const std::string& path, std::size_t playerCount);
