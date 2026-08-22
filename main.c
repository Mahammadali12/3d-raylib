
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

#define SENSITIVITY 0.01


// gcc main.c -g -o bin/main -Wall -Wextra -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
void logCamera();
void controlCamera();

Camera3D camera;
Vector3 cubePosition;

double yaw =   0;
double pitch = 0;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

    // Define the camera to look into our 3d world
    float x=0.0f;
    float y=0.0f;
    float z=0.0f;

    camera.position = (Vector3){ 0.0f,10.0f,10.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    cubePosition = (Vector3){ x,y,z };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    DisableCursor();
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        controlCamera();
        logCamera();


        

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

              
                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

                DrawGrid(10, 1.0f);

            EndMode3D();


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void logCamera()
{
    Vector3 direction = Vector3Normalize(Vector3Subtract(camera.target,camera.position));
    Vector2 mousePosition = GetMousePosition();

    // printf("POSITION: x: %f | y: %f | z: %f\n",camera.position.x,camera.position.y,camera.position.z);
    // printf("MOUSE_POSITION: x: %f | y: %f\n",mousePosition.x,mousePosition.y);
    printf("DIRECTION: x: %f | y: %f | z: %f\n",direction.x,direction.y,direction.z);
}

void controlCamera()
{
    Vector2 deltaMouse = GetMouseDelta();
    
    yaw +=   deltaMouse.x * SENSITIVITY;
    pitch -= deltaMouse.y * SENSITIVITY;

    float x,y,z;
    x = cos(pitch) * sin(yaw);
    y = sin(pitch);
    z = -cos(pitch) * cos(yaw);

    Vector3 lookDirection = (Vector3){x,y,z};


    Vector3 forwardDirection = Vector3Normalize((Vector3){lookDirection.x,0,lookDirection.z});
    Vector3 rightDirection = Vector3Normalize(Vector3CrossProduct(forwardDirection,camera.up));

    printf("DIRECTION: x: %f | y: %f | z: %f\n",forwardDirection.x,forwardDirection.y,forwardDirection.z);

    if(IsKeyDown(KEY_W))
        camera.position = Vector3Add(Vector3Scale(forwardDirection,0.5f),camera.position);
        
    if(IsKeyDown(KEY_S))
        camera.position = Vector3Add(Vector3Scale(forwardDirection,-0.5f),camera.position);

    if(IsKeyDown(KEY_A))
    {
        printf("RIGHT_DIRECTION: x: %f | y: %f | z: %f\n",rightDirection.x,rightDirection.y,rightDirection.z);
        camera.position = Vector3Add(Vector3Scale(rightDirection,-0.5f),camera.position);
    }
    if(IsKeyDown(KEY_D))
    {
        printf("RIGHT_DIRECTION: x: %f | y: %f | z: %f\n",rightDirection.x,rightDirection.y,rightDirection.z);
        camera.position = Vector3Add(Vector3Scale(rightDirection,0.5f),camera.position);
    }

    camera.target = Vector3Add(camera.position,lookDirection);


}
