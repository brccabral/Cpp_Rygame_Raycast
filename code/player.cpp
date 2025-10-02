#include "player.hpp"
#include "settings.hpp"

Player::Player()
{
    x = Settings::GetInstance()->player_pos.x;
    y = Settings::GetInstance()->player_pos.y;
    angle = Settings::GetInstance()->player_angle;
    player_speed = Settings::GetInstance()->player_speed;
    player_rotation = Settings::GetInstance()->player_rotation;
}

rg::math::Vector2 Player::pos() const
{
    return {x, y};
}

void Player::movement(const float dt)
{
    auto sin_a = sinf(angle);
    auto cos_a = cosf(angle);

    if (rl::IsKeyDown(rl::KEY_W))
    {
        x += player_speed * dt * cos_a;
        y += player_speed * dt * sin_a;
    }
    if (rl::IsKeyDown(rl::KEY_S))
    {
        x -= player_speed * dt * cos_a;
        y -= player_speed * dt * sin_a;
    }
    if (rl::IsKeyDown(rl::KEY_A))
    {
        x += player_speed * dt * sin_a;
        y -= player_speed * dt * cos_a;
    }
    if (rl::IsKeyDown(rl::KEY_D))
    {
        x -= player_speed * dt * sin_a;
        y += player_speed * dt * cos_a;
    }
    if (rl::IsKeyDown(rl::KEY_LEFT))
    {
        angle -= player_rotation * dt;
    }
    if (rl::IsKeyDown(rl::KEY_RIGHT))
    {
        angle += player_rotation * dt;
    }
}
