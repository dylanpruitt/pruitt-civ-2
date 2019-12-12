#include "Settler.h"
#include "utility.h"
#include <iostream>

Settler::Settler()
{
    name = "Settler";
    production_cost = 25;
    render_icon = 'S';
}

Settler::~Settler()
{
    //dtor
}

void Settler::action (worldMap &world, std::vector <Unit*> &units) {
    if (get_distance_of_closest_city (world) >= 3) {
        is_founding_city = true;
        movement_points = 0;
        std::cout << "Settler is founding a city." << std::endl;
    } else {
        std::cout << "Settler is too close to other cities to found a new city!" << std::endl;
    }

}

int Settler::get_distance_of_closest_city (worldMap &world) {
    int closest_distance = 1000;

    for (int i = 0; i < world.map_size; i++) {
        for (int j = 0; j < world.map_size; j++) {
            char city_character = '!';
            if (world.tiles [i + j * world.map_size]->render_character == city_character) {
                int distance = utility::get_distance (this->x, this->y, i, j);
                if (distance < closest_distance) {
                    closest_distance = distance;
                }
            }
        }
    }

    return closest_distance;
}
