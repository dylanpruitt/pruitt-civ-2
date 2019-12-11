#include "Unit.h"
#include <iostream>
#include <math.h>

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
    std::cout << "\nAttack with which unit? " << std::endl;

    int which_one = 0;

    std::vector<int> valid_enemy_unit_indices;

    for (unsigned int i = 0; i < units.size(); i++) {
        if (units [i]->owner_index != owner_index && this->distanceFrom (units [i]->x, units [i]->y) < 2) {
            valid_enemy_unit_indices.push_back (i);
        }
    }

    if (valid_enemy_unit_indices.size() >= 1) {
        std::cout << "Attack which unit?" << std::endl;

        for (unsigned int i = 0; i < valid_enemy_unit_indices.size(); i++) {
            Unit* enemy_unit = units [valid_enemy_unit_indices[i]];
            std::cout << "[" << i << "] " << enemy_unit->name << " (" << enemy_unit->x << ", " << enemy_unit->y << ") - "
                << enemy_unit->health << " HP" << std::endl;
        }

        std::cin >> which_one;

        if (which_one >= 0 && which_one < valid_enemy_unit_indices.size ()) {
            attack (units [valid_enemy_unit_indices [which_one]]);
        } else {
            std::cout << "Invalid target!" << std::endl;
        }

    } else {
        std::cout << "You can't attack any other units with this unit!" << std::endl;
    }
}

int Unit::distanceFrom (int x, int y) {
    return (int) sqrt (pow (this->x - x, 2) + pow (this->y - y, 2));
}

void Unit::attack (Unit* &target) {

    health -= target->combat_strength;
    target->health -= combat_strength;

    std::cout << name << " dealt " << combat_strength << " to the defender." << std::endl;
    std::cout << target->name << " dealt " << target->combat_strength << " to the attacker." << std::endl;
}
