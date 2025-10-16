#pragma once
#include <rygame.hpp>

#include "drawing.hpp"
#include "player.hpp"

bool ray_casting_npc_player(float npc_x, float npc_y, rg::math::Vector2<float> player_pos);

class Interaction
{
public:

    Interaction(Player *player, Sprites *sprites, Drawing *drawing);
    void interation_objects() const;
    void npc_action() const;

private:

    Player *player;
    Sprites *sprites;
    Drawing *drawing;
};
