#include "worldMap.h"
#include "textGraphics.h"
#include "CityTile.h"
#include "MountainTile.h"
#include "ForestTile.h"
#include "PlainsTile.h"
#include <iostream>
#include <random>
#include <time.h>

worldMap::worldMap()
{
    //ctor
}

worldMap::~worldMap()
{
    //dtor
}

void worldMap::render () {
    for (int i = 0; i < map_size; i++) {
        for (int j = 0; j < map_size; j++) {
            int index = i + j * map_size;
            int NOT_OWNED_BY_CIVILIZATION = -1;
            if (tiles[index]->ownerIndex != NOT_OWNED_BY_CIVILIZATION) {
                textGraphics::changeTextColor (textGraphics::colors::BRIGHT_WHITE, tiles[index]->ownerIndex + 1);
            } else {
                textGraphics::changeTextColor (textGraphics::colors::BRIGHT_WHITE, textGraphics::colors::BLACK);
            }
            std::cout << tiles[index]->render_character;
        }
        std::cout << "\n";
    }
    textGraphics::changeTextColor (textGraphics::colors::WHITE, textGraphics::colors::BLACK);
}

void worldMap::generate (int map_size) {

    for (int i = 0; i < map_size * map_size; i++) {
        tiles.push_back (new PlainsTile ());
    }

    render ();
    srand (time (NULL));

    int number_of_forest_tiles = rand () % ((map_size * map_size / 4) - 1) + 1;
    int number_of_mountain_tiles = rand () % ((map_size * map_size / 4) - 1) + 1;

    if (map_size >= 5) {
        number_of_forest_tiles = rand () % ((map_size * map_size / 4) - 1) + 1;
        number_of_mountain_tiles = rand () % ((map_size * map_size / 4) - 1) + 1;
    } else {
        number_of_forest_tiles = 2;
        number_of_mountain_tiles = 2;
    }


    int number_of_forest_tiles_filled = 0;
    while (number_of_forest_tiles_filled < number_of_forest_tiles) {
        int spawn_x = rand () % (map_size - 2) + 1;
        int spawn_y = rand () % (map_size - 2) + 1;

        int index = spawn_x + spawn_y * map_size;

        if (tiles[index]->render_character == ',') {
            tiles[index] = new ForestTile ();
            number_of_forest_tiles_filled++;
        }
    }

    int number_of_mountain_tiles_filled = 0;
    while (number_of_mountain_tiles_filled < number_of_mountain_tiles) {
        int spawn_x = rand () % (map_size - 2) + 1;
        int spawn_y = rand () % (map_size - 2) + 1;

        int index = spawn_x + spawn_y * map_size;

        if (tiles[index]->render_character == ',') {
            tiles[index] = new MountainTile ();
            number_of_mountain_tiles_filled++;
        }
    }
}
