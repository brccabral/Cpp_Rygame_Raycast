#pragma once
#include <rygame.hpp>
#include "maplevels.hpp"
#include "settings.hpp"


class Sprites;

class Player
{
public:

    Player(
            Sprites *sprites, const rg::math::Vector2<float> &pos, float angle, int speed,
            float rotation_speed, float mouse_sensitivity);
    ~Player() = default;

    [[nodiscard]] rg::math::Vector2<float> pos() const;
    [[nodiscard]] rg::math::Vector2<float> pos_map() const;
    void movement(float dt);

    void shoot();

    float x{};
    float y{};
    float angle{};
    int speed{};
    float rotation_speed{};
    float mouse_sensitivity{};
    Settings *settings{};
    MapLevels *map_levels{};
    float side = 50;
    rg::Rect rect{x, y, side, side};

    // weapon
    bool shot{};

private:

    void keys_control(float dt);
    void mouse_control(float dt);
    void detect_collision(float dx, float dy);

    Sprites *sprites{};
    std::vector<rg::Rect> collision_list{};

};
