#include "player.hpp"
#include "settings.hpp"

Player::Player(
        const rg::math::Vector2 &pos, const float angle, const int speed,
        const float rotation_speed)
    : x(pos.x), y(pos.y), angle(angle), speed(speed), rotation_speed(rotation_speed)
{
}

rg::math::Vector2 Player::pos() const
{
    return {x, y};
}

void Player::movement(const float dt)
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
