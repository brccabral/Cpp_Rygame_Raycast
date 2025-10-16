#pragma once
#include <rygame.hpp>

#include "drawing.hpp"
#include "player.hpp"

bool ray_casting_npc_player(
        float npc_x, float npc_y, rg::math::Vector2<float> player_pos,
        std::unordered_map<rg::math::Vector2<int>, int> blocked_doors);

class Interaction
{
public:

    Interaction(Player *player, Sprites *sprites, Drawing *drawing);
    void interation_objects() const;
    void npc_action(float dt) const;
    void clear_world() const;

private:

    Player *player;
    Sprites *sprites;
    Drawing *drawing;

    void npc_move(SpriteObject *obj, float dt) const;

    rg::mixer::Sound pain_sound{};
};
