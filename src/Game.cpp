#include "Game.h"
#include <iostream>
#include <time.h>
#include "Settler.h"
#include "Scout.h"
#include "Warrior.h"
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
                render ();
                std::cout << "=== " << civilizations [i].name << " ===" << std::endl;
                displayCivilizationInformation ();

                char input = ' ';
                std::cin >> input;

                if (input == 'u') {
                    interactWithUnits (i);
                } else if (input == 'p') {
                    produceUnits (i);
                } else if (input == 'e') {
                    civilization_still_playing = false;
                }
            }
        }

        for (int i = 0; i < civilizations.size (); i++) {
            for (int j = 0; j < civilizations [i].cities.size (); j++) {
                civilizations [i].cities [j].update_population (world);

                civilizations [i].production += civilizations [i].cities [j].total_production_yield;
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

void Game::render () {
    for (int i = 0; i < world.map_size; i++) {
        for (int j = 0; j < world.map_size; j++) {
            int index = i + j * world.map_size;
            int NOT_OWNED_BY_CIVILIZATION = -1;

            if (world.tiles [index]->ownerIndex != NOT_OWNED_BY_CIVILIZATION) {
                textGraphics::changeTextColor (textGraphics::colors::BRIGHT_WHITE, world.tiles[index]->ownerIndex + 1);
            } else {
                textGraphics::changeTextColor (textGraphics::colors::BRIGHT_WHITE, textGraphics::colors::BLACK);
            }

            if (unitIsAtPosition (i, j)) {
                int unit_index = getUnitIndexAtPosition (i, j);
                std::cout << units [unit_index]->render_icon;
            } else {
                std::cout << world.tiles [index]->render_character;
            }
        }
        std::cout << "\n";
    }
    textGraphics::changeTextColor (textGraphics::colors::WHITE, textGraphics::colors::BLACK);
}

void Game::interactWithUnits (int civilization_index) {
    std::vector <int> unit_indices;

    std::cout << "UNITS" << std::endl;
    int owned_unit_count = 0;

    for (unsigned int j = 0; j < units.size (); j++) {

        if (units [j]->owner_index == civilization_index) {
            std::cout << owned_unit_count << ": " << units [j]->name << " - ("
                << units [j]->render_icon << ") | " << units [j]->x << ", "
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
            std::cout << "[1] move down [2] move right [3] move up [4] move left [5] end movement" << std::endl;

            int direction;
            std::cin >> direction;

            int unit_x = units [unit_indices[which_one]]->x;
            int unit_y = units [unit_indices[which_one]]->y;
            if (direction == 1 && unit_x < world.map_size
                && !unitIsAtPosition (unit_x + 1, unit_y)) {
                units [unit_indices[which_one]]->move (world, 1, 0);
            }

            if (direction == 2 && unit_y < world.map_size
                && !unitIsAtPosition (unit_x, unit_y + 1)) {
                units [unit_indices[which_one]]->move (world, 0, 1);
            }

            if (direction == 3 && unit_x > 0
                && !unitIsAtPosition (unit_x - 1, unit_y)) {
                units [unit_indices[which_one]]->move (world, -1, 0);
            }

            if (direction == 4 && unit_y > 0
                && !unitIsAtPosition (unit_x, unit_y - 1)) {
                units [unit_indices[which_one]]->move (world, 0, -1);
            }
            if (direction == 5) {
                units [unit_indices[which_one]]->movement_points = 0;
            }
        }

        } else if (choice == 'a' && unit_indices.size () >= 1) {
            std::cout << "\nUse which unit's special ability? " << std::endl;

            int which_one;
            std::cin >> which_one;

            units [unit_indices[which_one]]->action (world, units);
        }
}

void Game::produceUnits (int civilization_index) {
    std::cout << "PRODUCTION" << std::endl;

    bool still_producing = true;
    std::vector <Unit*> units_for_production;
    units_for_production.push_back (new Scout ());
    units_for_production.push_back (new Warrior ());
    units_for_production.push_back (new Settler ());

    while (still_producing) {
        std::cout << "Your civilization has " << civilizations [civilization_index].production << " production saved up." << std::endl;
        std::cout << "Produce...\nUnit (Production Cost)" << std::endl;
        for (int i = 0; i < units_for_production.size (); i++) {
            std::cout << "[" << i << "] " << units_for_production [i]->name << " (" << units_for_production [i]->production_cost << ")" << std::endl;
        }
        std::cout << "[" << units_for_production.size () << "] - Quit producing" << std::endl;

        int unit_index = 0;
        std::cin >> unit_index;

        if (unit_index == units_for_production.size ()) {
            still_producing = false;
        } else if (unit_index >= 0 && unit_index < units_for_production.size () && civilizations [civilization_index].cities.size () >= 1) {
            if (civilizations [civilization_index].production < units_for_production [unit_index]->production_cost) {
                std::cout << "You can't produce that!" << std::endl;
            } else {
                civilizations [civilization_index].production -= units_for_production [unit_index]->production_cost;

                std::cout << "Produce unit in which city?" << std::endl;
                for (int j = 0; j < civilizations [civilization_index].cities.size (); j++) {
                    City city = civilizations [civilization_index].cities [j];
                    std::cout << "[" << j << "] " << city.name << " (" << city.x << ", " << city.y << ")" << std::endl;
                }

                int city_index = 0;
                std::cin >> city_index;

                if (city_index >= 0 && city_index < civilizations [civilization_index].cities.size ()) {
                    Unit* unit = units_for_production [unit_index];
                    unit->owner_index = civilization_index;
                    unit->x = civilizations [civilization_index].cities [city_index].x;
                    unit->y = civilizations [civilization_index].cities [city_index].y;
                    units.push_back (unit);
                }
            }
        }
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

bool Game::unitIsAtPosition (int x, int y) {
    for (int i = 0; i < units.size (); i++) {
        if (units [i]->x == x && units [i]->y == y) {
            return true;
        }
    }
    return false;
}

int Game::getUnitIndexAtPosition (int x, int y) {
    for (int i = 0; i < units.size (); i++) {
        if (units [i]->x == x && units [i]->y == y) {
            return i;
        }
    }

    int UNIT_NOT_FOUND = -1;
    return UNIT_NOT_FOUND;
}