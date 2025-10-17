#pragma once
#include <deque>
#include <rygame.hpp>

#include "player.hpp"
#include "settings.hpp"


enum class SpriteFlagType
{
    FLAG_DECOR = 0,
    FLAG_NPC,
    FLAG_DOOR_V,
    FLAG_DOOR_H
};

enum class SpriteStatus
{
    STATUS_ALIVE = 0,
    STATUS_DEAD,
    STATUS_IMMORTAL
};

struct SpriteParameter
{
    // base sprite
    std::vector<rg::Surface> sprite{};
    // if sprite rotates
    bool viewing_angles{};
    // y offset
    float shift{};
    // size (x, y)
    rg::math::Vector2<float> scale{};
    //
    float side{};
    // animation sprites
    std::deque<rg::Surface> animation{};
    // minimum distance to animate
    int animation_dist{};
    // animation speed
    int animation_speed{};
    // player collision
    bool blocked{};
    // sprites to play when dying
    std::vector<rg::Surface> death_animation{};
    float death_animation_speed{};
    // y position offset when dead
    float dead_shift{};
    // STATUS_ALIVE, STATUS_DEAD, STATUS_IMMORTAL
    SpriteStatus is_dead{};
    // FLAG_DECOR, FLAG_NPC, FLAG_DOOR_V, FLAG_DOOR_H
    SpriteFlagType flag_type{};
    // sprites for when npc is in action
    std::vector<rg::Surface> obj_action{};
    float obj_speed{};
};

struct SpriteProjection
{
    // distance from player
    float depth{};
    // screen dimension
    rg::math::Vector2<float> dimensions{};
    // world X
    float x{};
    // world Y
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

    bool npc_action_trigger{};
    float dead_animation_index{};
    SpriteStatus is_dead{};

    float distance_to_sprite{};

    SpriteFlagType flag{};

    float obj_speed{};

    bool deleted{};
    bool door_open_trigger{};

private:

    rg::Surface *dead_animation(float dt);
    rg::Surface *npc_in_action(float dt);
    rg::Surface *sprite_animation(float dt);
    rg::Surface *visible_sprite();
    void open_door(float dt);

    rg::Surface *object{};

    float proj_height{};
    float sprite_width{};
    float sprite_height{};
    int current_ray{};
    rg::math::Vector2<float> position{};

    SpriteParameter *parameter{};

    // min/max
    std::vector<rg::math::Vector2<int>> sprite_angles{};
    std::unordered_map<rg::math::Vector2<int>, rg::Surface *> sprite_positions{};
    double theta{};

    Settings *settings;

    float door_prev_pos{};
};

class Sprites
{
public:

    Sprites();
    SpriteProjection closest_sprite_projection() const;
    // Returns the mapping position of all closed doors. The value doesn't matter, only the key.
    std::unordered_map<rg::math::Vector2<int>, int> blocked_doors() const;

    std::unordered_map<std::string, SpriteParameter> sprite_parameters{};
    std::vector<SpriteObject> list_of_objects{};
};
