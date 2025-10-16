#include <rygame.hpp>

#include "drawing.hpp"
#include "interaction.hpp"
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
        auto interaction = Interaction(&player, &sprites, &drawing);

        SpriteProjection wall_center{}, closest_obj{};


        SpriteParameter sprite_sfx_params;
        sprite_sfx_params.sprite.emplace_back(
                rg::image::Load("resources/sprites/weapons/sfx/0.png"));
        sprite_sfx_params.shift = 0.5f;
        sprite_sfx_params.scale = {1.0f, 1.0f};
        for (int i = 0; i < 9; ++i)
        {
            std::string path = std::string("resources/sprites/weapons/sfx/") + std::to_string(i) +
                               ".png";
            sprite_sfx_params.animation.emplace_back(rg::image::Load(path.c_str()));
        }
        sprite_sfx_params.animation_dist = 80000.0f;
        sprite_sfx_params.animation_speed = 15;

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
                drawing.shot_animation_trigger = true;
                sfx.animation_index = 0;
                wall_center = {.depth = walls[settings->center_ray].depth,
                               .dimensions = walls[settings->center_ray].sprite_dimension,
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

            if (player.shot)
            {
                auto sfx_locate = sfx.object_locate(&player, dt);
                if (sfx.animation_index == 0)
                {
                    sfx.x = 8000.0f;
                    sfx.y = 8000.0f;
                }
                else
                {
                    sc->Blit(
                            sfx_locate.sprite, sfx_locate.sprite_pos, sfx_locate.sprite_area,
                            rl::BLEND_ALPHA,
                            sfx_locate.sprite_dimension.x, sfx_locate.sprite_dimension.y);
                }
            }
            drawing.player_weapon(dt);

            drawing.fps(dt);

            interaction.interation_objects();
            interaction.npc_action(dt);
            drawing.shot_animation_trigger = false;

            rg::display::Update();
        }

        Settings::Destroy();
    }

    rg::Quit();
}
