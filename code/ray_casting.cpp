#include "ray_casting.hpp"

#include "maplevels.hpp"
#include "settings.hpp"


void ray_casting_distance(rg::Surface *sc, Player *player)
{
    auto cur_angle = player->angle - Settings::GetInstance()->half_fov;
    auto [xo, yo] = player->pos();
    for (int ray = 0; ray < Settings::GetInstance()->num_rays; ++ray)
    {
        const auto sin_a = sinf(cur_angle);
        const auto cos_a = cosf(cur_angle);
        for (int depth = 0; depth < Settings::GetInstance()->max_depth; ++depth)
        {
            auto x = xo + depth * cos_a;
            auto y = yo + depth * sin_a;
            // stop ray casting if ray touches a wall
            if (MapLevels::GetInstance()->world_map.find(mapping(x, y)) !=
                MapLevels::GetInstance()->world_map.end())
            {
                // show ray line when they hit a wall
                rg::draw::line(
                        &player->map_surface, Settings::GetInstance()->darkgray, player->pos(),
                        {x, y}, 2);
                // remove fish eye effect
                depth *= cosf(player->angle - cur_angle);
                auto proj_height = Settings::GetInstance()->proj_coeff / depth;
                // closer walls are brighter
                const unsigned char c = 255 / (1 + depth * depth * 0.0001);
                const auto color = rl::Color{c, static_cast<unsigned char>(c / 2),
                                             static_cast<unsigned char>(c / 3), 255};
                rg::draw::rect(
                        sc, color, {
                                static_cast<float>(ray * Settings::GetInstance()->scale),
                                Settings::GetInstance()->half_height - proj_height / 2.0f,
                                static_cast<float>(Settings::GetInstance()->scale),
                                proj_height
                        });
                break;
            }
        }
        cur_angle += Settings::GetInstance()->delta_angle;
    }
}
