#include "maplevels.hpp"
#include "settings.hpp"

MapLevels *MapLevels::instance = nullptr;

MapLevels::MapLevels()
{
    const Settings *settings = Settings::GetInstance();

    for (size_t r = 0; r < matrix_map.size(); ++r)
    {
        auto row = matrix_map[r];
        for (size_t c = 0; c < row.size(); ++c)
        {
            if (const auto mc = row[c]; mc != 0)
            {
                world_map[{static_cast<int>(c) * settings->tile,
                           static_cast<int>(r) * settings->tile}] = mc;
                mini_map[{static_cast<int>(c) * settings->map_tile,
                          static_cast<int>(r) * settings->map_tile}] = mc;
                collision_walls.push_back(
                {static_cast<int>(c) * settings->tile, static_cast<int>(r) * settings->tile,
                 settings->tile, settings->tile});
            }
        }
    }
}

MapLevels *MapLevels::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new MapLevels();
    }
    return instance;
}

void MapLevels::Destroy()
{
    delete instance;
}
