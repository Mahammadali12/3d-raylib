#include "world.h"

void World_Init(World *w)
{
    w->bodies[0] = (Body){
        .position    = { 0.0f, 1.0f, 0.0f }, 
        .velocity    = { 0.0f, 0.0f, 0.0f },
        .halfExtents = {1.0f,1.0f,1.0f},
        .restitution = 0.6f,
        .mass        = 1.0f,
    };
}
