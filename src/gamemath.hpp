#ifndef GAMEMATH_HPP
#define GAMEMATH_HPP

#include "collidesprite.hpp"

// Return angle in degrees between two points
double GDK_GetAngle(int x1, int y1, int x2, int y2) {
    return atan2(y2 - y1, x2 - x1) * (180 / M_PI);
}

// Return angle in degrees between two sprites
double GDK_GetAngle(CollideSprite *s1, CollideSprite *s2) {
    int x1 = s1->GetX();
    int y1 = s1->GetY();
    int x2 = s2->GetX();
    int y2 = s2->GetY();

    return GDK_GetAngle(x1, y1, x2, y2);
}

// Return angle in degrees between sprite and point
double GDK_GetAngle(CollideSprite *s, int x2, int y2) {
    int x1 = s->GetX();
    int y1 = s->GetY();

    return GDK_GetAngle(x1, y1, x2, y2);
}

// Get distance between two points
double GDK_GetDistance(int x1, int y1, int x2, int y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Get distance between two sprites
double GDK_GetDistance(CollideSprite *s1, CollideSprite *s2) {
    int x1 = s1->GetX();
    int y1 = s1->GetY();
    int x2 = s2->GetX();
    int y2 = s2->GetY();

    return GDK_GetDistance(x1, y1, x2, y2);
}

// Get distance between sprite and point
double GDK_GetDistance(CollideSprite *s, int x2, int y2) {
    int x1 = s->GetX();
    int y1 = s->GetY();

    return GDK_GetDistance(x1, y1, x2, y2);
}

// Set seed for use with GDK_GetRandomInt()
void GDK_SetRandomSeed(uint seed) {
    srand(seed);
}

// Returns random integer in range
int GDK_GetRandomInt(int range_start, int range_end) {
    if (range_start > range_end) {
        int tmp = range_start;
        range_start = range_end;
        range_end = tmp;
    }

    return range_start + rand() % (range_end - range_start + 1);
}

#endif