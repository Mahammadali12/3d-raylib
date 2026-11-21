#ifndef INPUT_H
#define INPUT_H

#include <raylib.h>

// Device-agnostic snapshot of what the player wants this frame.
// Gameplay code reads this instead of calling IsKeyDown() directly.
typedef struct Input {
    Vector2 look;        // mouse delta in pixels
    float   moveForward; // -1..1
    float   moveRight;   // -1..1
    float   moveUp;      // -1..1
} Input;

void Input_Poll(Input *in);

#endif
