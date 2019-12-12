#include "utility.h"
#include <iostream>
#include <climits>
#include <thread>
#include <math.h>

namespace utility {

    int integer_input () {
        int input = 0;

        std::cin >> input;
        while (!std::cin) {
            std::cout << "Bad value!";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> input;
        }

        return input;
    }

    int get_distance (int x1, int y1, int x2, int y2) {
        return sqrt ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }
}
