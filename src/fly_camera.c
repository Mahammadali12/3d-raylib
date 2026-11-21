#include "fly_camera.h"

#include <math.h>
#include <raymath.h>

// Stay just short of straight up/down so the view never flips.
#define PITCH_LIMIT (PI / 2.0f - 0.01f)

void FlyCamera_Init(FlyCamera *fc, Vector3 position, float yaw, float pitch)
{
    fc->yaw              = yaw;
    fc->pitch            = pitch;
    fc->moveSpeed        = 15.0f;
    fc->mouseSensitivity = 0.001f;

    fc->camera.position   = position;
    fc->camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
    fc->camera.fovy       = 45.0f;
    fc->camera.projection = CAMERA_PERSPECTIVE;

    // Sets the initial target from yaw/pitch.
    FlyCamera_Update(fc, &(Input){ 0 }, 0.0f);
}

Vector3 FlyCamera_Direction(const FlyCamera *fc)
{
    return (Vector3){
         cosf(fc->pitch) * sinf(fc->yaw),
         sinf(fc->pitch),
        -cosf(fc->pitch) * cosf(fc->yaw)
    };
}

void FlyCamera_Update(FlyCamera *fc, const Input *in, float dt)
{
    // Look
    fc->yaw   += in->look.x * fc->mouseSensitivity;
    fc->pitch -= in->look.y * fc->mouseSensitivity;
    fc->pitch  = Clamp(fc->pitch, -PITCH_LIMIT, PITCH_LIMIT);

    Vector3 look    = FlyCamera_Direction(fc);
    Vector3 forward = Vector3Normalize((Vector3){ look.x, 0.0f, look.z }); // walk on the XZ plane
    Vector3 right   = Vector3Normalize(Vector3CrossProduct(forward, fc->camera.up));

    // Move (scaled by dt so speed doesn't depend on frame rate)
    float step = fc->moveSpeed * dt;
    Vector3 pos = fc->camera.position;
    pos = Vector3Add(pos, Vector3Scale(forward,          in->moveForward * step));
    pos = Vector3Add(pos, Vector3Scale(right,            in->moveRight   * step));
    pos = Vector3Add(pos, Vector3Scale(fc->camera.up,    in->moveUp      * step));

    fc->camera.position = pos;
    fc->camera.target   = Vector3Add(pos, look);
}
