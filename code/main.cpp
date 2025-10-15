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

        Sprites sprites{};
        auto player = Player(
                &sprites,
                {static_cast<float>(settings->half_width) / 4,
                 static_cast<float>(settings->half_height) - 50.0f},
                0.f, 120, 1.2f, 0.3f);
        auto drawing = Drawing(sc, &sc_map, &player);
        SpriteProjection wall_center{}, closest_obj{};


        SpriteParameter sprite_sfx_params;
        sprite_sfx_params.sprite.emplace_back(
                rg::image::Load("resources/sprites/weapons/sfx/0.png"));
        sprite_sfx_params.shift = 0.5f;
        sprite_sfx_params.scale = 1.0f;
        for (int i = 0; i < 9; ++i)
        {
            std::string path = std::string("resources/sprites/weapons/sfx/") + std::to_string(i) +
                               ".png";
            sprite_sfx_params.animation.emplace_back(rg::image::Load(path.c_str()));
        }
        sprite_sfx_params.animation_dist = 80000.0f;
        sprite_sfx_params.animation_speed = 5;

        SpriteObject sfx{&sprite_sfx_params, rg::math::Vector2{800.0f, 800.0f}};

        while (!rg::WindowCloseOrQuit())
        {
            dt = rl::GetFrameTime();

            player.movement(dt);

            sc->Fill(settings->black);

            drawing.background();
            auto walls = ray_casting_depth(&sc_map, &player, &drawing.textures);

            if (rl::IsMouseButtonPressed(rl::MOUSE_BUTTON_LEFT) && !player.shot)
            {
                player.shoot();
                wall_center = {.depth = walls[settings->center_ray].depth,
                               .proj_height = walls[settings->center_ray].
                                              sprite_dimension.y,
                               .x = walls[settings->center_ray].x,
                               .y = walls[settings->center_ray].y
                };
                closest_obj = sprites.closest_sprite_projection();
                if (wall_center.depth < closest_obj.depth)
                {
                    sfx.x = wall_center.x;
                    sfx.y = wall_center.y;
                }
                else
                {
                    sfx.x = closest_obj.x;
                    sfx.y = closest_obj.y;
                }
            }

            static std::vector<SpriteObjectLocate> locates;
            locates.clear();
            locates.reserve(walls.size() + sprites.list_of_objects.size());
            locates.insert(locates.end(), walls.begin(), walls.end());
            for (auto &obj: sprites.list_of_objects)
            {
                locates.emplace_back(obj.object_locate(&player, dt));
            }
            drawing.world(locates);
            drawing.mini_map();

            if (!player.shot)
            {
                sfx.x = 800.0f;
                sfx.y = 800.0f;
            }
            else
            {
                auto sfx_locate = sfx.object_locate(&player, dt);
                sc->Blit(
                        sfx_locate.sprite, sfx_locate.sprite_pos, sfx_locate.sprite_area,
                        rl::BLEND_ALPHA,
                        sfx_locate.sprite_dimension.x, sfx_locate.sprite_dimension.y);
            }
            drawing.player_weapon(dt, &sfx);

            drawing.fps(dt);

            rg::display::Update();
        }

        Settings::Destroy();
    }

    rg::Quit();
}
