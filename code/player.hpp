#pragma once
#include "rygame.hpp"
#include "settings.hpp"


class Player
{
public:

    Player(const rg::math::Vector2<float> &pos, float angle, int speed, float rotation_speed);
    ~Player() = default;

    [[nodiscard]] rg::math::Vector2<float> pos() const;
    [[nodiscard]] rg::math::Vector2<float> pos_map() const;
    void movement(float dt, rg::Surface *sc);

    float x{};
    float y{};
    float angle{};
    int speed{};
    float rotation_speed{};
    Settings *settings;

};
