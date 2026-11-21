#ifndef WORLD_H
#define WORLD_H

#include "body.h"

// Everything that exists in the scene.
typedef struct World {
    Body cube;
} World;

// Also used as "reset": restores the initial state.
void World_Init(World *w);

#endif
