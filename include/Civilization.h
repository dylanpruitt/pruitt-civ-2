#ifndef CIVILIZATION_H
#define CIVILIZATION_H
#include <vector>
#include "City.h"

struct Civilization
{
    public:
        std::string name;

        std::vector <City> cities;

        bool playedByHumans = false;

        int production = 0;

};

#endif // CIVILIZATION_H
