#include "render.h"

void Render_Frame(const FlyCamera *cam, const World *world)
{
    BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(cam->camera);
            DrawCube(world->cubePosition, world->cubeSize, world->cubeSize, world->cubeSize, RED);
            DrawCubeWires(world->cubePosition, world->cubeSize, world->cubeSize, world->cubeSize, MAROON);
            DrawGrid(10, 1.0f);
        EndMode3D();

        DrawFPS(10, 10);
    EndDrawing();
}
