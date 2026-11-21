#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include "fly_camera.h"
#include "input.h"
#include "world.h"

// Owns all game state. No globals anywhere else.
typedef struct App {
    Input     input;
    FlyCamera cam;
    World     world;
    float     physicsAccumulator; // leftover frame time not yet simulated, seconds
    bool      debug;              // toggled with F3
} App;

void App_Init(App *app);
void App_Update(App *app, float dt);
void App_Render(const App *app);

#endif
