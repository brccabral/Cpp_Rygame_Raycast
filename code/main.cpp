#include <rygame.hpp>
#include "settings.hpp"


int main()
{
    rg::Init();

    auto* sc = &rg::display::SetMode(Settings::width, Settings::height);

    while (!rg::WindowCloseOrQuit())
    {
        sc->Fill(Settings::black);
        rg::display::Update();
    }

    rg::Quit();
}
