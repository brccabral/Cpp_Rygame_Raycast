#pragma once
#include <deque>
#include <rygame.hpp>

#include "player.hpp"
#include "settings.hpp"


struct SpriteParameter
{
    // base sprite
    std::vector<rg::Surface> sprite{};
    // if sprite rotates
    std::vector<rg::math::Vector2<int>> viewing_angles{};
    // y offset
    float shift{};
    // size
    float scale{};
    // animation sprites
    std::deque<rg::Surface> animation{};
    // minimum distance to animate
    int animation_dist{};
    // animation speed
    int animation_speed{};
};

class SpriteObject
{
public:

    SpriteObject(SpriteParameter *parameter, rg::math::Vector2<float> pos);

    SpriteObjectLocate object_locate(const Player *player, float dt);

private:

    rg::Surface *object{};
    rg::math::Vector2<float> pos{};
    float x{};
    float y{};

    SpriteParameter *parameter{};
    float animation_index{};

    // min/max
    std::vector<rg::math::Vector2<int>> sprite_angles{};
    std::unordered_map<rg::math::Vector2<int>, rg::Surface *> sprite_positions{};
};

class Sprites
{
public:

    Sprites();

    std::unordered_map<std::string, SpriteParameter> sprite_parameters{};
    std::vector<SpriteObject> list_of_objects{};
};
