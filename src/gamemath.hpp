#ifndef GAMEMATH_HPP
#define GAMEMATH_HPP

#include "collidesprite.hpp"

// Return angle in degrees between two points
double GetAngle(int x1, int y1, int x2, int y2) {
    return atan2(y2 - y1, x2 - x1) * (180 / M_PI);
}

// Return angle in degrees between two sprites
double GetAngle(CollideSprite *s1, CollideSprite *s2) {
    int x1 = s1->GetX();
    int y1 = s1->GetY();
    int x2 = s2->GetX();
    int y2 = s2->GetY();

    return GetAngle(x1, y1, x2, y2);
}

// Return angle in degrees between sprite and point
double GetAngle(CollideSprite *s, int x2, int y2) {
    int x1 = s->GetX();
    int y1 = s->GetY();

    return GetAngle(x1, y1, x2, y2);
}

// Get distance between two points
double GetDistance(int x1, int y1, int x2, int y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Get distance between two sprites
double GetDistance(CollideSprite *s1, CollideSprite *s2) {
    int x1 = s1->GetX();
    int y1 = s1->GetY();
    int x2 = s2->GetX();
    int y2 = s2->GetY();

    return GetDistance(x1, y1, x2, y2);
}

// Get distance between sprite and point
double GetDistance(CollideSprite *s, int x2, int y2) {
    int x1 = s->GetX();
    int y1 = s->GetY();

    return GetDistance(x1, y1, x2, y2);
}

#endif