#pragma once
#include <rygame.hpp>

#include "player.hpp"
#include "settings.hpp"


class SpriteObject
{
public:

    SpriteObject(
            std::vector<rg::Surface> *objects, bool is_static, rg::math::Vector2<float> pos,
            float shift, float scale);

    SpriteObjectLocate object_locate(
            const Player *player, const Settings *settings,
            const std::vector<SpriteObjectLocate> &walls);

private:

    std::vector<rg::Surface> *objects{};
    bool is_static{};
    rg::math::Vector2<float> pos{};
    float x{};
    float y{};
    float shift{};
    float scale{};

    rg::Surface *current_object{};

    // min/max
    std::vector<rg::math::Vector2<int>> sprite_angles{};
    std::unordered_map<rg::math::Vector2<int>, rg::Surface *> sprite_positions{};
};

class Sprites
{
public:

    Sprites();

    std::unordered_map<std::string, std::vector<rg::Surface>> sprite_types{};
    std::vector<SpriteObject> list_of_objects{};
};
