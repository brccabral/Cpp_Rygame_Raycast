#pragma once
#include "player.hpp"
#include "rygame.hpp"
#include "settings.hpp"


class Drawing
{
public:

    Drawing(rg::Surface *sc, Player *player);

    void background() const;
    void world() const;
    void fps(float dt) const;

    rg::Surface *sc;
    Settings *settings;
    Player *player;
    rg::font::Font font{"/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed-Bold.ttf", 36};
};
