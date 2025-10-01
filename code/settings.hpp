#pragma once
#include <rygame.hpp>

class Settings
{
public:

    static Settings *GetInstance();
    static void Destroy();

    int width = 1200;
    int height = 800;
    int half_width = 1200 / 2;
    int half_height = 800 / 2;
    rl::Color white = {255, 255, 255, 255};
    rl::Color black = {0, 0, 0, 255};
    rl::Color red = {220, 0, 0, 255};
    rl::Color green = {0, 220, 0, 255};
    rl::Color blue = {0, 0, 220, 255};
    rl::Color darkgray = {110, 110, 110, 255};
    rl::Color purple = {120, 0, 120, 255};
    rg::math::Vector2 player_pos = {1200 / 2, 800 / 2};
    float player_angle = 2.0f;
    float player_speed = 0.0f;
    float player_rotation = 0.0f;

private:

    Settings() = default;
    ~Settings() = default;

    static Settings *instance;
};
