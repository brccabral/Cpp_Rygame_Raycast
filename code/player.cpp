#include "player.hpp"

#include "maplevels.hpp"
#include "settings.hpp"
#include "sprite_objects.hpp"

Player::Player(
        Sprites *sprites, const rg::math::Vector2<float> &pos, const float angle, const int speed,
        const float rotation_speed, const float mouse_sensitivity)
    : x(pos.x), y(pos.y), angle(angle), speed(speed), rotation_speed(rotation_speed),
      mouse_sensitivity(mouse_sensitivity), settings(Settings::GetInstance()),
      map_levels(MapLevels::GetInstance()), rect(x, y, side, side), sprites(sprites)
{
    collision_list.reserve(sprites->list_of_objects.size() + map_levels->collision_walls.size());
    for (auto &object: sprites->list_of_objects)
    {
        if (object.blocked)
        {
            collision_list.emplace_back(object.pos().x, object.pos().y, object.side, object.side);
        }
    }
    collision_list.insert(
            collision_list.end(), map_levels->collision_walls.begin(),
            map_levels->collision_walls.end());
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

    rect.centerx(x).centery(y);

    // angle %= double_pi (modulo)
    angle += settings->double_pi;
    const auto div = angle / settings->double_pi;
    angle = angle - int(div) * settings->double_pi;
}

void Player::shoot()
{
    if (!shot)
    {
        shot = true;
    }
}

void Player::keys_control(const float dt)
{
    const auto sin_a = sinf(angle);
    const auto cos_a = cosf(angle);

    float dx{}, dy{};

    if (rl::IsKeyDown(rl::KEY_W))
    {
        dx = speed * dt * cos_a;
        dy = speed * dt * sin_a;
        detect_collision(dx, dy);
    }
    if (rl::IsKeyDown(rl::KEY_S))
    {
        dx = -speed * dt * cos_a;
        dy = -speed * dt * sin_a;
        detect_collision(dx, dy);
    }
    if (rl::IsKeyDown(rl::KEY_A))
    {
        dx = speed * dt * sin_a;
        dy = -speed * dt * cos_a;
        detect_collision(dx, dy);
    }
    if (rl::IsKeyDown(rl::KEY_D))
    {
        dx = -speed * dt * sin_a;
        dy = speed * dt * cos_a;
        detect_collision(dx, dy);
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

void Player::detect_collision(float dx, float dy)
{
    auto next_rect = rect.copy();
    next_rect.move_ip(dx, dy);
    const auto hit_indexes = next_rect.collidelistall(collision_list);

    if (!hit_indexes.empty())
    {
        float delta_x{}, delta_y{};
        for (const auto &hit_index: hit_indexes)
        {
            auto &hit_rect = collision_list[hit_index];
            if (dx > 0)
            {
                delta_x += next_rect.right() - hit_rect.left();
            }
            else
            {
                delta_x += hit_rect.right() - next_rect.left();
            }
            if (dy > 0)
            {
                delta_y += next_rect.bottom() - hit_rect.top();
            }
            else
            {
                delta_y += hit_rect.bottom() - next_rect.top();
            }
        }
        if (fabsf(delta_x - delta_y) < 10)
        {
            dx = 0;
            dy = 0;
        }
        else if (delta_x > delta_y)
        {
            dy = 0;
        }
        else if (delta_y > delta_x)
        {
            dx = 0;
        }
    }

    x += dx;
    y += dy;
}
