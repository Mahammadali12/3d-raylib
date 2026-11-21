#include "physics.h"
#include <stdio.h>
#include <math.h>
#include <raymath.h>

#define GROUND_Y 0.0f

static void Body_Step(Body *b, float dt)
{
    Physics_ApplyGravity(b,dt);
    Physics_ApplyFriction(b,dt);

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
    b->position = Vector3Add(b->position, Vector3Scale(b->velocity, dt));
}

void Physics_ApplyImpulse(Body *body, Vector3 impulse)
{
    if(body->mass > 0.0f) //Safeguard for division by zero
    {
        float inverseMass = 1/body->mass;
        body->velocity = Vector3Add(body->velocity,Vector3Scale(impulse,1/body->mass));
    }
}

void Physics_ApplyGravity(Body *body, float dt)
{
    // Semi-implicit Euler: velocity first, then position using the NEW velocity.
    body->velocity.y -= PHYSICS_GRAVITY * dt;
    // body->position = Vector3Add(body->position, Vector3Scale(body->velocity, dt));
}

void Physics_ApplyFriction(Body *body, float dt)
{

    // Check if on ground and kinetic  friction should be applied
    // After applying gravity and drag to acc
    // Vector2 predicted_vel = Vector2Add(body->velocity, Vector2Scale(c->acc, dt));
    Vector3 predicted_pos = Vector3Add(body->position, Vector3Scale(body->velocity, dt));

    if (predicted_pos.y - body->halfSize >= MINIMUM_INTERACTION_HEIGHT)
    {
        //On the air: NO FRICTION FORCE
        // printf("POS y: %f | PRED: y %f\n",body->position.y,predicted_pos.y);
    }
    else
    {

        float frictionMagnitude = FRICTION_COEFFICIENT * body->mass * PHYSICS_GRAVITY;   //mu * m * g

        float vx = body->velocity.x;
        float speed = fabsf(vx);
        if (speed < EPSILON)
        {
            printf("No friction applied, speed too low\n");
            return;
        } 

        if (fabsf(speed - 5.0f) < 0.001f) {
            float stopTime = speed / (FRICTION_COEFFICIENT * PHYSICS_GRAVITY);
            float slideDistance = speed * speed /
                                  (2.0f * FRICTION_COEFFICIENT * PHYSICS_GRAVITY);
            printf("FRICTION VERIFY: vx=%.2f mu=%.2f stop=%.2f s distance=%.2f m\n",
                   speed, FRICTION_COEFFICIENT, stopTime, slideDistance);
        }

        Vector3 friction = { -vx / speed * frictionMagnitude , 0.0f , 0.0f}; // in Newtons
        // friction is added as acceleration (F/m) 


        friction.x = Clamp(friction.x, -body->velocity.x,0.0f);
        body->velocity.x += friction.x * dt;
        printf("FRICTION: x: %f \n",friction.x);

    }
}

void Physics_Step(World *world, float dt)
{
    Body_Step(&world->cube, dt);
}
