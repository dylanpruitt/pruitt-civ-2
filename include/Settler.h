#ifndef SETTLER_H
#define SETTLER_H
#include "Unit.h"

class Settler : public Unit
{
    public:
        Settler();
        virtual ~Settler();

        void action (worldMap &world, std::vector <Unit*> &units);
    protected:

    private:
        int get_distance_of_closest_city (worldMap &world);
};

#endif // SETTLER_H
