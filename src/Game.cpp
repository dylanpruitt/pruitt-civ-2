#include "Game.h"
#include <iostream>
#include <time.h>
#include <fstream>
#include "Settler.h"
#include "Scout.h"
#include "Warrior.h"
#include "PlainsTile.h"
#include "ForestTile.h"
#include "CityTile.h"
#include "MountainTile.h"
#include "textGraphics.h"
#include "utility.h"
#include "worldMap.h"

Game::Game()
{
    std::cout << "Load game? [Y/N]" << std::endl;

    std::string input;
    std::cin >> input;

    if (input == "Y") {
        std::cout << "Enter a filename to load from: ";
        std::string file_name;
        std::cin >> file_name;

        load_game (file_name);
    }
    if (loaded == false) {
        std::cout << "Input map size (8 - 40):";
        map_size = utility::integer_input ();

        if (map_size < 8 || map_size > 40) {
            map_size = 16;
        }
        world = worldMap (map_size);

        std::cout << "Input number of players:";
        number_of_civilizations = utility::integer_input ();

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
                } else if (input == 's') {
                    std::cout << "Enter a filename to save to: ";
                    std::string file_name;
                    std::cin >> file_name;

                    save_game (file_name);
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
        textGraphics::changeTextColor (textGraphics::colors::WHITE, textGraphics::colors::BLACK);
        std::cout << "\n";
    }
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

        int which_one = utility::integer_input ();

        while (units [unit_indices[which_one]]->movement_points > 0) {
            std::cout << "[1] move down [2] move right [3] move up [4] move left [5] end movement" << std::endl;

            int direction = utility::integer_input ();

            int unit_x = units [unit_indices[which_one]]->x;
            int unit_y = units [unit_indices[which_one]]->y;
            if (direction == 1 && unit_x < world.map_size - 1
                && !unitIsAtPosition (unit_x + 1, unit_y)) {
                units [unit_indices[which_one]]->move (world, 1, 0);
            }

            if (direction == 2 && unit_y < world.map_size - 1
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
            which_one = utility::integer_input ();

            units [unit_indices[which_one]]->action (world, units);

            std::vector <Unit*> alive_units;
            for (int i = 0; i < units.size (); i++) {
                if (units [i]->health > 0) {
                    alive_units.push_back (units [i]);
                }
            }
            units.swap (alive_units);
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
        unit_index = utility::integer_input ();

        if (unit_index == units_for_production.size ()) {
            still_producing = false;
        } else if (unit_index >= 0 && unit_index < units_for_production.size () && civilizations [civilization_index].cities.size () >= 1) {
            if (civilizations [civilization_index].production < units_for_production [unit_index]->production_cost) {
                std::cout << "You can't produce that!" << std::endl;
            } else {
                civilizations [civilization_index].production -= units_for_production [unit_index]->production_cost;

                std::vector <int> valid_indices;

                for (int j = 0; j < civilizations [civilization_index].cities.size (); j++) {
                    int city_x = civilizations [civilization_index].cities [j].x, city_y = civilizations [civilization_index].cities [j].y;
                    if (world.tiles [city_x + city_y * world.map_size]->ownerIndex == civilization_index) {
                        valid_indices.push_back (j);
                    }
                }
                std::cout << "Produce unit in which city?" << std::endl;
                for (int j = 0; j < valid_indices.size (); j++) {
                    City city = civilizations [civilization_index].cities [valid_indices [j]];
                    std::cout << "[" << j << "] " << city.name << " (" << city.x << ", " << city.y << ")" << std::endl;
                }

                int input = utility::integer_input ();
                if (input < 0 || input > valid_indices.size () - 1) {
                    input = 0;
                }

                int city_index = 0;
                city_index = valid_indices [input];
                int city_x = civilizations [civilization_index].cities [city_index].x, city_y = civilizations [civilization_index].cities [city_index].y;

                if (city_index >= 0 && city_index < civilizations [civilization_index].cities.size ()
                    && world.tiles [city_x + city_y * world.map_size]->ownerIndex == civilization_index) {
                    Unit* unit = units_for_production [unit_index];
                    std::cout << unit->name << " was produced." << std::endl;
                    unit->owner_index = civilization_index;
                    unit->x = civilizations [civilization_index].cities [city_index].x;
                    unit->y = civilizations [civilization_index].cities [city_index].y;
                    units.push_back (unit);
                }

                units_for_production.clear ();

                units_for_production.push_back (new Scout ());
                units_for_production.push_back (new Warrior ());
                units_for_production.push_back (new Settler ());
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
    std::vector <Civilization> remaining_civilizations;
    std::vector <int> eliminated_civilization_indices;
    for (int i = 0; i < civilizations.size (); i++) {
        if (civilizations [i].cities.size () == 0) {
            eliminated_civilization_indices.push_back (i);
            for (int j = 0; j < world.tiles.size (); j++) {
                if (world.tiles [j]->ownerIndex == i) {
                    world.tiles [j]->ownerIndex = -1;
                }
            }
        } else {
            remaining_civilizations.push_back (civilizations [i]);
        }
    }

    civilizations.swap (remaining_civilizations);
    for (int i = 0; i < eliminated_civilization_indices.size (); i++) {
        cleanupRemainingCivilizations (eliminated_civilization_indices [i] - i);
    }
}

void Game::cleanupRemainingCivilizations (int eliminated_civilization_index) {
    for (int i = 0; i < world.tiles.size (); i++) {
        if (world.tiles [i]->ownerIndex > eliminated_civilization_index) {
            world.tiles [i]->ownerIndex -= 1;
        }
    }

    std::vector <Unit*> remaining_units;
    for (int i = 0; i < units.size (); i++) {
        if (units [i]->owner_index != eliminated_civilization_index) {
            remaining_units.push_back (units [i]);
        }
        if (units [i]->owner_index > eliminated_civilization_index) {
            units [i]->owner_index -= 1;
        }
    }

    units.swap (remaining_units);
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

void Game::load_game (std::string filename) {
    std::string line;
    std::ifstream file (filename);

    if (file.is_open()) {
        loaded = true;
        file >> world.map_size;
        for (int i = 0; i < world.map_size * world.map_size; i++) {
            char render_character;
            Tile* tile;
            file >> render_character;

            if (render_character == ',') {
                tile = new PlainsTile ();
            } else if (render_character == '*') {
                tile = new ForestTile ();
            } else if (render_character == '^') {
                tile = new MountainTile ();
            } else if (render_character == '!') {
                tile = new CityTile ();
            }
            file >> tile->ownerIndex;

            world.tiles.push_back (tile);
        }

        int unit_size = 0;
        file >> unit_size;
        for (int i = 0; i < unit_size; i++) {
            char render_character;
            Unit* unit;
            file >> render_character;

            if (render_character == 'S') {
                unit = new Settler ();
            } else if (render_character == 'W') {
                unit = new Warrior ();
            } else if (render_character == '>') {
                unit = new Scout ();
            }
            file >> unit->owner_index;

            file >> unit->x;
            file >> unit->y;

            file >> unit->health;
            file >> unit->maxHealth;
            file >> unit->combat_strength;
            units.push_back (unit);
        }

        int civilization_size = 0;
        file >> civilization_size;
        for (int i = 0; i < civilization_size; i++) {
            Civilization civilization;

            file >> civilization.name;
            file >> civilization.production;

            int city_size = 0;
            file >> city_size;
            for (unsigned int j = 0; j < city_size; j++) {
                int owner_index = 0;
                file >> owner_index;

                int x = 0, y = 0;
                file >> x;
                file >> y;

                City city (x, y, owner_index, world);
                file >> city.name;

                file >> city.population;
                file >> city.food_stockpile;

                file >> city.total_food_yield;
                file >> city.total_production_yield;

                civilization.cities.push_back (city);
            }

            civilizations.push_back (civilization);
        }

        file.close();

    } else std::cout << "Can't open file.\n";

}

void Game::save_game (std::string filename) {

    std::string line;
    std::ofstream file (filename);

    if (file.is_open()) {
        file << world.map_size << "\n";
        for (int i = 0; i < world.map_size * world.map_size; i++) {
            file << world.tiles [i]->render_character << "\n";
            file << world.tiles [i]->ownerIndex << "\n";
        }

        file << units.size() << "\n";
        for (int i = 0; i < units.size(); i++) {
            file << units [i]->render_icon << "\n";
            file << units [i]->owner_index << "\n";

            file << units [i]->x << "\n";
            file << units [i]->y << "\n";

            file << units [i]->health << "\n";
            file << units [i]->maxHealth << "\n";
            file << units [i]->combat_strength << "\n";
        }

        file << civilizations.size() << "\n";

        for (int i = 0; i < civilizations.size(); i++) {
            Civilization civilization = civilizations [i];

            file << civilization.name << "\n";
            file << civilization.production << "\n";

            file << civilization.cities.size() << "\n";
            for (unsigned int j = 0; j < civilization.cities.size(); j++) {
                City city = civilization.cities [j];

                file << city.owner_index << "\n";

                file << city.x << "\n";
                file << city.y << "\n";

                file << city.name << "\n";

                file << city.population << "\n";
                file << city.food_stockpile << "\n";

                file << city.total_food_yield << "\n";
                file << city.total_production_yield << "\n";

            }

        }

        file.close();

    }
}
