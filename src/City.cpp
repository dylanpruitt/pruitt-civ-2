#include <random>
#include <iostream>
#include <math.h>
#include "City.h"
#include "CityTile.h"

City::City(int x, int y, int owner_index, worldMap &world)
{
    world.tiles[x + y * world.map_size] = new CityTile ();
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (x+i >= 0 && x+i < world.map_size && y+j >= 0 && y+j < world.map_size) {
                world.tiles[x + i + (y + j) * world.map_size]->ownerIndex = owner_index;
            }
        }
    }

    this->x = x;
    this->y = y;
    this->owner_index = owner_index;
}

City::~City()
{
    //dtor
}

void City::expand (worldMap &world) {
    if (this->canExpand(world)) {
        bool tileFound = false;
        while (!tileFound) {

            int i = rand() % 7 - 3;
            int j = rand() % 7 - 3;

            if (x+i >= 0 && x+i < world.map_size && y+j >= 0 && y+j < world.map_size) {
                if (tileIsBorderedByCivilization (world, this->owner_index, this->x + i, this->y + j)
                && world.tiles[x + i + (y + j) * world.map_size]->ownerIndex == -1) {
                    world.tiles[x + i + (y + j) * world.map_size]->ownerIndex = this->owner_index;
                    tileFound = true;
                }
            }
        }
    }
}

bool City::canExpand (worldMap world) {
    for (int i = -3; i < 4; i++) {
        for (int j = -3; j < 4; j++) {
            if (x+i >= 0 && x+i < world.map_size && y+j >= 0 && y+j < world.map_size) {
                if (tileIsBorderedByCivilization (world, owner_index, this->x + i, this->y + j)
                    && world.tiles[x + i + (y + j) * world.map_size]->ownerIndex == -1) {
                    return true;
                }
            }
        }
    }
    return false;
}

void City::update_population (worldMap &world) {
    update_yields (world);

    food_stockpile += total_food_yield;

    if (food_stockpile >= (int) (pow (2, population))) {
        food_stockpile -= (int) (pow (2, population));
        population++;
        expand (world);
    }
}

void City::update_yields (worldMap &world) {
    int new_food_yield = 0, new_production_yield = 0;
    for (int i = -3; i < 4; i++) {
        for (int j = -3; j < 4; j++) {
            if (x+i >= 0 && x+i < world.map_size && y+j >= 0 && y+j < world.map_size) {
                if (world.tiles[x+i + (y + j) * world.map_size]->ownerIndex == owner_index) {
                    new_food_yield += world.tiles[x+i + (y + j) * world.map_size]->getFoodYield();
                    new_production_yield += world.tiles[x+i + (y + j) * world.map_size]->getProductionYield();
                }
            }
        }
    }

    total_food_yield = new_food_yield;
    total_production_yield = new_production_yield;
}

bool City::tileIsBorderedByCivilization (worldMap world, int civilizationIndex, int x, int y) {
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (x+i >= 0 && x+i < world.map_size && y+j >= 0 && y+j < world.map_size) {
                if (world.tiles[x + i + (y + j) * world.map_size]->ownerIndex == civilizationIndex) {
                    return true;
                }
            }
        }
    }
    return false;
}
