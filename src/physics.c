#include "physics.h"

#include <math.h>
#include <raymath.h>

#define GROUND_Y 0.0f

static void Body_Step(Body *b, float dt)
{
    // Semi-implicit Euler: velocity first, then position using the NEW velocity.
    b->velocity.y -= PHYSICS_GRAVITY * dt;
    b->position = Vector3Add(b->position, Vector3Scale(b->velocity, dt));

    // Ground plane: test the bottom face, not the center.
    float bottom = b->position.y - b->halfSize;
    if (bottom < GROUND_Y) {
        b->position.y = GROUND_Y + b->halfSize; // push out of the ground

        if (b->velocity.y < 0.0f) {
            b->velocity.y = -b->velocity.y * b->restitution;

            // Rest threshold: without this, gravity re-feeds a micro-bounce forever.
            if (fabsf(b->velocity.y) < PHYSICS_REST_EPS)
                b->velocity.y = 0.0f;
        }
    }
}

void Physics_ApplyImpulse(Body *body, Vector3 force)
{
    body->velocity = Vector3Add(body->velocity,Vector3Scale(force,1/body->mass));
}

void Physics_Step(World *world, float dt)
{
    Body_Step(&world->cube, dt);
}
