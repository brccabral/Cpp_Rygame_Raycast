#include <limits>
#include "sprite_objects.hpp"

SpriteObject::SpriteObject(SpriteParameter *parameter, const rg::math::Vector2<float> pos)
    : parameter(parameter), settings(Settings::GetInstance())
{
    blocked = parameter->blocked;

    x = pos.x * settings->tile;
    y = pos.y * settings->tile;

    object = &parameter->sprite[0];

    if (!parameter->viewing_angles)
    {
        for (int i = 0; i < 360; i += 45)
        {
            sprite_angles.emplace_back(i, i + 45);
        }
        for (int i = 0; i < sprite_angles.size(); ++i)
        {
            sprite_positions[sprite_angles[i]] = &parameter->sprite[i];
        }
    }

    animation_speed = parameter->animation_speed;
}

SpriteObjectLocate SpriteObject::object_locate(const Player *player, const float dt)
{
    const auto dx = x - player->x;
    const auto dy = y - player->y;
    distance_to_sprite = std::sqrt(dx * dx + dy * dy);

    // theta = sprite to player horizontal
    auto theta = atan2(dy, dx);
    // gamma = angle between sprite and player direction
    auto gamma = theta - player->angle;

    // work with positive angles
    const auto degrees = player->angle * 180.0f / M_PI;
    if (dx > 0.0f && 180.0f <= degrees && degrees <= 360.0f || dx < 0.0f && dy < 0.0f)
    {
        gamma += settings->double_pi;
    }

    // delta_rays = number of rays between player direction and sprite
    const auto delta_rays = int(gamma / settings->delta_angle);
    current_ray = settings->center_ray + delta_rays;

    // fix fish eye
    distance_to_sprite *= cosf(settings->half_fov - current_ray * settings->delta_angle);

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
                int(settings->proj_coeff / distance_to_sprite * parameter->scale),
                settings->double_height);
        const auto half_proj_height = proj_height / 2;
        const auto sprite_shift = half_proj_height * parameter->shift;

        if (!parameter->viewing_angles)
        {
            if (theta < 0)
            {
                theta += settings->double_pi;
            }
            theta = 360 - int(theta * 180.0f / M_PI);

            for (const auto &angles: sprite_angles)
            {
                if (theta >= angles.x && theta < angles.y)
                {
                    object = sprite_positions[angles];
                    break;
                }
            }
        }

        // sprite animation
        if (!parameter->animation.empty() && distance_to_sprite < parameter->animation_dist)
        {
            animation_index += parameter->animation_speed * dt;
            if (animation_index >= parameter->animation.size())
            {
                animation_index = 0;
            }
            object = &parameter->animation[int(animation_index)];
        }
        else
        {
            object = &parameter->sprite[0];
        }

        position = {current_ray * settings->scale - half_proj_height,
                    settings->half_height - half_proj_height + sprite_shift};

        return {.depth = distance_to_sprite, .sprite = object,
                .sprite_dimension = {proj_height, proj_height},
                .sprite_pos = position,
                .sprite_area = {}};
    }
    return {};
}

rg::math::Vector2<float> SpriteObject::pos() const
{
    return {x - side / 2, y - side / 2};
}

SpriteProjection SpriteObject::sprite_projection() const
{
    if (settings->center_ray - side < current_ray && current_ray < settings->center_ray + side &&
        parameter->blocked)
    {
        return {.depth = distance_to_sprite, .proj_height = proj_height, .x = x, .y = y};
    }
    return {.depth = std::numeric_limits<float>::infinity(), .proj_height = 0, .x = 0, .y = 0};
}

Sprites::Sprites()
{
    SpriteParameter sprite_barrel_params;
    sprite_barrel_params.sprite.emplace_back(
            rg::image::Load("resources/sprites/barrel/base/0.png"));
    sprite_barrel_params.shift = 1.8f;
    sprite_barrel_params.scale = 0.4f;
    for (int i = 0; i < 13; ++i)
    {
        std::string path = std::string("resources/sprites/barrel/anim/") + std::to_string(i) +
                           ".png";
        sprite_barrel_params.animation.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_barrel_params.animation_dist = 800;
    sprite_barrel_params.animation_speed = 10;
    sprite_barrel_params.blocked = true;
    sprite_parameters["sprite_barrel"] = std::move(sprite_barrel_params);

    SpriteParameter sprite_pin_params;
    sprite_pin_params.sprite.emplace_back(
            rg::image::Load("resources/sprites/pin/base/0.png"));
    sprite_pin_params.shift = 0.6f;
    sprite_pin_params.scale = 0.6f;
    for (int i = 0; i < 8; ++i)
    {
        std::string path = std::string("resources/sprites/pin/anim/") + std::to_string(i) +
                           ".png";
        sprite_pin_params.animation.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_pin_params.animation_dist = 800;
    sprite_pin_params.animation_speed = 10;
    sprite_pin_params.blocked = true;
    sprite_parameters["sprite_pin"] = std::move(sprite_pin_params);

    SpriteParameter sprite_devil_params;
    sprite_devil_params.sprite.emplace_back(
            rg::image::Load("resources/sprites/devil/base/0.png"));
    sprite_devil_params.viewing_angles = true;
    sprite_devil_params.shift = -0.2f;
    sprite_devil_params.scale = 1.1f;
    for (int i = 0; i < 9; ++i)
    {
        std::string path = std::string("resources/sprites/devil/anim/") + std::to_string(i) +
                           ".png";
        sprite_devil_params.animation.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_devil_params.animation_dist = 200;
    sprite_devil_params.animation_speed = 10;
    sprite_devil_params.blocked = true;
    sprite_parameters["sprite_devil"] = std::move(sprite_devil_params);

    SpriteParameter sprite_flame_params;
    sprite_flame_params.sprite.emplace_back(
            rg::image::Load("resources/sprites/flame/base/0.png"));
    sprite_flame_params.shift = 0.7f;
    sprite_flame_params.scale = 0.6f;
    for (int i = 0; i < 16; ++i)
    {
        std::string path = std::string("resources/sprites/flame/anim/") + std::to_string(i) +
                           ".png";
        sprite_flame_params.animation.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_flame_params.animation_dist = 800;
    sprite_flame_params.animation_speed = 5;
    sprite_parameters["sprite_flame"] = std::move(sprite_flame_params);

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
}

SpriteProjection Sprites::closest_sprite_projection() const
{
    SpriteProjection result{.depth = std::numeric_limits<float>::infinity(),
                            .proj_height = 0, .x = 0, .y = 0};
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
