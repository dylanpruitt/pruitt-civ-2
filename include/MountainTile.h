#ifndef MOUNTAINTILE_H
#define MOUNTAINTILE_H


class MountainTile : public Tile
{
    public:
        MountainTile () {
            foodYield = 0;
            productionYield = 0;
            movementCost = 10;
            render_character = '^';
        }
        virtual ~MountainTile() {}

    protected:

    private:
};

#endif // MOUNTAINTILE_H
