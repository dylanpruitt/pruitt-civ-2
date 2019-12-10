#include "Game.h"
#include <iostream>
#include <time.h>
#include "Settler.h"
#include "textGraphics.h"
#include "worldMap.h"

Game::Game()
{
    std::cout << "Input map size:";
    std::cin >> map_size;

    world = worldMap (map_size);

    std::cout << "Input number of players:";
    std::cin >> number_of_civilizations;

    if (number_of_civilizations <= 0 || number_of_civilizations > 8) {
        number_of_civilizations = 2;
    }

    for (int i = 0; i < number_of_civilizations; i++) {
        civilizations.push_back (Civilization ());
        std::cout << "Enter player " << i << " name:";
        std::cin >> civilizations[i].name;

        int x = 0, y = 0;
        bool spawning = true;

        while (spawning) {
            x = rand () % (map_size - 2) + 1;
            y = rand () % (map_size - 2) + 1;
            if (world.tiles [x + y * map_size]->ownerIndex == -1) {
                spawning = false;
            }
        }

        City city (x, y, i, world);
        city.name = civilizations [i].name;
        civilizations [i].cities.push_back (city);

        Settler* settler = new Settler ();
        settler->x = x;
        settler->y = y;
        settler->owner_index = i;
        units.push_back (settler);
    }

}

Game::~Game()
{
    //dtor
}

void Game::loop () {
    bool looping = true;
    while (looping) {
        for (int i = 0; i < civilizations.size (); i++) {
            bool civilization_still_playing = true;
            while (civilization_still_playing) {
                world.render ();
                std::cout << "=== " << civilizations [i].name << " ===" << std::endl;
                displayCivilizationInformation ();

                char input = ' ';
                std::cin >> input;

                if (input == 'u') {
                    interactWithUnits (i);
                }

                if (input == 'e') {
                    civilization_still_playing = false;
                }
            }
        }

        for (int i = 0; i < civilizations.size (); i++) {
            for (int j = 0; j < civilizations[i].cities.size (); j++) {
                civilizations[i].cities[j].update_population (world);
            }
        }

        for (int i = 0; i < units.size (); i++) {
            units [i]->movement_points = units [i]->max_move_points;
        }

        foundNewCities ();
        updateCapturedCities ();

        removeEliminatedCivilizations ();
    }
}

void Game::interactWithUnits (int civilization_index) {
    std::vector <int> unit_indices;

    std::cout << "UNITS" << std::endl;

    for (unsigned int j = 0; j < units.size (); j++) {
        int owned_unit_count = 0;

        if (units [j]->owner_index == civilization_index) {
            std::cout << owned_unit_count << ": " << units [j]->name << " - " << units [j]->x << ", "
                << units [j]->y << " | MOVEMENT LEFT : " << units [j]->movement_points << std::endl;
            owned_unit_count++;
            unit_indices.push_back (j);
        }
    }

    char choice;
    std::cin >> choice;

    if (choice == 'm' && unit_indices.size () >= 1) {
        std::cout << "\nMove which unit? " << std::endl;

        int which_one;
        std::cin >> which_one;

        while (units [unit_indices[which_one]]->movement_points > 0) {
            std::cout << "[1] move down [2] move right [3] move up [4] move left" << std::endl;

            int direction;
            std::cin >> direction;

            if (direction == 1 && units [unit_indices[which_one]]->x < world.map_size) {
                units [unit_indices[which_one]]->move (world, 1, 0);
            }

            if (direction == 2 && units [unit_indices[which_one]]->y < world.map_size) {
                units [unit_indices[which_one]]->move (world, 0, 1);
            }

            if (direction == 3 && units [unit_indices[which_one]]->x > 0) {
                units [unit_indices[which_one]]->move (world, -1, 0);
            }

            if (direction == 4 && units [unit_indices[which_one]]->y > 0) {
                units [unit_indices[which_one]]->move (world, 0, -1);
            }
        }

        } else if (choice == 'a' && unit_indices.size () >= 1) {
            std::cout << "\nUse which unit's special ability? " << std::endl;

            int which_one;
            std::cin >> which_one;

            units [unit_indices[which_one]]->action (world, units);
        }
}

void Game::displayCivilizationInformation () {
    for (int i = 0; i < civilizations.size (); i++) {
        std::cout << civilizations [i].name << " ";
        textGraphics::changeTextColor (textGraphics::colors::BRIGHT_WHITE, i + 1);
        std::cout << " ";
        textGraphics::changeTextColor (textGraphics::colors::WHITE, textGraphics::colors::BLACK);
        std::cout << " (" << getPercentageOfWorldControlled (i) << "%)" << std::endl;
        for (int j = 0; j < civilizations [i].cities.size (); j++) {
            City city = civilizations [i].cities [j];
            std::cout << " -- " << city.name << " (" << city.population << ") : " << city.total_food_yield << " food, " << city.total_production_yield << " production" << std::endl;
        }
    }
}

int Game::getPercentageOfWorldControlled (int civilization_index) {
    int total_world_tiles = world.map_size * world.map_size;
    int owned_tiles = 0;

    for (int i = 0; i < world.tiles.size (); i++) {
        if (world.tiles [i]->ownerIndex == civilization_index) {
            owned_tiles++;
        }
    }

    return ((double)owned_tiles / (double)total_world_tiles) * 100;
}

void Game::foundNewCities () {
    std::vector <Unit*> remaining_units;

    for (int i = 0; i < units.size (); i++) {
        if (units [i]->is_founding_city) {
            City city (units [i]->x, units [i]->y, units [i]->owner_index, world);
            city.name = civilizations [units [i]->owner_index].name + " " + std::to_string(civilizations [units [i]->owner_index].cities.size () + 1);
            civilizations [units [i]->owner_index].cities.push_back (city);
        } else {
            remaining_units.push_back (units [i]);
        }
    }

    units.swap (remaining_units);
}

void Game::updateCapturedCities () {
    for (int i = 0; i < civilizations.size (); i++) {
        for (int j = 0; j < civilizations [i].cities.size (); j++) {
            int city_x = civilizations [i].cities [j].x, city_y = civilizations [i].cities [j].y;
            if (world.tiles [city_x + city_y * world.map_size]->ownerIndex != i) {
                int new_owner_index = world.tiles [city_x + city_y * world.map_size]->ownerIndex;
                City captured_city = civilizations [i].cities [j];
                captured_city.owner_index = new_owner_index;

                civilizations [new_owner_index].cities.push_back (captured_city);
                civilizations [i].cities.erase (civilizations [i].cities.begin () + j);
            }
        }
    }
}

void Game::removeEliminatedCivilizations () {
    for (int i = 0; i < civilizations.size (); i++) {
        if (civilizations [i].cities.size () == 0) {
            for (int j = 0; j < world.tiles.size (); j++) {
                if (world.tiles [j]->ownerIndex == i) {
                    world.tiles [j]->ownerIndex = -1;
                }
            }
            civilizations.erase (civilizations.begin () + i);
        }
    }
}
