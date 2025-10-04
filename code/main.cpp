#include <rygame.hpp>

#include "drawing.hpp"
#include "settings.hpp"
#include "player.hpp"


int main()
{
    rg::Init();

    {
        const Settings *settings = Settings::GetInstance();

        auto *sc = &rg::display::SetMode(settings->width, settings->height);
        float dt = 0;

        auto player = Player(
                {static_cast<float>(Settings::GetInstance()->half_width),
                 static_cast<float>(Settings::GetInstance()->half_height)}, 0.f,
                120,
                1.2f);
        const auto drawing = Drawing(sc, &player);

        while (!rg::WindowCloseOrQuit())
        {
            dt = rl::GetFrameTime();

            sc->Fill(settings->black);
            drawing.background();
            drawing.world();

            player.movement(dt, sc);

            drawing.fps(dt);

            rg::display::Update();
        }

        Settings::Destroy();
    }

    rg::Quit();
}
