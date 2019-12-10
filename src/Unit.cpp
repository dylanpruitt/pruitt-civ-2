#include "Unit.h"
#include <iostream>

Unit::Unit()
{
    //ctor
}

Unit::~Unit()
{
    //dtor
}

void Unit::move (worldMap &world, int delta_x, int delta_y) {
    x += delta_x;
    y += delta_y;

    movement_points -= world.tiles[x + y * world.map_size]->getMovementCost ();
    world.tiles[x + y * world.map_size]->ownerIndex = owner_index;
}

void Unit::action (worldMap &world, std::vector <Unit*> &units) {

}
