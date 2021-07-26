#pragma once
#include "shared-constants.hpp"

constexpr const struct MaskLayers_t {
    static const int GorillaTrigger = 15;
    static const int HandTrigger = 18;
    static const int PlayerTrigger = 20;
} MaskLayers;

constexpr const char* pluginVersion = VERSION;