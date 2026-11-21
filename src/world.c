#include "world.h"

void World_Init(World *w)
{
    w->cube = (Body){
        .position    = { 0.0f, 1.0f, 0.0f }, 
        .velocity    = { 0.0f, 0.0f, 0.0f },
        .halfSize    = 1.0f,
        .restitution = 0.6f,
        .mass        = 1.0f,
    };
}
