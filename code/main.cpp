#include <rygame.hpp>

#include "drawing.hpp"
#include "settings.hpp"
#include "player.hpp"
#include "ray_casting.hpp"
#include "sprite_objects.hpp"


int main()
{
    rg::Init();

    {
        const Settings *settings = Settings::GetInstance();

        auto *sc = &rg::display::SetMode(settings->width, settings->height);
        auto sc_map = rg::Surface(settings->minimap_res.x, settings->minimap_res.y);

        rl::HideCursor();
        float dt = 0;

        auto player = Player(
                {static_cast<float>(settings->half_width) / 4,
                 static_cast<float>(settings->half_height) - 50.0f},
                0.f, 120, 1.2f, 0.3f);
        auto drawing = Drawing(sc, &sc_map, &player);
        Sprites sprites{};

        while (!rg::WindowCloseOrQuit())
        {
            dt = rl::GetFrameTime();

            player.movement(dt);

            sc->Fill(settings->black);

            drawing.background();
            auto walls = ray_casting_depth(&sc_map, &player, &drawing.textures);
            static std::vector<SpriteObjectLocate> locates;
            locates.clear();
            locates.reserve(walls.size() + sprites.list_of_objects.size());
            locates.insert(locates.end(), walls.begin(), walls.end());
            for (auto &obj: sprites.list_of_objects)
            {
                locates.emplace_back(obj.object_locate(&player, settings, walls));
            }
            drawing.world(locates);
            drawing.mini_map();
            drawing.fps(dt);

            rg::display::Update();
        }

        Settings::Destroy();
    }

    rg::Quit();
}
