#pragma once
#include <deque>
#include <rygame.hpp>
#include "maplevels.hpp"
#include "player.hpp"
#include "settings.hpp"
#include "sprite_objects.hpp"


class Drawing
{
public:

    Drawing(rg::Surface *sc, rg::Surface *sc_map, Player *player);

    void background();
    void world(std::vector<SpriteObjectLocate> &locates) const;
    void fps(float dt);
    void mini_map(const Sprites *sprites) const;
    void player_weapon(float dt);

    rg::Surface *sc{};
    rg::Surface *sc_map{};
    Settings *settings{};
    MapLevels *map_levels{};
    Player *player{};
    rg::Surface fps_text_surface{};
    rg::font::Font font{"/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed-Bold.ttf", 36};
    std::unordered_map<int, rg::Surface> textures{};
    rg::Surface sky_surface{};

    rg::Surface *weapon_sprite{};
    rg::Surface weapon_base_sprite{};
    std::deque<rg::Surface> weapon_shot_animation{};
    rg::Rect weapon_rect{};
    rg::math::Vector2<float> weapon_pos{};
    float shot_length_index{};
    float shot_animation_speed = 24.0f;
    float shot_animation_index{};
    bool shot_animation_trigger = true;
};
