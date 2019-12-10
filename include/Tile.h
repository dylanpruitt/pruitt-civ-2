#ifndef TILE_H
#define TILE_H


class Tile
{
    public:
        Tile() {}
        virtual ~Tile() {}

        char render_character = ',';

        static const int NOT_OWNED_BY_CIVILIZATION = -1;
        int ownerIndex = NOT_OWNED_BY_CIVILIZATION;

        int getFoodYield () { return foodYield; }
        int getProductionYield () { return productionYield; }

        int getMovementCost () { return movementCost; }
    protected:
        int foodYield = 0;
        int productionYield = 0;

        int movementCost = 1;

    private:
};

#endif // TILE_H
