#pragma once
#pragma once
#include <cstddef>

// Choices that must survive switching between states.
// States are created and destroyed, but Game lives the whole time, so it keeps these.
struct GameSettings
{
    std::size_t levelIndex = 0;
};