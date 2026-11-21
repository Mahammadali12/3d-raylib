#ifndef WORLD_H
#define WORLD_H

#include <raylib.h>

// Everything that exists in the scene. Physics state will live here later.
typedef struct World {
    Vector3 cubePosition;
    float   cubeSize;
} World;

void World_Init(World *w);

#endif
