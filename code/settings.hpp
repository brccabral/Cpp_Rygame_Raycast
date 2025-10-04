#pragma once
#include <rygame.hpp>

class Settings
{
public:

    static Settings *GetInstance();
    static void Destroy();

    int width = 1200;
    int height = 800;
    int half_width = width / 2;
    int half_height = height / 2;
    rl::Color white = {255, 255, 255, 255};
    rl::Color black = {0, 0, 0, 255};
    rl::Color blank = {0, 0, 0, 0};
    rl::Color red = {220, 0, 0, 255};
    rl::Color green = {0, 220, 0, 255};
    rl::Color blue = {0, 0, 220, 255};
    rl::Color darkgray = {40, 40, 40, 255};
    rl::Color purple = {120, 0, 120, 255};
    rl::Color skyblue = {0, 186, 255, 255};
    rl::Color raycolor = {220, 220, 0, 255};
    int tile = 100;

    float fov = PI / 3.0f;
    float half_fov = fov / 2.0f;
    int num_rays = 300;
    int max_depth = 800;
    float delta_angle = fov / num_rays;

    // projection
    float dist = num_rays / (2 * tanf(half_fov));
    float proj_coeff = 3 * dist * tile;
    int scale = width / num_rays;

    rg::math::Vector2<float> fps_pos = {width - 100.0f, 5};

private:

    Settings() = default;
    ~Settings() = default;

    static Settings *instance;
};
