#ifndef DIRECTION_HPP
#define DIRECTION_HPP

enum Direction {
    DOWN,
    UP,
    RIGHT,
    LEFT,
};

int ConvertDirectionToInt(Direction direction) {
    if (direction == DOWN || direction == RIGHT) {
        return 1;
    } else {
        return -1;
    }
}

#endif