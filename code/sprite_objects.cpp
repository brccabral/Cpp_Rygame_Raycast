#include <limits>
#include "sprite_objects.hpp"
#include "ray_casting.hpp"


SpriteObject::SpriteObject(SpriteParameter *parameter, const rg::math::Vector2<float> pos)
    : parameter(parameter), settings(Settings::GetInstance())
{
    blocked = parameter->blocked;

    x = pos.x * settings->tile;
    y = pos.y * settings->tile;

    object = &parameter->sprite[0];

    if (parameter->viewing_angles)
    {
        if (parameter->sprite.size() == 8)
        {
            sprite_angles.emplace_back(338, 361);
            sprite_positions[sprite_angles[0]] = &parameter->sprite[0];
            sprite_angles.emplace_back(0, 23);
            sprite_positions[sprite_angles[1]] = &parameter->sprite[0];
            for (int i = 23; i < 338; i += 45)
            {
                sprite_angles.emplace_back(i, i + 45);
            }
            for (int i = 2; i < sprite_angles.size(); ++i)
            {
                sprite_positions[sprite_angles[i]] = &parameter->sprite[i - 1];
            }
        }
        else
        // doors have 16 viewing angles
        {
            sprite_angles.emplace_back(348, 361);
            sprite_positions[sprite_angles[0]] = &parameter->sprite[0];
            sprite_angles.emplace_back(0, 11);
            sprite_positions[sprite_angles[1]] = &parameter->sprite[0];
            for (int i = 11; i < 348; i += 23)
            {
                sprite_angles.emplace_back(i, i + 23);
            }
            for (int i = 2; i < sprite_angles.size(); ++i)
            {
                sprite_positions[sprite_angles[i]] = &parameter->sprite[i - 1];
            }
        }
    }

    animation_speed = parameter->animation_speed;
    is_dead = parameter->is_dead;

    flag = parameter->flag_type;
    obj_speed = parameter->obj_speed;

    door_prev_pos = flag == SpriteFlagType::FLAG_DOOR_H ? y : x;
}

SpriteObjectLocate SpriteObject::object_locate(const Player *player, const float dt)
{
    const auto dx = x - player->x;
    const auto dy = y - player->y;
    distance_to_sprite = std::sqrt(dx * dx + dy * dy);

    // theta = sprite to player horizontal
    theta = atan2(dy, dx);
    // gamma = angle between sprite and player direction
    auto gamma = theta - player->angle;

    // work with positive angles
    const auto degrees = player->angle * 180.0f / M_PI;
    if (dx > 0.0f && 180.0f <= degrees && degrees <= 360.0f || (dx < 0.0f && dy < 0.0f))
    {
        gamma += settings->double_pi;
    }

    // shifted viewing angles to compensate sprites positions
    theta -= 1.4f * gamma;

    // delta_rays = number of rays between player direction and sprite
    const auto delta_rays = int(gamma / settings->delta_angle);
    current_ray = settings->center_ray + delta_rays;

    // fix fish eye
    if (flag != SpriteFlagType::FLAG_DOOR_H && flag != SpriteFlagType::FLAG_DOOR_V)
    {
        distance_to_sprite *= cosf(settings->half_fov - current_ray * settings->delta_angle);
    }

    // FAKE_RAYS_RANGE increase rays on the sides to avoid a
    // sprite disappearing when close to the border of the screen
    const auto fake_ray = current_ray + settings->fake_rays;
    // check if sprite is inside "field of view" and
    // if it is more than 30 px of distance to avoid
    // dropping frame rate when player is too close
    // of a sprite, or a sprite is half-way behind a wall
    if (0 <= fake_ray && fake_ray <= settings->fake_rays_range && distance_to_sprite > 30)
    {
        proj_height = std::min(
                int(settings->proj_coeff / distance_to_sprite),
                settings->double_height);

        sprite_width = proj_height * parameter->scale.x;
        sprite_height = proj_height * parameter->scale.y;
        const auto half_sprite_width = sprite_width / 2.0f;
        const auto half_sprite_height = sprite_height / 2.0f;

        auto shift = half_sprite_height * parameter->shift;

        if (flag == SpriteFlagType::FLAG_DOOR_H || flag == SpriteFlagType::FLAG_DOOR_V)
        {
            if (door_open_trigger)
            {
                open_door(dt);
            }
            object = visible_sprite();
            object = sprite_animation(dt);
        }
        else if (is_dead == SpriteStatus::STATUS_DEAD)
        {
            object = dead_animation(dt);
            shift = half_sprite_height * parameter->dead_shift;
        }
        else if (npc_action_trigger)
        {
            object = npc_in_action(dt);
        }
        else
        {
            object = visible_sprite();
            object = sprite_animation(dt);
        }

        position = {current_ray * settings->scale - half_sprite_width,
                    settings->half_height - half_sprite_height + shift};

        return {.depth = distance_to_sprite, .sprite = object,
                .sprite_dimension = {sprite_width, sprite_height},
                .sprite_pos = position, .sprite_area = {}, .x = x, .y = y};
    }
    return {};
}

