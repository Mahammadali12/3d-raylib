#ifndef PHYSICS_H
#define PHYSICS_H

#include "world.h"

// Units: 1 unit = 1 meter, +Y is up.
#define PHYSICS_STEP     (1.0f / 120.0f) // fixed simulation step, seconds
#define PHYSICS_GRAVITY  9.81f           // m/s^2, applied along -Y
#define PHYSICS_REST_EPS 0.2f            // bounce speeds below this (m/s) become 0
                                         // (must stay above GRAVITY * STEP ~= 0.08)

// Advances the simulation by exactly dt seconds.
// Pure function of (world, dt): no input, no clocks, no drawing.
void Physics_Step(World *world, float dt);

void Physics_ApplyImpulse(Body *body, Vector3 force);

#endif
