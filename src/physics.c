#include "physics.h"
#include <stdio.h>
#include <math.h>
#include <raymath.h>

#define GROUND_Y 0.0f

static void Body_Step(Body *b, float dt)
{
    Physics_ApplyGravity(b,dt);
    b->velocity = Vector3Add(b->velocity,Vector3Scale(b->force,dt/b->mass));

    float bottom = b->position.y - b->halfSize;
    b->grounded = bottom <= GROUND_Y;
    Physics_ApplyFriction(b,dt);
    Physics_ApplyDrag(b,dt);

    //Integration
    b->position = Vector3Add(b->position, Vector3Scale(b->velocity, dt));

    // Ground plane: test the bottom face, not the center.
    bottom = b->position.y - b->halfSize;
    if (bottom < GROUND_Y) {
        b->position.y = GROUND_Y + b->halfSize; // push out of the ground
        b->grounded = true;

        if (b->velocity.y < 0.0f) {
            b->velocity.y = -b->velocity.y * b->restitution;

            // Rest threshold: without this, gravity re-feeds a micro-bounce forever.
            if (fabsf(b->velocity.y) < PHYSICS_REST_EPS)
                b->velocity.y = 0.0f;
        }
    } else {
        b->grounded = false;
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
}

void Physics_ApplyFriction(Body *body, float dt)
{
    if (body->grounded)
    {

        float frictionMagnitude = FRICTION_COEFFICIENT * body->mass * PHYSICS_GRAVITY;   //mu * m * g

        float vx = body->velocity.x;
        float vz = body->velocity.z;
        float speed = sqrt(vx*vx + vz*vz);
        if (speed < PHYSICS_EPSILON)
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

void Physics_ApplyDrag(Body *body, float dt)
{
    float speed = Vector3Length(body->velocity);
    if (speed < EPSILON) return; // nothing to do

    // Projected area of cube
    float area = body->halfSize*body->halfSize*2;
    float rho = 1.225f;

    float dragMag = 0.5f * rho * DRAG_COEFFICIENT * area * speed * speed; // Newtons

    // Direction opposite velocity
    Vector3 dir = Vector3Scale(Vector3Normalize(body->velocity), -1.0f);
    Vector3 dragForce = Vector3Scale(dir, dragMag);

    body->velocity = Vector3Add(body->velocity,Vector3Scale(dragForce,dt/body->mass));

}

void Physics_Step(World *world, float dt)
{
    Body_Step(&world->cube, dt);
}
