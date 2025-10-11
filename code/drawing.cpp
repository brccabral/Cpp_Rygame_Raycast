#include "drawing.hpp"

#include "ray_casting.hpp"

Drawing::Drawing(rg::Surface *sc, rg::Surface *sc_map, Player *player)
    : sc(sc), sc_map(sc_map), settings(Settings::GetInstance()),
      map_levels(MapLevels::GetInstance()), player(player)
{
}

void Drawing::background() const
{
    rg::draw::rect(
            sc, settings->skyblue,
            {0, 0, (settings->width),
             (settings->half_height)});
    rg::draw::rect(
            sc, settings->darkgray,
            {0, settings->half_height, settings->width, settings->half_height});
    sc_map->Fill(settings->black);
}

void Drawing::world()
{
    // ray_casting_distance(sc, sc_map, player, settings, &texture);
    ray_casting_depth(sc, sc_map, player, settings, &texture);
}

void Drawing::fps(const float dt)
{
    fps_text_surface = font.render(rl::TextFormat("%.1f", 1.0f / dt), settings->red);
    sc->Blit(&fps_text_surface, settings->fps_pos);
}

void Drawing::mini_map() const
{
    // scale player position
    auto [player_x_map, player_y_map] = rg::math::Vector2{player->x / settings->map_scale,
                                                          player->y / settings->map_scale};

    // draw walls
    for (auto [x, y]: map_levels->mini_map | std::views::keys)
    {
        rg::draw::rect(
                sc_map, settings->white, {x, y, settings->map_tile, settings->map_tile});
    }

    // player position
    rg::draw::circle(sc_map, settings->red, {player_x_map, player_y_map}, 5);
    // player direction
    rg::draw::line(
            sc_map, settings->green, {player_x_map, player_y_map}, {
                    player_x_map + settings->width / settings->map_scale * cosf(player->angle),
                    player_y_map + settings->width / settings->map_scale * sinf(player->angle)});

    sc->Blit(sc_map, settings->map_pos);
}
