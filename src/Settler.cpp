#include "Settler.h"
#include <iostream>

Settler::Settler()
{
    name = "Settler";
    production_cost = 5;
    render_icon = 'S';
}

Settler::~Settler()
{
    //dtor
}

void Settler::action (worldMap &world, std::vector <Unit*> &units) {
    is_founding_city = true;
    movement_points = 0;
    std::cout << "Settler is founding a city." << std::endl;
}
