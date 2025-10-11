#pragma once
#include <rygame.hpp>

#include "player.hpp"
#include "settings.hpp"


class SpriteObject
{
public:

    SpriteObject(
            rg::Surface *object, bool is_static, rg::math::Vector2<float> pos, float shift,
            float scale);

    SpriteObjectLocate object_locate(
            const Player *player, const Settings *settings,
            const std::vector<SpriteObjectLocate> &walls) const;

private:

    rg::Surface *object{};
    bool is_static{};
    rg::math::Vector2<float> pos{};
    float x{};
    float y{};
    float shift{};
    float scale{};
};

class Sprites
{
public:

    Sprites();

    std::unordered_map<std::string, rg::Surface> sprite_types{};
    std::vector<SpriteObject> list_of_objects{};
};
