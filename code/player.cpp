#include "player.hpp"

#include "maplevels.hpp"
#include "settings.hpp"

Player::Player(
        const rg::math::Vector2<float> &pos, const float angle, const int speed,
        const float rotation_speed, const float mouse_sensitivity)
    : x(pos.x), y(pos.y), angle(angle), speed(speed), rotation_speed(rotation_speed),
      mouse_sensitivity(mouse_sensitivity), settings(Settings::GetInstance())
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

void Player::movement(const float dt)
{
    keys_control(dt);
    mouse_control(dt);

    // angle %= double_pi (modulo)
    angle += settings->double_pi;
    const auto div = angle / settings->double_pi;
    angle = angle - int(div) * settings->double_pi;
}

void Player::keys_control(const float dt)
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

void Player::mouse_control(const float dt)
{
    // const float difference = rl::GetMouseDelta().x;
    const float difference = rl::GetMousePosition().x - settings->half_width;
    // always center the mouse after detecting the difference to
    // have the same reference at all passes
    rl::SetMousePosition(settings->half_width, settings->half_height);
    angle += difference * mouse_sensitivity * dt;
}
