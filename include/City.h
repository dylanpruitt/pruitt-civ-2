#ifndef CITY_H
#define CITY_H
#include <string>
#include "worldMap.h"

class City
{
    public:
        City(int x, int y, int owner_index, worldMap &world);
        virtual ~City();

        std::string name;

        int x = 0;
        int y = 0;

        int population = 1;
        int food_stockpile = 0;

        int total_food_yield = 0;
        int total_production_yield = 0;

        int owner_index = 0;

        void expand (worldMap &world);

        void update_population (worldMap &world);

        void update_yields (worldMap &world);
    protected:

    private:
        bool tileIsBorderedByCivilization (worldMap world, int civilizationIndex, int x, int y);

        bool canExpand (worldMap world);
};

#endif // CITY_H
