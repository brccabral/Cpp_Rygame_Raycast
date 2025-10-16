#include "drawing.hpp"
#include "ray_casting.hpp"


Drawing::Drawing(rg::Surface *sc, rg::Surface *sc_map, Player *player)
    : sc(sc), sc_map(sc_map), settings(Settings::GetInstance()),
      map_levels(MapLevels::GetInstance()), player(player)
{
    textures[1] = rg::image::Load("resources/images/wall3.png");
    textures[2] = rg::image::Load("resources/images/wall4.png");
    textures[3] = rg::image::Load("resources/images/wall5.png");
    textures[4] = rg::image::Load("resources/images/wall6.png");
    sky_surface = rg::image::Load("resources/images/sky1.png");

    weapon_base_sprite = rg::image::Load("resources/sprites/weapons/shotgun/base/0.png");
    for (int i = 0; i < 20; ++i)
    {
        std::string path = "resources/sprites/weapons/shotgun/shot/" + std::to_string(i) + ".png";
        weapon_shot_animation.emplace_back(rg::image::Load(path.c_str()));
    }
    weapon_rect = weapon_base_sprite.GetRect();
    weapon_pos = {settings->half_width - weapon_rect.width / 2,
                  settings->height - weapon_rect.height};

    shot_sound = rg::mixer::Sound("resources/sound/shotgun.wav");
}

void Drawing::background()
{
    const auto sky_offset = -10 * static_cast<int>(player->angle * 180.0f / M_PI) % settings->width;
    sc->Blit(&sky_surface, rg::math::Vector2{sky_offset, 0});
    sc->Blit(&sky_surface, rg::math::Vector2{sky_offset - settings->width, 0});
    sc->Blit(&sky_surface, rg::math::Vector2{sky_offset + settings->width, 0});
    rg::draw::rect(
            sc, settings->darkgray,
            {0, settings->half_height, settings->width, settings->half_height});
    sc_map->Fill(settings->black);
}

void Drawing::world(std::vector<SpriteObjectLocate> &locates) const
{
    std::ranges::sort(
            locates, [](
            const SpriteObjectLocate &locate1, const SpriteObjectLocate &locate2)
            {
                return locate1.depth > locate2.depth;
            });
    for (const auto &[depth,
             sprite,
             sprite_dimension,
             sprite_pos,
             sprite_area, x, y]: locates)
    {
        if (depth)
        {
            sc->Blit(
                    sprite, sprite_pos, sprite_area, rl::BLEND_ALPHA,
                    sprite_dimension.x, sprite_dimension.y);
        }
    }
}

void Drawing::fps(const float dt)
{
    fps_text_surface = font.render(rl::TextFormat("%.1f", 1.0f / dt), settings->darkorange);
    sc->Blit(&fps_text_surface, settings->fps_pos);
}

void Drawing::mini_map(const Sprites *sprites) const
{
    // scale player position
    auto [player_x_map, player_y_map] = rg::math::Vector2{player->x / settings->map_scale,
                                                          player->y / settings->map_scale};

    // draw walls
    for (auto [x, y]: map_levels->mini_map | std::views::keys)
    {
        rg::draw::rect(
                sc_map, settings->darkbrown, {x, y, settings->map_tile, settings->map_tile});
    }

    // player position
    rg::draw::circle(sc_map, settings->red, {player_x_map, player_y_map}, 5);
    // player direction
    rg::draw::line(
            sc_map, settings->green, {player_x_map, player_y_map}, {
                    player_x_map + settings->width / settings->map_scale * cosf(player->angle),
                    player_y_map + settings->width / settings->map_scale * sinf(player->angle)});

    // npc position
    for (auto &sprite: sprites->list_of_objects)
    {
        if (sprite.flag == SpriteFlagType::FLAG_NPC
            && sprite.is_dead == SpriteStatus::STATUS_ALIVE)
        {
            rg::draw::circle(
                    sc_map, settings->green,
                    {sprite.pos().x / settings->map_scale, sprite.pos().y / settings->map_scale},
                    5);
        }
    }

    sc->Blit(sc_map, settings->map_pos);
}

void Drawing::player_weapon(const float dt)
{
    if (player->shot)
    {
        if (shot_animation_index == 0)
        {
            shot_sound.Play();
        }
        weapon_sprite = &weapon_shot_animation[int(shot_animation_index)];
        shot_animation_index += shot_animation_speed * dt;
        if (shot_animation_index >= weapon_shot_animation.size())
        {
            shot_animation_index = 0;
            player->shot = false;
        }
    }
    else
    {
        weapon_sprite = &weapon_base_sprite;
    }
    sc->Blit(weapon_sprite, weapon_pos);
}
