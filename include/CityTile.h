#ifndef CITYTILE_H
#define CITYTILE_H


class CityTile : public Tile
{
    public:
        CityTile () {
            foodYield = 1;
            productionYield = 1;
            movementCost = 1;
            render_character = '!';
        }
        virtual ~CityTile() {};
};

#endif // CITYTILE_H
