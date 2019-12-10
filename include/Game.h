#ifndef GAME_H
#define GAME_H
#include "Civilization.h"
#include "Unit.h"
#include "worldMap.h"

class Game
{
    public:
        Game();
        virtual ~Game();

        void loop ();
    protected:

    private:
        int map_size = 25;
        int number_of_civilizations = 2;

        std::vector <Civilization> civilizations;
        std::vector <Unit*> units;

        worldMap world;

        void interactWithUnits (int civilization_index);

        void displayCivilizationInformation ();

        int getPercentageOfWorldControlled (int civilization_index);

        void foundNewCities ();
};

#endif // GAME_H
