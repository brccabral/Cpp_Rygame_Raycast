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
                        &player->map_surface, Settings::GetInstance()->raycolor, player->pos(),
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

void ray_casting_depth(rg::Surface *sc, Player *player)
{
    auto [ox, oy] = player->pos();
    auto [xm, ym] = mapping(ox, oy);
    auto cur_angle = player->angle - Settings::GetInstance()->half_fov;
    auto [ray_x, ray_y] = player->pos();

    int ray_x_v = xm, ray_x_h = xm;
    int ray_y_v = ym, ray_y_h = ym;
    float depth = 0, depth_v = 0, depth_h = 0;

    for (int ray = 0; ray < Settings::GetInstance()->num_rays; ++ray)
    {
        const auto sin_a = sinf(cur_angle);
        const auto cos_a = cosf(cur_angle);

        // vertical hits
        // scan to the right if cos_a greater than 0
        auto [x, dx] = cos_a >= 0
                           ? rg::math::Vector2{xm + Settings::GetInstance()->tile, 1}
                           : rg::math::Vector2{xm, -1};
        for (int i = 0; i < Settings::GetInstance()->width; i += Settings::GetInstance()->tile)
        {
            depth_v = (x - ox) / cos_a;
            const auto depth_y = oy + depth_v * sin_a;
            auto loc = mapping(static_cast<float>(x + dx), depth_y);
            if (MapLevels::GetInstance()->world_map.find(loc) !=
                MapLevels::GetInstance()->world_map.end())
            {
                ray_x_v = x;
                ray_y_v = depth_y;
                break;
            }
            x += dx * Settings::GetInstance()->tile;
        }

        // horizontal hits
        auto [y, dy] = sin_a >= 0
                           ? rg::math::Vector2{ym + Settings::GetInstance()->tile, 1}
                           : rg::math::Vector2{ym, -1};
        for (int i = 0; i < Settings::GetInstance()->height; i += Settings::GetInstance()->tile)
        {
            depth_h = (y - oy) / sin_a;
            const auto depth_x = ox + depth_h * cos_a;
            auto loc = mapping(depth_x, static_cast<float>(y + dy));
            if (MapLevels::GetInstance()->world_map.find(loc) !=
                MapLevels::GetInstance()->world_map.end())
            {
                ray_x_h = depth_x;
                ray_y_h = y;
                break;
            }
            y += dy * Settings::GetInstance()->tile;
        }

        // projection
        if (depth_v < depth_h)
        {
            depth = depth_v;
            ray_x = ray_x_v;
            ray_y = ray_y_v;
        }
        else
        {
            depth = depth_h;
            ray_x = ray_x_h;
            ray_y = ray_y_h;
        }

        // show only rays when they hit wall
        rg::draw::line(
                &player->map_surface, Settings::GetInstance()->raycolor, player->pos(),
                {ray_x, ray_y}, 2);
        // remove fish eye effect
        depth *= cosf(player->angle - cur_angle);
        // project wall
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
        cur_angle += Settings::GetInstance()->delta_angle;
    }
}
