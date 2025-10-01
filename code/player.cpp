#include "player.hpp"
#include "settings.hpp"

Player::Player()
{
    x = Settings::GetInstance()->player_pos.x;
    y = Settings::GetInstance()->player_pos.y;
    angle = Settings::GetInstance()->player_angle;
}

rg::math::Vector2 Player::pos()
{
    return {x, y};
}

void Player::movement(const float dt)
{
    if (rl::IsKeyDown(rl::KEY_W))
    {
        y -= Settings::GetInstance()->player_speed * dt;
    }
    if (rl::IsKeyDown(rl::KEY_S))
    {
        y += Settings::GetInstance()->player_speed * dt;
    }
    if (rl::IsKeyDown(rl::KEY_A))
    {
        x -= Settings::GetInstance()->player_speed * dt;
    }
    if (rl::IsKeyDown(rl::KEY_D))
    {
        x += Settings::GetInstance()->player_speed * dt;
    }
    if (rl::IsKeyDown(rl::KEY_LEFT))
    {
        angle -= Settings::GetInstance()->player_rotation * dt;
    }
    if (rl::IsKeyDown(rl::KEY_RIGHT))
    {
        angle += Settings::GetInstance()->player_rotation * dt;
    }
}
