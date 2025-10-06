#include "player.hpp"

#include "maplevels.hpp"
#include "settings.hpp"

Player::Player(
        const rg::math::Vector2<float> &pos, const float angle, const int speed,
        const float rotation_speed)
    : x(pos.x), y(pos.y), angle(angle), speed(speed), rotation_speed(rotation_speed),
      settings(Settings::GetInstance())
{
}

rg::math::Vector2<float> Player::pos() const
{
    return {x, y};
}

rg::math::Vector2<float> Player::pos_map() const
{
    return {x / settings->map_scale, y / settings->map_scale};
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
}
