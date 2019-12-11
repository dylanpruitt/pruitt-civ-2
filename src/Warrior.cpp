#include "Warrior.h"

Warrior::Warrior()
{
    name = "Warrior";
    max_move_points = 2;
    movement_points = 2;
    health = 3;
    maxHealth = 3;
    combat_strength = 2;
    production_cost = 17;
    render_icon = 'W';
}

Warrior::~Warrior()
{
    //dtor
}
