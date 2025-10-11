#pragma once
#include <rygame.hpp>
#include "maplevels.hpp"
#include "player.hpp"
#include "settings.hpp"


class Drawing
{
public:

    Drawing(rg::Surface *sc, rg::Surface *sc_map, Player *player);

    void background();
    void world();
    void fps(float dt);
    void mini_map() const;

    rg::Surface *sc;
    rg::Surface *sc_map;
    Settings *settings;
    MapLevels *map_levels;
    Player *player;
    rg::Surface fps_text_surface{};
    rg::font::Font font{"/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed-Bold.ttf", 36};
    std::unordered_map<int, rg::Surface> textures;
    rg::Surface sky_surface{};
};
