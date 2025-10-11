#include "sprite_objects.hpp"

SpriteObject::SpriteObject(
        rg::Surface *object, const bool is_static, const rg::math::Vector2<float> pos,
        const float shift, const float scale)
    : object(object), is_static(is_static), pos(pos), x(pos.x), y(pos.y), shift(shift), scale(scale)
{
    x *= Settings::GetInstance()->tile;
    y *= Settings::GetInstance()->tile;
}

SpriteObjectLocate SpriteObject::object_locate(
        const Player *player, const Settings *settings,
        const std::vector<SpriteObjectLocate> &walls) const
{
    const auto dx = x - player->x;
    const auto dy = y - player->y;
    auto distance_to_sprite = std::sqrt(dx * dx + dy * dy);

    // theta = sprite to player horizontal
    const auto theta = atan2(dy, dx);
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
    const auto current_ray = settings->center_ray + delta_rays;

    // fix fish eye
    distance_to_sprite *= cosf(settings->half_fov - current_ray * settings->delta_angle);

    // check if sprite is inside "field of view" and
    // if it is in front of wall
    if (0 <= current_ray && current_ray <= settings->num_rays - 1
        && distance_to_sprite < walls.at(current_ray).depth)
    {
        const auto proj_height = int(settings->proj_coeff / distance_to_sprite * scale);
        const auto half_proj_height = proj_height / 2;
        const auto sprite_shift = half_proj_height * shift;

        return {distance_to_sprite, object,
                {static_cast<float>(proj_height), static_cast<float>(proj_height)},
                {current_ray * settings->scale - half_proj_height,
                 settings->half_height - half_proj_height + static_cast<int>(sprite_shift)},
                {}};
    }
    return {};
}

Sprites::Sprites()
{
    sprite_types.emplace("barrel", rg::image::Load("resources/sprites/barrel/base/0.png"));
    sprite_types.emplace("pedestal", rg::image::Load("resources/sprites/pedestal/base/0.png"));

    // image, static/animated, position, shift height, scale
    list_of_objects.emplace_back(
            &sprite_types["barrel"], true, rg::math::Vector2{7.1f, 2.1f}, 1.8f, 0.4f);
    list_of_objects.emplace_back(
            &sprite_types["barrel"], true, rg::math::Vector2{5.9f, 2.1f}, 1.8f, 0.4f);
    list_of_objects.emplace_back(
            &sprite_types["pedestal"], true, rg::math::Vector2{8.8f, 2.5f}, 1.6f, 0.5f);
    list_of_objects.emplace_back(
            &sprite_types["pedestal"], true, rg::math::Vector2{8.8f, 5.6f}, 1.6f, 0.5f);
}
