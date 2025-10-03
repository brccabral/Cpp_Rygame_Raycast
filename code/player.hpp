#pragma once
#include "rygame.hpp"


class Player
{
public:

    Player(const rg::math::Vector2 &pos, float angle, int speed, float rotation_speed);
    ~Player() = default;

    [[nodiscard]] rg::math::Vector2 pos() const;
    void movement(float dt);

    float x{};
    float y{};
    float angle{};
    int speed{};
    float rotation_speed{};

};
