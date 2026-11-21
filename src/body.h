#ifndef BODY_H
#define BODY_H

#include <raylib.h>

// A physical cube. Defined here and nowhere else.
typedef struct Body {
    Vector3 position;    // center of the cube, meters
    Vector3 velocity;    // meters per second
    float   halfSize;    // half the edge length, meters
    float   restitution; // 0 = no bounce, 1 = perfectly elastic
    float   mass;
    Vector3 force;
} Body;

#endif
