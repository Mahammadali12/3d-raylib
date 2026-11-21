#ifndef FLY_CAMERA_H
#define FLY_CAMERA_H

#include <raylib.h>
#include "input.h"

typedef struct FlyCamera {
    Camera3D camera;
    float    yaw;              // radians
    float    pitch;            // radians, clamped in Update
    float    moveSpeed;        // units per second
    float    mouseSensitivity; // radians per pixel
} FlyCamera;

void    FlyCamera_Init(FlyCamera *fc, Vector3 position, float yaw, float pitch);
void    FlyCamera_Update(FlyCamera *fc, const Input *in, float dt);
Vector3 FlyCamera_Direction(const FlyCamera *fc);

#endif
