#include "render.h"

void Render_Frame(const FlyCamera *cam, const World *world)
{
    const Body *c = &world->bodies[0];
    float size = c->halfExtents.x * 2.0f;

    BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(cam->camera);
            DrawCube(c->position, size, size, size, RED);
            DrawCubeWires(c->position, size, size, size, MAROON);
            DrawGrid(5, 1.0f);
        EndMode3D();

        DrawFPS(10, 10);
    EndDrawing();
}
