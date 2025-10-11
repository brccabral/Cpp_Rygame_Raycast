#pragma once
#include "maplevels.hpp"
#include "rygame.hpp"
#include "settings.hpp"


class Player
{
public:

    Player(
            const rg::math::Vector2<float> &pos, float angle, int speed, float rotation_speed,
            float mouse_sensitivity);
    ~Player() = default;

    [[nodiscard]] rg::math::Vector2<float> pos() const;
    [[nodiscard]] rg::math::Vector2<float> pos_map() const;
    void movement(float dt);

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

private:

    void keys_control(float dt);
    void mouse_control(float dt);
    void detect_collision(float dx, float dy);

};
