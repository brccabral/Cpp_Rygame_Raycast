#include "maplevels.hpp"
#include "settings.hpp"

MapLevels *MapLevels::instance = nullptr;

MapLevels::MapLevels()
{
    for (size_t r = 0; r < text_map.size(); ++r)
    {
        auto row = text_map[r];
        for (size_t c = 0; c < row.size(); ++c)
        {
            if (const auto mc = row[c]; mc != 0)
            {
                world_map[{(int) c * Settings::GetInstance()->tile,
                           (int) r * Settings::GetInstance()->tile}] = mc;
                mini_map[{(int) c * Settings::GetInstance()->map_tile,
                          (int) r * Settings::GetInstance()->map_tile}] = mc;
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
