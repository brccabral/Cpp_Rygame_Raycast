#include "player.hpp"

#include "maplevels.hpp"
#include "settings.hpp"

Player::Player(
        const rg::math::Vector2 &pos, const float angle, const int speed,
        const float rotation_speed)
    : x(pos.x), y(pos.y), angle(angle), speed(speed), rotation_speed(rotation_speed),
      settings(Settings::GetInstance())
{
    map_surface = rg::Surface(settings->width, settings->height);
    map_surface.SetColorKey(settings->black);
}

rg::math::Vector2 Player::pos() const
{
    return {x, y};
}

void Player::movement(const float dt, rg::Surface *sc)
{
    const auto sin_a = sinf(angle);
    const auto cos_a = cosf(angle);

    if (rl::IsKeyDown(rl::KEY_W))
    {
        x += speed * dt * cos_a;
        y += speed * dt * sin_a;
    }
    if (rl::IsKeyDown(rl::KEY_S))
    {
        x -= speed * dt * cos_a;
        y -= speed * dt * sin_a;
    }
    if (rl::IsKeyDown(rl::KEY_A))
    {
        x += speed * dt * sin_a;
        y -= speed * dt * cos_a;
    }
    if (rl::IsKeyDown(rl::KEY_D))
    {
        x -= speed * dt * sin_a;
        y += speed * dt * cos_a;
    }
    if (rl::IsKeyDown(rl::KEY_LEFT))
    {
        angle -= rotation_speed * dt;
    }
    if (rl::IsKeyDown(rl::KEY_RIGHT))
    {
        angle += rotation_speed * dt;
    }

    map_surface.Fill(settings->blank);
    ray_casting(sc);

    if (rl::IsKeyDown(rl::KEY_TAB))
    {
        show_map(sc);
    }

}


void Player::ray_casting(rg::Surface *sc)
{
    auto cur_angle = angle - settings->half_fov;
    for (int ray = 0; ray < settings->num_rays; ++ray)
    {
        auto sin_a = sinf(cur_angle);
        auto cos_a = cosf(cur_angle);
        for (int depth = 0; depth < settings->max_depth; ++depth)
        {
            auto xf = x + depth * cos_a;
            auto yf = y + depth * sin_a;
            int xi = int(xf / settings->tile) * settings->tile;
            int yi = int(yf / settings->tile) * settings->tile;
            if (MapLevels::GetInstance()->world_map.find({xi, yi}) !=
                MapLevels::GetInstance()->world_map.end())
            {
                rg::draw::line(&map_surface, settings->darkgray, pos(), {xf, yf}, 2);
                auto proj_height = settings->proj_coeff / depth;
                const unsigned char c = 255 / (1 + depth * depth * 0.0001);
                auto color = rl::Color{c, c, c, 255};
                rg::draw::rect(
                        sc, color, {
                                (float) ray * settings->scale,
                                settings->half_height - proj_height / 2,
                                (float) settings->scale,
                                proj_height
                        });
                break;
            }
        }
        cur_angle += settings->delta_angle;
    }
}

void Player::show_map(rg::Surface *sc)
{
    rg::draw::circle(&map_surface, settings->green, pos(), 12);
    rg::draw::line(
            &map_surface, settings->green, pos(),
            {x + settings->width * cosf(angle),
             y + settings->height * sinf(angle)}
            );

    for (auto &[mapx, mapy]: MapLevels::GetInstance()->world_map | std::views::keys)
    {
        rg::draw::rect(
                &map_surface, settings->darkgray, {mapx, mapy, (float) settings->tile,
                                                   (float) settings->tile},
                2);
    }
    sc->Blit(&map_surface, {0, 0});
}
