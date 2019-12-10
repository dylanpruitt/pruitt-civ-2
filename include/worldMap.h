#ifndef WORLDMAP_H
#define WORLDMAP_H
#include <vector>
#include "Tile.h"

class worldMap
{
    public:
        worldMap ();
        worldMap (int map_size) {
            this->generate (map_size);
            this->map_size = map_size;
        };
        virtual ~worldMap ();

        std::vector <Tile*> tiles;

        void render ();

        int map_size = 0;

    private:
        void generate (int map_size);
};

#endif // WORLDMAP_H
