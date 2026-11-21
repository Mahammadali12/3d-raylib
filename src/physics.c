#include "physics.h"
#include <stdio.h>
#include <math.h>
#include <raymath.h>

#define GROUND_Y 0.0f

static void Body_Step(Body *b, float dt)
{
    Physics_ApplyGravity(b,dt);
    b->velocity = Vector3Add(b->velocity,Vector3Scale(b->force,dt/b->mass));
    Physics_ApplyFriction(b,dt);


    b->position = Vector3Add(b->position, Vector3Scale(b->velocity, dt));

    b->grounded = false;
    // Ground plane: test the bottom face, not the center.
    float bottom = b->position.y - b->halfSize;
    if (bottom < GROUND_Y) {
        b->position.y = GROUND_Y + b->halfSize; // push out of the ground

        if (b->velocity.y < 0.0f) {
            b->velocity.y = -b->velocity.y * b->restitution;

            // Rest threshold: without this, gravity re-feeds a micro-bounce forever.
            if (fabsf(b->velocity.y) < PHYSICS_REST_EPS)
                b->velocity.y = 0.0f; b->grounded = true;
        }
    }
}

void Physics_ApplyImpulse(Body *body, Vector3 impulse)
{
    // if(body->mass > 0.0f) //Safeguard for division by zero
    // {
    //     float inverseMass = 1/body->mass;
    //     body->velocity = Vector3Add(body->velocity,Vector3Scale(body->force,inverseMass));
    // }
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

    if (body->grounded)
    {
        //On the air: NO FRICTION FORCE
        // printf("POS y: %f | PRED: y %f\n",body->position.y,predicted_pos.y);
    }
    else
    {

        float frictionMagnitude = FRICTION_COEFFICIENT * body->mass * PHYSICS_GRAVITY;   //mu * m * g

        float vx = body->velocity.x;
        float vz = body->velocity.z;
        float speed = sqrt(vx*vx + vz*vz);
        if (speed < EPSILON)
        {
            return;
        } 

        Vector3 direction = Vector3Scale((Vector3){vx,0.0f,vz},1/speed);
        Vector3 inverseDirection = Vector3Scale(direction,-1.0f);

        Vector3 friction = Vector3Scale(inverseDirection,frictionMagnitude); // in Newtons
        // friction is added as acceleration (F/m) 

        
        if (FRICTION_COEFFICIENT * PHYSICS_GRAVITY * dt > speed)
        {
            body->velocity.x = 0;
            body->velocity.z = 0;
        }else 
        {
            body->velocity = Vector3Add(body->velocity,Vector3Scale(friction,dt/body->mass));
            // body->velocity.x += friction.x * dt / body->mass;
            // body->velocity.z += friction.z * dt / body->mass;
        }


    }
}

void Physics_Step(World *world, float dt)
{
    Body_Step(&world->cube, dt);
}