rg::math::Vector2<float> SpriteObject::pos() const
{
    return {x - side / 2, y - side / 2};
}

SpriteProjection SpriteObject::sprite_projection() const
{
    if (settings->center_ray - side / 2 < current_ray
        && current_ray < settings->center_ray + side / 2
        && parameter->blocked)
    {
        return {.depth = distance_to_sprite, .dimensions = {sprite_width, sprite_height}, .x = x,
                .y = y, .pos = position};
    }
    return {.depth = std::numeric_limits<float>::infinity(), .dimensions = {}, .x = 0, .y = 0,
            .pos = {}};
}

rg::Surface *SpriteObject::dead_animation(const float dt)
{
    rg::Surface *result = nullptr;
    if (!parameter->death_animation.empty())
    {
        if (dead_animation_index < parameter->death_animation.size())
        {
            result = &parameter->death_animation[int(dead_animation_index)];
            dead_animation_index += dt * parameter->death_animation_speed;
            if (dead_animation_index >= parameter->death_animation.size())
            {
                dead_animation_index = parameter->death_animation.size() - 1;
            }
        }
    }
    return result;
}

rg::Surface *SpriteObject::npc_in_action(const float dt)
{
    rg::Surface *result = &parameter->obj_action[int(animation_index)];
    animation_index += dt * animation_speed;
    if (animation_index >= parameter->obj_action.size())
    {
        animation_index = 0;
    }
    return result;
}

rg::Surface *SpriteObject::sprite_animation(const float dt)
{
    if (!parameter->animation.empty() && distance_to_sprite < parameter->animation_dist)
    {
        animation_index += dt * animation_speed;
        if (animation_index >= parameter->animation.size())
        {
            animation_index = 0;
        }
        return &parameter->animation[int(animation_index)];
    }
    return object;
}

rg::Surface *SpriteObject::visible_sprite()
{
    if (parameter->viewing_angles)
    {
        if (theta < 0)
        {
            theta += settings->double_pi;
        }
        theta = 360 - int(theta * 180.f / M_PI);
        for (const auto &angles: sprite_angles)
        {
            if (theta >= angles.x && theta < angles.y)
            {
                return sprite_positions[angles];
            }
        }
    }
    return object;
}

void SpriteObject::open_door(const float dt)
{
    if (flag == SpriteFlagType::FLAG_DOOR_H)
    {
        y -= animation_speed * dt;
        if (fabsf(y - door_prev_pos) > settings->tile)
        {
            deleted = true;
        }
    }
    else if (flag == SpriteFlagType::FLAG_DOOR_V)
    {
        x -= animation_speed * dt;
        if (fabsf(x - door_prev_pos) > settings->tile)
        {
            deleted = true;
        }
    }
}

