#include "app.h"

#include <stdio.h>
#include <raymath.h>
#include "render.h"

void App_Init(App *app)
{
    World_Init(&app->world);

    // Start above and behind the cube, looking at the origin (45 degrees down).
    FlyCamera_Init(&app->cam, (Vector3){ 0.0f, 10.0f, 10.0f }, 0.0f, -PI / 4.0f);

    app->debug = false;
    DisableCursor();
}

void App_Update(App *app, float dt)
{
    Input_Poll(&app->input);

    if (IsKeyPressed(KEY_F3)) app->debug = !app->debug;

    FlyCamera_Update(&app->cam, &app->input, dt);

    if (app->debug) {
        Vector3 d = FlyCamera_Direction(&app->cam);
        printf("DIRECTION: x: %f | y: %f | z: %f\n", d.x, d.y, d.z);
    }
}

void App_Render(const App *app)
{
    Render_Frame(&app->cam, &app->world);
}
