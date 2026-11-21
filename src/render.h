#ifndef RENDER_H
#define RENDER_H

#include "fly_camera.h"
#include "world.h"

// Draws one full frame. Read-only access: rendering must never mutate state.
void Render_Frame(const FlyCamera *cam, const World *world);

#endif
