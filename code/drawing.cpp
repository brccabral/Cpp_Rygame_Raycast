#include "drawing.hpp"

Drawing::Drawing(rg::Surface *sc, Player *player)
    : sc(sc), settings(Settings::GetInstance()), player(player)
{
}

void Drawing::background() const
{
    rg::draw::rect(
            sc, settings->skyblue,
            {0, 0, static_cast<float>(settings->width),
             static_cast<float>(settings->half_height)});
    rg::draw::rect(
            sc, settings->darkgray,
            {0, static_cast<float>(settings->half_height), static_cast<float>(settings->width),
             static_cast<float>(settings->half_height)});
    player->map_surface.Fill(settings->blank);
}

void Drawing::world() const
{
    player->ray_casting(sc);
}

void Drawing::fps(const float dt) const
{
    const auto render = font.render(rl::TextFormat("%.1f", 1.0f / dt), settings->red);
    sc->Blit(&render, settings->fps_pos);
}
