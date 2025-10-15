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
    bool viewing_angles{};
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
    // player collision
    bool blocked{};
};

struct SpriteProjection
{
    float depth{};
    float proj_height{};
    float x{};
    float y{};
};

class SpriteObject
{
public:

    SpriteObject(SpriteParameter *parameter, rg::math::Vector2<float> pos);

    SpriteObjectLocate object_locate(const Player *player, float dt);
    rg::math::Vector2<float> pos() const;
    // Get sprite projection distance and height if sprite is on field of view. Else, returns infinity
    SpriteProjection sprite_projection() const;

    bool blocked{};
    float side = 30.0f;
    float x{};
    float y{};
    float animation_index{};
    float animation_speed{};

private:

    rg::Surface *object{};

    float distance_to_sprite{};
    float proj_height{};
    int current_ray{};
    rg::math::Vector2<float> position{};

    SpriteParameter *parameter{};

    // min/max
    std::vector<rg::math::Vector2<int>> sprite_angles{};
    std::unordered_map<rg::math::Vector2<int>, rg::Surface *> sprite_positions{};

    Settings *settings;
};

class Sprites
{
public:

    Sprites();
    SpriteProjection closest_sprite_projection() const;

    std::unordered_map<std::string, SpriteParameter> sprite_parameters{};
    std::vector<SpriteObject> list_of_objects{};
};
