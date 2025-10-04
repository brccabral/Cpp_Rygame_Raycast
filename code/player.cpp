#include "player.hpp"

#include "maplevels.hpp"
#include "settings.hpp"

Player::Player(
        const rg::math::Vector2<float> &pos, const float angle, const int speed,
        const float rotation_speed)
    : x(pos.x), y(pos.y), angle(angle), speed(speed), rotation_speed(rotation_speed),
      settings(Settings::GetInstance())
{
    map_surface = rg::Surface(settings->width, settings->height);
    map_surface.SetColorKey(settings->black);
}

rg::math::Vector2<float> Player::pos() const
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

    if (rl::IsKeyDown(rl::KEY_TAB))
    {
        show_map(sc);
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

    for (auto &[map_x, map_y]: MapLevels::GetInstance()->world_map | std::views::keys)
    {
        rg::draw::rect(
                &map_surface, settings->white, {static_cast<float>(map_x),
                                                static_cast<float>(map_y),
                                                static_cast<float>(settings->tile),
                                                static_cast<float>(settings->tile)},
                2);
    }
    sc->Blit(&map_surface, rg::math::Vector2<float>{});
}
