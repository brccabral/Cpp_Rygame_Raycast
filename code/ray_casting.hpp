#pragma once
#include <rygame.hpp>

#include "player.hpp"
#include "settings.hpp"

template<typename N>
rg::math::Vector2 mapping(N a, N b)
{
    int xi = int(a / Settings::GetInstance()->tile) * Settings::GetInstance()->tile;
    int yi = int(b / Settings::GetInstance()->tile) * Settings::GetInstance()->tile;
    return {xi, yi};
}

void ray_casting_distance(rg::Surface *sc, Player *player);
