#include "utility.h"
#include <iostream>
#include <climits>
#include <thread>

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

}
