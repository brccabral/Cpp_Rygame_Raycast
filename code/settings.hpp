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
    float penta_height = 5.0f * height;
    rl::Color white = {255, 255, 255, 255};
    rl::Color black = {0, 0, 0, 255};
    rl::Color blank = {0, 0, 0, 0};
    rl::Color red = {220, 0, 0, 255};
    rl::Color green = {0, 80, 0, 255};
    rl::Color blue = {0, 0, 220, 255};
    rl::Color darkgray = {40, 40, 40, 255};
    rl::Color purple = {120, 0, 120, 255};
    rl::Color skyblue = {0, 186, 255, 255};
    rl::Color raycolor = {220, 220, 0, 255};
    rl::Color yellow = {220, 220, 0, 255};
    rl::Color sandy = {244, 164, 90, 255};
    rl::Color darkbrown = {97, 61, 25, 255};
    rl::Color darkorange = {255, 140, 0, 255};
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

    // mini map
    int minimap_scale = 5;
    rg::math::Vector2<int> minimap_res = {width / minimap_scale, height / minimap_scale};
    int map_scale = 2 * minimap_scale;
    int map_tile = tile / map_scale;
    rg::math::Vector2<int> map_pos = {0, height - height / minimap_scale};

    // texture settings (1200 x 1200)
    int texture_width = 1200;
    int texture_height = 1200;
    int texture_scale = texture_width / tile;

    // sprite settings
    float double_pi = 2 * M_PI;
    int center_ray = num_rays / 2 - 1;
    int fake_rays = 100;

private:

    Settings() = default;
    ~Settings() = default;

    static Settings *instance;
};

struct SpriteObjectLocate
{
    float depth{};
    rg::Surface *sprite{};
    rg::math::Vector2<float> sprite_dimension{};
    rg::math::Vector2<int> sprite_pos{};
    rg::Rect sprite_area{};
};
