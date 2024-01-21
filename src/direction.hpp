#ifndef DIRECTION_HPP
#define DIRECTION_HPP

enum GDK_Direction {
    DOWN,
    UP,
    RIGHT,
    LEFT,
};

int ConvertDirectionToInt(GDK_Direction direction) {
    if (direction == DOWN || direction == RIGHT) {
        return 1;
    } else {
        return -1;
    }
}

#endif