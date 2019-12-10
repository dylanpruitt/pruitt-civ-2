#ifndef UNIT_H
#define UNIT_H
#include <string>
#include <vector>
#include "worldMap.h"

class Unit
{
    public:

        Unit ();
        virtual ~Unit ();

        std::string name;

        int x, y;

        int health = 3; int maxHealth = 3;

        int owner_index = 0;

        int combat_strength = 1;

        int movement_points = 2;

        int max_move_points = 2;

        int population_cost = 1;
        int production_cost = 1;

        bool is_founding_city = false;

        char render_icon = '@';

        void move (worldMap &world, int delta_x, int delta_y);

        virtual void action (worldMap &world, std::vector <Unit*> &units);
    private:
};

#endif // UNIT_H
