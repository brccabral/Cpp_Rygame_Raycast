#pragma once
#include "rygame.hpp"
#include "settings.hpp"


class Drawing
{
public:
    Drawing(rg::Surface *sc);

    void background() const;

    rg::Surface *sc;
    Settings *settings;
};
