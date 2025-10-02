#include "ray_casting.hpp"

#include "settings.hpp"


void ray_casting(rg::Surface *sc, rg::math::Vector2 player_pos, float player_angle)
{
    auto cur_angle = player_angle - Settings::GetInstance()->half_fov;
    auto [xo, yo] = player_pos;
    for (int ray = 0; ray < Settings::GetInstance()->num_rays; ++ray)
    {
        auto sin_a = sinf(cur_angle);
        auto cos_a = cosf(cur_angle);
        for (int depth = 0; depth < Settings::GetInstance()->max_depth; ++depth)
        {
            auto x = xo + depth * cos_a;
            auto y = yo + depth * sin_a;
            rg::draw::line(sc, Settings::GetInstance()->darkgray, player_pos, {x, y}, 2);
        }
        cur_angle += Settings::GetInstance()->delta_angle;
    }
}
