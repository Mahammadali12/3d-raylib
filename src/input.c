#include "input.h"

void Input_Poll(Input *in)
{
    in->look = GetMouseDelta();

    in->moveForward = (float)(IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
    in->moveRight   = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    in->moveUp      = (float)(IsKeyDown(KEY_SPACE) - IsKeyDown(KEY_LEFT_CONTROL));

    in->kickUp        = (float)(IsKeyPressed(KEY_F));

    in->pushX = (float)(IsKeyPressed(KEY_K) - IsKeyPressed(KEY_I));
    in->pushZ = (float)(IsKeyPressed(KEY_L) - IsKeyPressed(KEY_J));
}
