#include "drawing.hpp"

Drawing::Drawing(rg::Surface *sc)
    : sc(sc), settings(Settings::GetInstance())
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
}
