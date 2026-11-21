#include "render.h"

void Render_Frame(const FlyCamera *cam, const World *world)
{
    const Body *c = &world->cube;
    float size = c->halfSize * 2.0f;

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
