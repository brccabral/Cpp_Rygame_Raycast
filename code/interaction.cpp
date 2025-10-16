#include "interaction.hpp"

#include "ray_casting.hpp"

bool ray_casting_npc_player(
        const float npc_x, const float npc_y, rg::math::Vector2<float> player_pos,
        std::unordered_map<rg::math::Vector2<int>, int> blocked_doors)
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
        if (map_levels->world_map.contains(tile_v) || blocked_doors.contains(tile_v))
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
        if (map_levels->world_map.contains(tile_h) || blocked_doors.contains(tile_h))
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
    pain_sound = rg::mixer::Sound("resources/sound/pain.wav");
}

void Interaction::interation_objects() const
{
    if (player->shot && drawing->shot_animation_trigger)
    {
        std::ranges::sort(
                sprites->list_of_objects, [](const auto &lhs, const auto &rhs)
                {
                    return lhs.distance_to_sprite < rhs.distance_to_sprite;
                });
        for (auto &obj: sprites->list_of_objects)
        {
            if (obj.sprite_projection().dimensions.x)
            {
                if (obj.is_dead == SpriteStatus::STATUS_ALIVE)
                {
                    if (ray_casting_npc_player(
                            obj.x, obj.y, player->pos(), sprites->blocked_doors()))
                    {
                        obj.is_dead = SpriteStatus::STATUS_DEAD;
                        obj.blocked = false;
                        drawing->shot_animation_trigger = false;
                        if (obj.flag == SpriteFlagType::FLAG_NPC)
                        {
                            pain_sound.Play();
                        }
                    }
                }

                // if player is close to the door and shoots, door is triggered
                if ((obj.flag == SpriteFlagType::FLAG_DOOR_H
                     || obj.flag == SpriteFlagType::FLAG_DOOR_V)
                    && obj.distance_to_sprite < Settings::GetInstance()->tile)
                {
                    obj.door_open_trigger = true;
                    obj.blocked = false;
                }
                break;
            }
        }
    }
}

void Interaction::npc_action(const float dt) const
{
    // npc only triggers action if it is in player's line of sight
    for (auto &obj: sprites->list_of_objects)
    {
        if (obj.flag == SpriteFlagType::FLAG_NPC && obj.is_dead != SpriteStatus::STATUS_DEAD)
        {
            if (ray_casting_npc_player(obj.x, obj.y, player->pos(), sprites->blocked_doors()))
            {
                obj.npc_action_trigger = true;
                npc_move(&obj, dt);
            }
            else
            {
                obj.npc_action_trigger = false;
            }
        }
    }
}

void Interaction::clear_world() const
{
    std::erase_if(
            sprites->list_of_objects, [](const auto &obj)
            {
                return obj.deleted;
            });
}

void Interaction::npc_move(SpriteObject *obj, const float dt) const
{
    // npc moves towards player, function triggered only if
    // npc is in player's line of sight
    if (obj->distance_to_sprite > Settings::GetInstance()->tile)
    {
        const auto dx = obj->x - player->pos().x;
        const auto dy = obj->y - player->pos().y;
        obj->x = dx < 0 ? obj->x + dt * obj->obj_speed : obj->x - dt * obj->obj_speed;
        obj->y = dy < 0 ? obj->y + dt * obj->obj_speed : obj->y - dt * obj->obj_speed;
    }
}
