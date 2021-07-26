#pragma once

constexpr const float gravity = -9.81f;

constexpr const struct DefaultLimits_t {
    static constexpr const float slowJumpLimit = 6.5f;
    static constexpr const float fastJumpLimit = 8.5f;
    static constexpr const float slowJumpMultiplier = 1.1f;
    static constexpr const float fastJumpMultiplier = 1.3f;
} DefaultLimits;
