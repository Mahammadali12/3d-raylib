#include "world.h"

void World_Init(World *w)
{
    w->cubePosition = (Vector3){ 0.0f, 0.0f, 0.0f };
    w->cubeSize     = 2.0f;
}
