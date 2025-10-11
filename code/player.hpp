#pragma once
#include "rygame.hpp"
#include "settings.hpp"


class Player
{
public:

    Player(
            const rg::math::Vector2<float> &pos, float angle, int speed, float rotation_speed,
            float mouse_sensitivity);
    ~Player() = default;

    [[nodiscard]] rg::math::Vector2<float> pos() const;
    [[nodiscard]] rg::math::Vector2<float> pos_map() const;
    void movement(float dt);

    float x{};
    float y{};
    float angle{};
    int speed{};
    float rotation_speed{};
    float mouse_sensitivity{};
    Settings *settings{};

private:

    void keys_control(float dt);
    void mouse_control(float dt);

};
