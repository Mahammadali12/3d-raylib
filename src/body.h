#ifndef BODY_H
#define BODY_H

#include <raylib.h>

// A physical cube. Defined here and nowhere else.
typedef struct Body {
    Vector3 position;    // center of the cube, meters
    Vector3 velocity;    // meters per second
    Vector3 halfExtents;  // Distnace from center to the each axis
    float   restitution; // 0 = no bounce, 1 = perfectly elastic
    float   mass;
    bool    grounded;
    Vector3 force;
    int     invMass; // 0 means immoveable (static) object
} Body;

#endif
