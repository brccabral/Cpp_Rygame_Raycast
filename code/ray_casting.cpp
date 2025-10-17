#include "ray_casting.hpp"

#include "maplevels.hpp"
#include "settings.hpp"


void ray_casting_distance(
        rg::Surface *sc, rg::Surface *sc_map, const Player *player,
        std::unordered_map<int, rg::Surface> *textures)
{
    const Settings *settings = Settings::GetInstance();
    const MapLevels *map_levels = MapLevels::GetInstance();

    auto cur_angle = player->angle - settings->half_fov;
    auto [xo, yo] = player->pos();
    for (int ray = 0; ray < settings->num_rays; ++ray)
    {
        const auto sin_a = sinf(cur_angle);
        const auto cos_a = cosf(cur_angle);
        float depth = 0;
        for (int d = 0; d < settings->max_depth; depth += 1.0f, d = depth)
        {
            const auto x = xo + depth * cos_a;
            const auto y = yo + depth * sin_a;
            // stop ray casting if ray touches a wall
            if (map_levels->world_map.contains(mapping(x, y)))
            {
                // show ray line when they hit a wall
                rg::draw::line(
                        sc_map, settings->raycolor, player->pos_map(),
                        {x / settings->map_scale,
                         y / settings->map_scale}, 2);
                // remove fish eye effect
                depth *= cosf(player->angle - cur_angle);
                depth = std::max(depth, 0.00001f);
                const auto proj_height = settings->proj_coeff / depth;

                auto [xm, ym] = mapping(x, y);
                const auto dif_x = cos_a >= 0 ? x - xm : settings->tile - (x - xm);
                const auto dif_y = sin_a >= 0 ? y - ym : settings->tile - (y - ym);

                const auto depth_v = (x - xo) / cos_a;
                auto yv = yo + depth_v * sin_a;

                const auto depth_h = (y - yo) / sin_a;
                auto xh = xo + depth_h * cos_a;

                auto offset = dif_x < dif_y ? yv : xh;
                offset = int(offset) % settings->tile;

                auto *texture = &textures->at(map_levels->world_map.at({xm, ym}));

                sc->Blit(
                        texture,
                        rg::math::Vector2
                        {ray * settings->scale, settings->half_height - int(proj_height / 2)},
                        {offset * settings->texture_scale, 0.0f,
                         static_cast<float>(settings->texture_scale),
                         static_cast<float>(settings->texture_height)}, rl::BLEND_ALPHA,
                        settings->scale, proj_height);

                break;
            }
        }
        cur_angle += settings->delta_angle;
    }
}

std::vector<SpriteObjectLocate> ray_casting_depth(
        rg::Surface *sc_map, const Player *player, std::unordered_map<int, rg::Surface> *textures)
{
    const Settings *settings = Settings::GetInstance();
    const MapLevels *map_levels = MapLevels::GetInstance();

    std::vector<SpriteObjectLocate> walls{};
    walls.reserve(settings->num_rays);

    auto [ox, oy] = player->pos();
    auto [xm, ym] = mapping(ox, oy);
    auto cur_angle = player->angle - settings->half_fov;

    // rays of field of view on minimap
    // game won't crash if player goes out of map
    auto [ray_x_v, ray_y_v] = player->pos();
    auto ray_x_h = ray_x_v;
    auto ray_y_h = ray_y_v;
    float ray_x, ray_y;

    float depth = 0, depth_v = 0, depth_h = 0;

    float xh = 0, yv = 0, offset = 0;

    int texture{};
    int texture_v = 1;
    int texture_h = 1;

    for (int ray = 0; ray < settings->num_rays; ++ray)
    {
        const auto sin_a = sinf(cur_angle);
        const auto cos_a = cosf(cur_angle);

        // vertical hits
        // scan to the right if cos_a greater than 0
        auto [x, dx] = cos_a >= 0
                           ? rg::math::Vector2{xm + settings->tile, 1}
                           : rg::math::Vector2{xm, -1};
        for (int i = 0; i < map_levels->world_width; i += settings->tile)
        {
            depth_v = (x - ox) / cos_a;
            yv = oy + depth_v * sin_a;
            auto tile_v = mapping(static_cast<float>(x + dx), yv);
            if (map_levels->world_map.contains(tile_v))
            {
                ray_x_v = x;
                ray_y_v = yv;
                texture_v = map_levels->world_map.at(tile_v);
                break;
            }
            x += dx * settings->tile;
        }

        // horizontal hits
        auto [y, dy] = sin_a >= 0
                           ? rg::math::Vector2{ym + settings->tile, 1}
                           : rg::math::Vector2{ym, -1};
        for (int i = 0; i < map_levels->world_height; i += settings->tile)
        {
            depth_h = (y - oy) / sin_a;
            xh = ox + depth_h * cos_a;
            auto tile_h = mapping(xh, static_cast<float>(y + dy));
            if (map_levels->world_map.contains(tile_h))
            {
                ray_x_h = xh;
                ray_y_h = y;
                texture_h = map_levels->world_map.at(tile_h);
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
            offset = yv;
            texture = texture_v;
        }
        else
        {
            depth = depth_h;
            ray_x = ray_x_h;
            ray_y = ray_y_h;
            offset = xh;
            texture = texture_h;
        }
        offset = int(offset) % settings->tile;

        // show only rays when they hit wall
        rg::draw::line(
                sc_map, settings->raycolor, player->pos_map(),
                {ray_x / settings->map_scale,
                 ray_y / settings->map_scale}, 2);
        // remove fish eye effect
        depth *= cosf(player->angle - cur_angle);
        depth = std::max(depth, 0.00001f);
        // project wall, limit rect height
        const auto proj_height = std::min(settings->proj_coeff / depth, settings->penta_height);

        walls.emplace_back(
                depth, &(*textures)[texture],
                rg::math::Vector2{static_cast<float>(settings->scale), proj_height},
                rg::math::Vector2{static_cast<float>(ray * settings->scale),
                                  settings->half_height - proj_height / 2.0f},
                rg::Rect{offset * settings->texture_scale, 0.0f,
                         static_cast<float>(settings->texture_scale),
                         -static_cast<float>(settings->texture_height)});
        cur_angle += settings->delta_angle;
    }

    return walls;
}
