#include <rygame.hpp>

#include "settings.hpp"
#include "player.hpp"


int main()
{
    rg::Init();

    const Settings *settings = Settings::GetInstance();

    auto *sc = &rg::display::SetMode(settings->width, settings->height);
    float dt = 0;

    auto player = Player(
            {Settings::GetInstance()->half_width, Settings::GetInstance()->half_height}, 0.f, 120,
            1.2f);

    while (!rg::WindowCloseOrQuit())
    {
        dt = rl::GetFrameTime();

        sc->Fill(settings->black);
        rg::draw::rect(
                sc, settings->skyblue,
                {0, 0, static_cast<float>(settings->width),
                 static_cast<float>(settings->half_height)});
        rg::draw::rect(
                sc, settings->darkgray,
                {0, static_cast<float>(settings->half_height), static_cast<float>(settings->width),
                 static_cast<float>(settings->half_height)});

        player.movement(dt, sc);

        rg::display::Update();
    }

    Settings::Destroy();

    rg::Quit();
}
