#ifndef WORLD_H
#define WORLD_H

#include "body.h"

#define MAX 1000
// Everything that exists in the scene.
typedef struct World {
    Body bodies[MAX];
    int count;
} World;

// Also used as "reset": restores the initial state.
void World_Init(World *w);

#endif
