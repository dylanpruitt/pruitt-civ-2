#ifndef TEXTGRAPHICS_H
#define TEXTGRAPHICS_H


namespace textGraphics
{
    extern void changeTextColor (int foreground, int background);

    enum colors : int {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        AQUA = 3,
        RED = 4,
        PURPLE = 5,
        YELLOW = 6,
        WHITE = 7,
        GRAY = 8,
        LIGHT_BLUE = 9,
        LIGHT_GREEN = 10,
        LIGHT_AQUA = 11,
        LIGHT_RED = 12,
        LIGHT_PURPLE = 13,
        LIGHT_YELLOW = 14,
        BRIGHT_WHITE = 15
    };
};

#endif // TEXTGRAPHICS_H
