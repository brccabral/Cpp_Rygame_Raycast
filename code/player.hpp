#pragma once
#include "rygame.hpp"
#include "settings.hpp"


class Player
{
public:

    Player(const rg::math::Vector2 &pos, float angle, int speed, float rotation_speed);
    ~Player() = default;

    [[nodiscard]] rg::math::Vector2 pos() const;
    void movement(float dt, rg::Surface *sc);
    void show_map(rg::Surface *sc);

    float x{};
    float y{};
    float angle{};
    int speed{};
    float rotation_speed{};
    rg::Surface map_surface{};
    Settings *settings;

};
