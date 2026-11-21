#include <raylib.h>
#include "app.h"

int main(void)
{
    InitWindow(800, 450, "raylib sandbox");
    SetTargetFPS(60);

    App app;
    App_Init(&app);

    while (!WindowShouldClose()) {
        App_Update(&app, GetFrameTime());
        App_Render(&app);
    }

    CloseWindow();
    return 0;
}
