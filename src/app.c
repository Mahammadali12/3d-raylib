#include "app.h"

#include <stdio.h>
#include <raymath.h>
#include "physics.h"
#include "render.h"

// A long frame (e.g. dragging the window) must not trigger hundreds of
// physics steps in one go ("spiral of death").
#define MAX_FRAME_TIME 0.25f

void App_Init(App *app)
{
    World_Init(&app->world);

    // Start above and behind the cube, looking at the origin (45 degrees down).
    FlyCamera_Init(&app->cam, (Vector3){ 0.0f, 10.0f, 10.0f }, 0.0f, -PI / 4.0f);

    app->physicsAccumulator = 0.0f;
    app->debug = false;
    DisableCursor();
}

void App_Update(App *app, float dt)
{
    Input_Poll(&app->input);

    if (IsKeyPressed(KEY_F3)) app->debug = !app->debug;

    // R: reset the simulation so you can rerun the drop.
    if (IsKeyPressed(KEY_R)) {
        World_Init(&app->world);
        app->physicsAccumulator = 0.0f;
    }

    FlyCamera_Update(&app->cam, &app->input, dt);

    // Fixed-timestep physics: consume real time in constant-size steps.
    app->physicsAccumulator += Clamp(dt, 0.0f, MAX_FRAME_TIME);
    while (app->physicsAccumulator >= PHYSICS_STEP) {

        if(app->input.kick)
        {
            Vector3 force = {0,5,0};
            Physics_ApplyImpulse(&app->world.cube,force);
        }


        Physics_Step(&app->world, PHYSICS_STEP);
        app->physicsAccumulator -= PHYSICS_STEP;
    }

    if (app->debug) {
        Vector3 d = FlyCamera_Direction(&app->cam);
        printf("DIRECTION: x: %f | y: %f | z: %f | CUBE: y: %f vy: %f\n",
               d.x, d.y, d.z, app->world.cube.position.y, app->world.cube.velocity.y);
    }
}

void App_Render(const App *app)
{
    Render_Frame(&app->cam, &app->world);
}
