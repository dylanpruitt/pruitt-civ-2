#ifndef WARRIOR_H
#define WARRIOR_H
#include "Unit.h"

class Warrior : public Unit
{
    public:
        Warrior();
        virtual ~Warrior();

        void action (worldMap &world, std::vector <Unit*> &units);
    protected:

    private:
};

#endif // WARRIOR_H
