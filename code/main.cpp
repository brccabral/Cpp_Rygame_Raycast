#include <rygame.hpp>
#include "settings.hpp"
#include "player.hpp"


int main()
{
    rg::Init();

    const Settings *settings = Settings::GetInstance();

    auto *sc = &rg::display::SetMode(settings->width, settings->height);
    float dt = 0;

    auto player = Player();

    while (!rg::WindowCloseOrQuit())
    {
        dt = rl::GetFrameTime();
        player.movement(dt);

        sc->Fill(settings->black);
        rg::draw::circle(sc, settings->green, player.pos(), 12);
        rg::draw::line(
                sc, settings->green, player.pos(),
                {player.x + settings->width * cosf(player.angle),
                 player.y + settings->height * sinf(player.angle)}
                );
        rg::display::Update();
    }

    Settings::Destroy();

    rg::Quit();
}
