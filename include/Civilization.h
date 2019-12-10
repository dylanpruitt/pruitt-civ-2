#ifndef CIVILIZATION_H
#define CIVILIZATION_H
#include <vector>
#include "City.h"

struct Civilization
{
    public:
        std::string name;

        std::vector <City> cities;

        int WorldExplorationMap[50][200];

        bool playedByHumans = false;

        int colorCode;

        int production = 0;

};

#endif // CIVILIZATION_H
