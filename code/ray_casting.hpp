#pragma once
#include <rygame.hpp>

#include "maplevels.hpp"
#include "player.hpp"
#include "settings.hpp"

template<typename N>
rg::math::Vector2<int> mapping(N a, N b)
{
    int xi = int(int(a / Settings::GetInstance()->tile) * Settings::GetInstance()->tile);
    int yi = int(int(b / Settings::GetInstance()->tile) * Settings::GetInstance()->tile);
    return {xi, yi};
}

void ray_casting_distance(
        rg::Surface *sc, rg::Surface *sc_map, const Player *player,
        std::unordered_map<int, rg::Surface> *textures);
std::vector<SpriteObjectLocate> ray_casting_depth(
        rg::Surface *sc_map, const Player *player, std::unordered_map<int, rg::Surface> *textures);
