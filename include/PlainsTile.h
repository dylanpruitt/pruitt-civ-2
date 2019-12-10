#ifndef PLAINSTILE_H
#define PLAINSTILE_H


class PlainsTile : public Tile
{
    public:
        PlainsTile () {
            foodYield = 1;
            productionYield = 0;
            movementCost = 1;
        }

        virtual ~PlainsTile() {}
};

#endif // PLAINSTILE_H
