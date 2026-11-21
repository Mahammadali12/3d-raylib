#include "input.h"

void Input_Poll(Input *in)
{
    in->look = GetMouseDelta();

    in->moveForward = (float)(IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
    in->moveRight   = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    in->moveUp      = (float)(IsKeyDown(KEY_SPACE) - IsKeyDown(KEY_LEFT_CONTROL));
}
