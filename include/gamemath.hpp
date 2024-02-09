#ifndef GAMEMATH_HPP
#define GAMEMATH_HPP

#include <collidesprite.hpp>

// Return angle in degrees between two points
double GDK_GetAngle(int x1, int y1, int x2, int y2);

// Return angle in degrees between two sprites
double GDK_GetAngle(CollideSprite *s1, CollideSprite *s2);

// Return angle in degrees between sprite and point
double GDK_GetAngle(CollideSprite *s, int x2, int y2);

// Get distance between two points
double GDK_GetDistance(int x1, int y1, int x2, int y2);

// Get distance between two sprites
double GDK_GetDistance(CollideSprite *s1, CollideSprite *s2);

// Get distance between sprite and point
double GDK_GetDistance(CollideSprite *s, int x2, int y2);

// Set seed for use with GDK_GetRandomInt()
void GDK_SetRandomSeed(uint seed);

// Returns random integer in range
int GDK_GetRandomInt(int range_start, int range_end);

#endif