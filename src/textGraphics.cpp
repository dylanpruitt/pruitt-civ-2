#include "textGraphics.h"
#include <windows.h>

namespace textGraphics {

    void changeTextColor (int foreground, int background) {

        HANDLE  hConsole;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        int color_code = foreground + background * 16;
        SetConsoleTextAttribute (hConsole, color_code);

    }

}
