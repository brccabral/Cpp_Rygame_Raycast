#include "interaction.hpp"

#include "ray_casting.hpp"

bool ray_casting_npc_player(
        const float npc_x, const float npc_y, rg::math::Vector2<float> player_pos)
{
    const auto *settings = Settings::GetInstance();
    const auto *map_levels = MapLevels::GetInstance();

    auto [ox, oy] = player_pos;
    auto [xm, ym] = mapping(ox, oy);
    const auto delta_x = ox - npc_x;
    const auto delta_y = oy - npc_y;
    auto cur_angle = atan2f(delta_y, delta_x);
    cur_angle += M_PI;

    const auto sin_a = sinf(cur_angle);
    const auto cos_a = cosf(cur_angle);

    // vertical hits
    // scan to the right if cos_a greater than 0
    auto [x, dx] = cos_a >= 0
                       ? rg::math::Vector2{xm + settings->tile, 1}
                       : rg::math::Vector2{xm, -1};
    for (int i = 0; i < int(fabsf(delta_x)) / settings->tile; ++i)
    {
        const auto depth_v = (x - ox) / cos_a;
        const auto yv = oy + depth_v * sin_a;
        auto tile_v = mapping(static_cast<float>(x + dx), yv);
        if (map_levels->world_map.contains(tile_v))
        {
            return false;
        }
        x += dx * settings->tile;
    }

    // horizontal hits
    auto [y, dy] = sin_a >= 0
                       ? rg::math::Vector2{ym + settings->tile, 1}
                       : rg::math::Vector2{ym, -1};
    for (int i = 0; i < int(fabsf(delta_y)) / settings->tile; ++i)
    {
        const auto depth_h = (y - oy) / sin_a;
        const auto xh = ox + depth_h * cos_a;
        auto tile_h = mapping(xh, static_cast<float>(y + dy));
        if (map_levels->world_map.contains(tile_h))
        {
            return false;
        }
        y += dy * settings->tile;
    }

    return true;
}

Interaction::Interaction(Player *player, Sprites *sprites, Drawing *drawing)
    : player(player), sprites(sprites), drawing(drawing)
{
}

void Interaction::interation_objects() const
{
    if (player->shot && drawing->shot_animation_trigger)
    {
        std::ranges::sort(
                sprites->list_of_objects, [](const auto &lhs, const auto &rhs)
                {
                    return lhs.distance_to_sprite > rhs.distance_to_sprite;
                });
        for (auto &obj: sprites->list_of_objects)
        {
            if (obj.sprite_projection().proj_height)
            {
                if (obj.is_dead != SpriteStatus::STATUS_IMMORTAL && obj.is_dead ==
                    SpriteStatus::STATUS_ALIVE)
                {
                    if (ray_casting_npc_player(obj.x, obj.y, player->pos()))
                    {
                        obj.is_dead = SpriteStatus::STATUS_DEAD;
                        obj.blocked = false;
                        drawing->shot_animation_trigger = false;
                    }
                }
                break;
            }
        }
    }
}