Sprites::Sprites()
{
    SpriteParameter sprite_barrel_params{};
    sprite_barrel_params.sprite.emplace_back(
            rg::image::Load("resources/sprites/barrel/base/0.png"));
    sprite_barrel_params.shift = 1.8f;
    sprite_barrel_params.scale = {0.4f, 0.4f};
    sprite_barrel_params.side = 30;
    for (int i = 0; i < 13; ++i)
    {
        std::string path = std::string("resources/sprites/barrel/anim/") + std::to_string(i) +
                           ".png";
        sprite_barrel_params.animation.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_barrel_params.animation_dist = 800;
    sprite_barrel_params.animation_speed = 10;
    sprite_barrel_params.blocked = true;
    for (int i = 0; i < 4; ++i)
    {
        std::string path = std::string("resources/sprites/barrel/death/") + std::to_string(i) +
                           ".png";
        sprite_barrel_params.death_animation.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_barrel_params.death_animation_speed = 10;
    sprite_barrel_params.dead_shift = 2.6f;
    sprite_parameters["sprite_barrel"] = std::move(sprite_barrel_params);

    SpriteParameter sprite_pin_params{};
    sprite_pin_params.sprite.emplace_back(
            rg::image::Load("resources/sprites/pin/base/0.png"));
    sprite_pin_params.shift = 0.6f;
    sprite_pin_params.scale = {0.6f, 0.6f};
    sprite_pin_params.side = 30;
    for (int i = 0; i < 8; ++i)
    {
        std::string path = std::string("resources/sprites/pin/anim/") + std::to_string(i) +
                           ".png";
        sprite_pin_params.animation.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_pin_params.animation_dist = 800;
    sprite_pin_params.animation_speed = 10;
    sprite_pin_params.blocked = true;
    sprite_pin_params.is_dead = SpriteStatus::STATUS_IMMORTAL;
    sprite_parameters["sprite_pin"] = std::move(sprite_pin_params);

    SpriteParameter sprite_devil_params{};
    for (int i = 0; i < 8; ++i)
    {
        std::string path = std::string("resources/sprites/devil/base/") + std::to_string(i) +
                           ".png";
        sprite_devil_params.sprite.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_devil_params.viewing_angles = true;
    sprite_devil_params.shift = 0.2f;
    sprite_devil_params.scale = {1.1f, 1.1f};
    sprite_devil_params.side = 50;
    for (int i = 0; i < 9; ++i)
    {
        std::string path = std::string("resources/sprites/devil/anim/") + std::to_string(i) +
                           ".png";
        sprite_devil_params.obj_action.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_devil_params.animation_dist = 200;
    sprite_devil_params.animation_speed = 10;
    sprite_devil_params.blocked = true;
    for (int i = 0; i < 6; ++i)
    {
        std::string path = std::string("resources/sprites/devil/death/") + std::to_string(i) +
                           ".png";
        sprite_devil_params.death_animation.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_devil_params.death_animation_speed = 10;
    sprite_devil_params.dead_shift = 0.6f;
    sprite_devil_params.flag_type = SpriteFlagType::FLAG_NPC;
    sprite_devil_params.obj_speed = 30;
    sprite_parameters["sprite_devil"] = std::move(sprite_devil_params);

    SpriteParameter sprite_flame_params{};
    sprite_flame_params.sprite.emplace_back(
            rg::image::Load("resources/sprites/flame/base/0.png"));
    sprite_flame_params.shift = 0.7f;
    sprite_flame_params.scale = {0.6f, 0.6f};
    sprite_flame_params.side = 30;
    for (int i = 0; i < 16; ++i)
    {
        std::string path = std::string("resources/sprites/flame/anim/") + std::to_string(i) +
                           ".png";
        sprite_flame_params.animation.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_flame_params.animation_dist = 800;
    sprite_flame_params.animation_speed = 5;
    sprite_flame_params.dead_shift = 1.8f;
    sprite_flame_params.is_dead = SpriteStatus::STATUS_IMMORTAL;
    sprite_parameters["sprite_flame"] = std::move(sprite_flame_params);

    // door vertical - vertical from the map view
    SpriteParameter sprite_door_v_params{};
    for (int i = 0; i < 16; ++i)
    {
        std::string path = "resources/sprites/doors/door_v/" + std::to_string(i) + ".png";
        sprite_door_v_params.sprite.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_door_v_params.viewing_angles = true;
    sprite_door_v_params.shift = 0.1f;
    sprite_door_v_params.scale = {2.6f, 1.2f};
    sprite_door_v_params.side = 100;
    sprite_door_v_params.is_dead = SpriteStatus::STATUS_IMMORTAL;
    sprite_door_v_params.animation_speed = 60;
    sprite_door_v_params.blocked = true;
    // flag is inverted (h or v) because will indicate the
    // direction it will move once it is opened
    sprite_door_v_params.flag_type = SpriteFlagType::FLAG_DOOR_H;
    sprite_parameters["sprite_door_v"] = std::move(sprite_door_v_params);

    SpriteParameter sprite_door_h_params{};
    for (int i = 0; i < 16; ++i)
    {
        std::string path = "resources/sprites/doors/door_h/" + std::to_string(i) + ".png";
        sprite_door_h_params.sprite.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_door_h_params.viewing_angles = true;
    sprite_door_h_params.shift = 0.1f;
    sprite_door_h_params.scale = {2.6f, 1.2f};
    sprite_door_h_params.side = 100;
    sprite_door_h_params.is_dead = SpriteStatus::STATUS_IMMORTAL;
    sprite_door_h_params.animation_speed = 60;
    sprite_door_h_params.blocked = true;
    // flag is inverted (h or v) because will indicate the
    // direction it will move once it is opened
    sprite_door_h_params.flag_type = SpriteFlagType::FLAG_DOOR_V;
    sprite_parameters["sprite_door_h"] = std::move(sprite_door_h_params);

    // image, static/animated, position, shift height, scale
    list_of_objects.emplace_back(
            &sprite_parameters["sprite_barrel"], rg::math::Vector2{7.1f, 2.1f});
    list_of_objects.emplace_back(
            &sprite_parameters["sprite_barrel"], rg::math::Vector2{5.9f, 2.1f});
    list_of_objects.emplace_back(
            &sprite_parameters["sprite_pin"], rg::math::Vector2{8.7f, 2.5f});
    list_of_objects.emplace_back(
            &sprite_parameters["sprite_devil"], rg::math::Vector2{7.0f, 4.0f});
    list_of_objects.emplace_back(
            &sprite_parameters["sprite_flame"], rg::math::Vector2{8.6f, 5.6f});
    list_of_objects.emplace_back(
            &sprite_parameters["sprite_door_v"], rg::math::Vector2{3.5f, 3.5f});
    list_of_objects.emplace_back(
            &sprite_parameters["sprite_door_h"], rg::math::Vector2{1.5f, 4.5f});
}

SpriteProjection Sprites::closest_sprite_projection() const
{
    SpriteProjection result{.depth = std::numeric_limits<float>::infinity(),
                            .dimensions = {}, .x = 0, .y = 0, .pos = {}};
    for (const auto &obj: list_of_objects)
    {
        const auto proj = obj.sprite_projection();
        if (proj.depth < result.depth)
        {
            result = proj;
        }
    }
    return result;
}

std::unordered_map<rg::math::Vector2<int>, int> Sprites::blocked_doors() const
{
    std::unordered_map<rg::math::Vector2<int>, int> result;
    for (const auto &obj: list_of_objects)
    {
        if ((obj.flag == SpriteFlagType::FLAG_DOOR_H || obj.flag == SpriteFlagType::FLAG_DOOR_V)
            && obj.blocked)
        {
            auto [i,j] = mapping(obj.x, obj.y);
            result[{i, j}] = 0;
        }
    }
    return result;
}
