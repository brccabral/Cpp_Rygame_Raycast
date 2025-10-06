#include "ray_casting.hpp"

#include "maplevels.hpp"
#include "settings.hpp"


void ray_casting_distance(
        rg::Surface *sc, rg::Surface *sc_map, const Player *player, const Settings *settings)
{
    auto cur_angle = player->angle - settings->half_fov;
    auto [xo, yo] = player->pos();
    for (int ray = 0; ray < settings->num_rays; ++ray)
    {
        const auto sin_a = sinf(cur_angle);
        const auto cos_a = cosf(cur_angle);
        for (int depth = 0; depth < settings->max_depth; ++depth)
        {
            auto x = xo + depth * cos_a;
            auto y = yo + depth * sin_a;
            // stop ray casting if ray touches a wall
            if (MapLevels::GetInstance()->world_map.contains(mapping(x, y)))
            {
                // show ray line when they hit a wall
                rg::draw::line(
                        sc_map, settings->raycolor, player->pos_map(),
                        {x / settings->map_scale,
                         y / settings->map_scale}, 2);
                // remove fish eye effect
                depth *= cosf(player->angle - cur_angle);
                auto proj_height = settings->proj_coeff / depth;
                // closer walls are brighter
                const unsigned char c = 255 / (1 + depth * depth * 0.0001);
                const auto color = rl::Color{c, static_cast<unsigned char>(c / 2),
                                             static_cast<unsigned char>(c / 3), 255};
                rg::draw::rect(
                        sc, color, {
                                static_cast<float>(ray * settings->scale),
                                settings->half_height - proj_height / 2.0f,
                                static_cast<float>(settings->scale),
                                proj_height
                        });
                break;
            }
        }
        cur_angle += settings->delta_angle;
    }
}

void ray_casting_depth(
        rg::Surface *sc, rg::Surface *sc_map, const Player *player, const Settings *settings)
{
    auto [ox, oy] = player->pos();
    auto [xm, ym] = mapping(ox, oy);
    auto cur_angle = player->angle - settings->half_fov;
    auto [ray_x, ray_y] = player->pos();

    int ray_x_v = xm, ray_x_h = xm;
    int ray_y_v = ym, ray_y_h = ym;
    float depth = 0, depth_v = 0, depth_h = 0;

    for (int ray = 0; ray < settings->num_rays; ++ray)
    {
        const auto sin_a = sinf(cur_angle);
        const auto cos_a = cosf(cur_angle);

        // vertical hits
        // scan to the right if cos_a greater than 0
        auto [x, dx] = cos_a >= 0
                           ? rg::math::Vector2{xm + settings->tile, 1}
                           : rg::math::Vector2{xm, -1};
        for (int i = 0; i < settings->width; i += settings->tile)
        {
            depth_v = (x - ox) / cos_a;
            const auto depth_y = oy + depth_v * sin_a;
            auto loc = mapping(static_cast<float>(x + dx), depth_y);
            if (MapLevels::GetInstance()->world_map.contains(loc))
            {
                ray_x_v = x;
                ray_y_v = depth_y;
                break;
            }
            x += dx * settings->tile;
        }

        // horizontal hits
        auto [y, dy] = sin_a >= 0
                           ? rg::math::Vector2{ym + settings->tile, 1}
                           : rg::math::Vector2{ym, -1};
        for (int i = 0; i < settings->height; i += settings->tile)
        {
            depth_h = (y - oy) / sin_a;
            const auto depth_x = ox + depth_h * cos_a;
            auto loc = mapping(depth_x, static_cast<float>(y + dy));
            if (MapLevels::GetInstance()->world_map.contains(loc))
            {
                ray_x_h = depth_x;
                ray_y_h = y;
                break;
            }
            y += dy * settings->tile;
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
                sc_map, settings->raycolor, player->pos_map(),
                {ray_x / settings->map_scale,
                 ray_y / settings->map_scale}, 2);
        // remove fish eye effect
        depth *= cosf(player->angle - cur_angle);
        // project wall
        auto proj_height = settings->proj_coeff / depth;
        // closer walls are brighter
        const unsigned char c = 255 / (1 + depth * depth * 0.0001);
        const auto color = rl::Color{c, static_cast<unsigned char>(c / 2),
                                     static_cast<unsigned char>(c / 3), 255};
        rg::draw::rect(
                sc, color, {
                        static_cast<float>(ray * settings->scale),
                        settings->half_height - proj_height / 2.0f,
                        static_cast<float>(settings->scale),
                        proj_height
                });
        cur_angle += settings->delta_angle;
    }
}
