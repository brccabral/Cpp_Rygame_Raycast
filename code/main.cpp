#include <rygame.hpp>
#include "settings.hpp"


int main()
{
    rg::Init();

    const Settings *settings = Settings::GetInstance();

    auto *sc = &rg::display::SetMode(settings->width, settings->height);

    while (!rg::WindowCloseOrQuit())
    {
        sc->Fill(settings->black);
        rg::draw::circle(sc, settings->green, settings->player_pos, 12);
        rg::display::Update();
    }

    Settings::Destroy();

    rg::Quit();
}
