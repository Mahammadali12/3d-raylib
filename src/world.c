#include "world.h"

void World_Init(World *w)
{
    w->cube = (Body){
        .position    = { 0.0f, 5.0f, 0.0f }, // dropped from 4 m above the ground (bottom face at y = 4)
        .velocity    = { 0.0f, 0.0f, 0.0f },
        .halfSize    = 1.0f,
        .restitution = 0.6f,
        .mass        = 1.0f,
    };
}
