#include <direction.hpp>

int ConvertDirectionToInt(GDK_Direction direction) {
    if (direction == DOWN || direction == RIGHT) {
        return 1;
    } else {
        return -1;
    }
}
