#pragma once
#include "rygame.hpp"


class MapLevels
{
public:

    static MapLevels *GetInstance();
    static void Destroy();

    rg::InsertOrderMap<rg::math::Vector2, int> world_map{};

private:

    MapLevels();
    ~MapLevels() = default;
    static MapLevels *instance;

    std::array<std::array<char, 12>, 8> text_map = {{
            {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
            {'W', '.', '.', '.', '.', '.', 'W', '.', '.', '.', '.', 'W'},
            {'W', '.', '.', 'W', 'W', 'W', '.', '.', '.', 'W', '.', 'W'},
            {'W', '.', '.', '.', '.', 'W', '.', '.', 'W', 'W', '.', 'W'},
            {'W', '.', '.', 'W', '.', '.', '.', '.', 'W', '.', '.', 'W'},
            {'W', '.', '.', 'W', '.', '.', '.', 'W', 'W', 'W', '.', 'W'},
            {'W', '.', '.', '.', '.', 'W', '.', '.', '.', '.', '.', 'W'},
            {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'}
    }};
};
