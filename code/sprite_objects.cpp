#include "sprite_objects.hpp"

SpriteObject::SpriteObject(
        std::vector<rg::Surface> *objects, const bool is_static, const rg::math::Vector2<float> pos,
        const float shift, const float scale)
    : objects(objects), is_static(is_static), pos(pos), x(pos.x), y(pos.y), shift(shift),
      scale(scale)
{
    x *= Settings::GetInstance()->tile;
    y *= Settings::GetInstance()->tile;
    this->pos.x = x;
    this->pos.y = y;

    if (!is_static)
    {
        for (int i = 0; i < 360; i += 45)
        {
            sprite_angles.emplace_back(i, i + 45);
        }
        for (int i = 0; i < sprite_angles.size(); ++i)
        {
            sprite_positions[sprite_angles[i]] = &(*this->objects)[i];
        }
    }

    current_object = &(*this->objects)[0];
}

SpriteObjectLocate SpriteObject::object_locate(const Player *player)
{
    const Settings *settings = Settings::GetInstance();

    const auto dx = x - player->x;
    const auto dy = y - player->y;
    auto distance_to_sprite = std::sqrt(dx * dx + dy * dy);

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
    const auto current_ray = settings->center_ray + delta_rays;

    // fix fish eye
    distance_to_sprite *= cosf(settings->half_fov - current_ray * settings->delta_angle);

    // FAKE_RAYS_RANGE increase rays on the sides to avoid a
    // sprite disappearing when close to the border of the screen
    auto fake_ray = current_ray + settings->fake_rays;
    // check if sprite is inside "field of view" and
    // if it is more than 30 px of distance to avoid
    // dropping frame rate when player is too close
    // of a sprite, or a sprite is half-way behind a wall
    if (0 <= fake_ray && fake_ray <= settings->fake_rays_range && distance_to_sprite > 30)
    {
        const auto proj_height = std::min(
                int(settings->proj_coeff / distance_to_sprite * scale), settings->double_height);
        const auto half_proj_height = proj_height / 2;
        const auto sprite_shift = half_proj_height * shift;

        if (!is_static)
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
                    current_object = sprite_positions[angles];
                    break;
                }
            }
        }

        return {distance_to_sprite, current_object,
                {static_cast<float>(proj_height), static_cast<float>(proj_height)},
                {current_ray * settings->scale - half_proj_height,
                 settings->half_height - half_proj_height + static_cast<int>(sprite_shift)},
                {}};
    }
    return {};
}

Sprites::Sprites()
{
    std::vector<rg::Surface> barrel_surfs;
    barrel_surfs.emplace_back(rg::image::Load("resources/sprites/barrel/base/0.png"));
    sprite_types["barrel"] = std::move(barrel_surfs);
    std::vector<rg::Surface> pedestal_surfs;
    pedestal_surfs.emplace_back(rg::image::Load("resources/sprites/pedestal/base/0.png"));
    sprite_types["pedestal"] = std::move(pedestal_surfs);
    std::vector<rg::Surface> devil_surfs;
    for (int i = 0; i < 8; ++i)
    {
        std::string path = std::string("resources/sprites/devil/base/") + std::to_string(i) +
                           std::string(".png");
        devil_surfs.emplace_back(rg::image::Load(path.c_str()));
    }
    sprite_types["devil"] = std::move(devil_surfs);

    // image, static/animated, position, shift height, scale
    list_of_objects.emplace_back(
            &sprite_types["barrel"], true, rg::math::Vector2{7.1f, 2.1f}, 1.8f, 0.4f);
    list_of_objects.emplace_back(
            &sprite_types["barrel"], true, rg::math::Vector2{5.9f, 2.1f}, 1.8f, 0.4f);
    list_of_objects.emplace_back(
            &sprite_types["pedestal"], true, rg::math::Vector2{8.8f, 2.5f}, 1.6f, 0.5f);
    list_of_objects.emplace_back(
            &sprite_types["pedestal"], true, rg::math::Vector2{8.8f, 5.6f}, 1.6f, 0.5f);
    list_of_objects.emplace_back(
            &sprite_types["devil"], false, rg::math::Vector2{7.0f, 4.0f}, -0.2f, 0.7f);
}
