#pragma once
#include <rygame.hpp>


class MapLevels
{
public:

    static MapLevels *GetInstance();
    static void Destroy();

    std::unordered_map<rg::math::Vector2<int>, int> world_map{};
    std::unordered_map<rg::math::Vector2<int>, int> mini_map{};

private:

    MapLevels();
    ~MapLevels() = default;
    static MapLevels *instance;

    std::array<std::array<int, 12>, 8> text_map = {{
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1},
            {1, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 1},
            {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    }};
};
