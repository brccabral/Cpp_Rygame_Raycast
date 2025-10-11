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
        auto sc_map = rg::Surface(
                settings->width / settings->map_scale, settings->height / settings->map_scale);

        float dt = 0;

        auto player = Player(
                {static_cast<float>(Settings::GetInstance()->half_width) / 4,
                 static_cast<float>(Settings::GetInstance()->half_height) - 50.0f}, 0.f,
                120,
                1.2f);
        auto drawing = Drawing(sc, &sc_map, &player);

        while (!rg::WindowCloseOrQuit())
        {
            dt = rl::GetFrameTime();

            player.movement(dt, sc);

            sc->Fill(settings->black);

            drawing.background();
            drawing.world();
            drawing.mini_map();
            drawing.fps(dt);

            rg::display::Update();
        }

        Settings::Destroy();
    }

    rg::Quit();
}
