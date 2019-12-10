#include "Scout.h"

Scout::Scout()
{
    name = "Scout";
    max_move_points = 4;
    movement_points = 4;
    health = 1;
    maxHealth = 1;
    combat_strength = 1;
    render_icon = '>';
}

Scout::~Scout()
{
    //dtor
}
