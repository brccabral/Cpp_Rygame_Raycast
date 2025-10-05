#pragma once
#include "maplevels.hpp"
#include "player.hpp"
#include "rygame.hpp"
#include "settings.hpp"


class Drawing
{
public:

    Drawing(rg::Surface *sc, rg::Surface *sc_map, Player *player);

    void background() const;
    void world() const;
    void fps(float dt) const;
    void mini_map() const;

    rg::Surface *sc;
    rg::Surface *sc_map;
    Settings *settings;
    MapLevels *map_levels;
    Player *player;
    rg::font::Font font{"/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed-Bold.ttf", 36};
};
