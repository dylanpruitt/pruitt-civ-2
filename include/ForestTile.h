#ifndef FORESTTILE_H
#define FORESTTILE_H


class ForestTile : public Tile
{
    public:
        ForestTile () {
            foodYield = 0;
            productionYield = 1;
            movementCost = 2;
            render_character = '*';
        }
        virtual ~ForestTile() {};

};

#endif // FORESTTILE_H
