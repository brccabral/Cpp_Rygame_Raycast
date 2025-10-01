#pragma once
#include "rygame.hpp"


class Player
{
public:

    Player();
    ~Player() = default;

    rg::math::Vector2 pos();
    void movement(float dt);

    float x{};
    float y{};
    float angle{};

};
