#include <math.h>
#include <stdio.h>

#include "physics.h"

#define TEST_TOLERANCE 0.05f
#define SETTLE_STEPS 240
#define SLIDE_MAX_STEPS 600

#define ANSI_RESET  "\033[0m"
#define ANSI_GREEN  "\033[32m"
#define ANSI_RED    "\033[31m"

static int failures;

static void settle(World *world)
{
    World_Init(world);
    world->cube.force = (Vector3){ 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < SETTLE_STEPS; ++i)
        Physics_Step(world, PHYSICS_STEP);
}

static void report(const char *name, int passed)
{
    printf("%s: %s%s%s\n",
           name,
           passed ? ANSI_GREEN : ANSI_RED,
           passed ? "PASS" : "FAIL",
           ANSI_RESET);
    if (!passed)
        ++failures;
}

static int test_four_newtons_does_not_move(void)
{
    World world;
    settle(&world);

    Vector3 start = world.cube.position;
    world.cube.force = (Vector3){ 4.0f, 0.0f, 0.0f };

    for (int i = 0; i < SLIDE_MAX_STEPS; ++i)
        Physics_Step(&world, PHYSICS_STEP);

    float dx = fabsf(world.cube.position.x - start.x);
    float dz = fabsf(world.cube.position.z - start.z);
    printf("  4 N displacement: dx=%.4f m dz=%.4f m\n", dx, dz);

    return dx < TEST_TOLERANCE && dz < TEST_TOLERANCE &&
           fabsf(world.cube.velocity.x) < TEST_TOLERANCE &&
           fabsf(world.cube.velocity.z) < TEST_TOLERANCE;
}

static int run_axial_slide(float initial_x, float expected_stop_time,
                           float expected_distance)
{
    World world;
    settle(&world);

    Vector3 start = world.cube.position;
    world.cube.velocity = (Vector3){ initial_x, 0.0f, 0.0f };
    world.cube.force = (Vector3){ 0.0f, 0.0f, 0.0f };

    int stopped_step = 0;
    for (int i = 1; i <= SLIDE_MAX_STEPS; ++i) {
        Physics_Step(&world, PHYSICS_STEP);
        if (fabsf(world.cube.velocity.x) < PHYSICS_EPSILON &&
            fabsf(world.cube.velocity.z) < PHYSICS_EPSILON) {
            stopped_step = i;
            break;
        }
    }

    float stop_time = stopped_step * PHYSICS_STEP;
    float distance = fabsf(world.cube.position.x - start.x);
    printf("  vx=%+.1f: stop=%.4f s (expected %.3f s), "
           "distance=%.4f m (expected %.3f m)\n",
           initial_x, stop_time, expected_stop_time, distance, expected_distance);

    return stopped_step > 0 &&
           fabsf(stop_time - expected_stop_time) < TEST_TOLERANCE &&
           fabsf(distance - expected_distance) < TEST_TOLERANCE;
}

static int test_positive_slide(void)
{
    float frictionAcceleration = FRICTION_COEFFICIENT * PHYSICS_GRAVITY;
    float dragConstant = 0.5f * 1.225f * DRAG_COEFFICIENT *
                         (2.0f * 1.0f * 1.0f);
    float expectedTime = atanf(
        5.0f * sqrtf(dragConstant / frictionAcceleration)) /
        sqrtf(frictionAcceleration * dragConstant);
    float expectedDistance = (1.0f / (2.0f * dragConstant)) *
        logf((dragConstant * 25.0f + frictionAcceleration) /
             frictionAcceleration);

    return run_axial_slide(5.0f, expectedTime, expectedDistance);
}

static int test_negative_slide(void)
{
    float frictionAcceleration = FRICTION_COEFFICIENT * PHYSICS_GRAVITY;
    float dragConstant = 0.5f * 1.225f * DRAG_COEFFICIENT *
                         (2.0f * 1.0f * 1.0f);
    float expectedTime = atanf(
        5.0f * sqrtf(dragConstant / frictionAcceleration)) /
        sqrtf(frictionAcceleration * dragConstant);
    float expectedDistance = (1.0f / (2.0f * dragConstant)) *
        logf((dragConstant * 25.0f + frictionAcceleration) /
             frictionAcceleration);

    return run_axial_slide(-5.0f, expectedTime, expectedDistance);
}

static int test_diagonal_slide(void)
{
    World world;
    settle(&world);

    Vector3 start = world.cube.position;
    world.cube.velocity = (Vector3){ 3.0f, 0.0f, 4.0f };
    world.cube.force = (Vector3){ 0.0f, 0.0f, 0.0f };

    float max_line_error = 0.0f;
    int stopped_step = 0;

    for (int i = 1; i <= SLIDE_MAX_STEPS; ++i) {
        Physics_Step(&world, PHYSICS_STEP);

        float dx = world.cube.position.x - start.x;
        float dz = world.cube.position.z - start.z;
        float line_error = fabsf(4.0f * dx - 3.0f * dz);
        if (line_error > max_line_error)
            max_line_error = line_error;

        if (fabsf(world.cube.velocity.x) < PHYSICS_EPSILON &&
            fabsf(world.cube.velocity.z) < PHYSICS_EPSILON) {
            stopped_step = i;
            break;
        }
    }

    float distance = sqrtf(
        (world.cube.position.x - start.x) * (world.cube.position.x - start.x) +
        (world.cube.position.z - start.z) * (world.cube.position.z - start.z));

    printf("  diagonal: stopped=%s, distance=%.4f m, max line error=%.4f\n",
           stopped_step > 0 ? "yes" : "no", distance, max_line_error);

    return stopped_step > 0 &&
           distance > 1.25f &&
           distance < 1.36f &&
           max_line_error < TEST_TOLERANCE;
}

static int test_drag_zero_velocity(void)
{
    Body body = {
        .velocity = { 0.0f, 0.0f, 0.0f },
        .halfSize = 1.0f,
        .mass = 1.0f,
    };

    Physics_ApplyDrag(&body, 0.1f);

    printf("  zero velocity: vx=%.4f vy=%.4f vz=%.4f\n",
           body.velocity.x, body.velocity.y, body.velocity.z);

    return fabsf(body.velocity.x) < TEST_TOLERANCE &&
           fabsf(body.velocity.y) < TEST_TOLERANCE &&
           fabsf(body.velocity.z) < TEST_TOLERANCE;
}

static int test_drag_x_formula(void)
{
    Body body = {
        .velocity = { 10.0f, 0.0f, 0.0f },
        .halfSize = 1.0f,
        .mass = 1.0f,
    };
    float dt = 0.1f;

    float speed = 10.0f;
    float area = 2.0f * body.halfSize * body.halfSize;
    float dragMagnitude = 0.5f * 1.225f * DRAG_COEFFICIENT * area * speed * speed;
    float expectedVx = speed - dragMagnitude * dt / body.mass;

    Physics_ApplyDrag(&body, dt);

    printf("  drag x: vx=%.4f m/s, expected=%.4f m/s, drag=%.4f N\n",
           body.velocity.x, expectedVx, dragMagnitude);

    return fabsf(body.velocity.x - expectedVx) < TEST_TOLERANCE &&
           fabsf(body.velocity.y) < TEST_TOLERANCE &&
           fabsf(body.velocity.z) < TEST_TOLERANCE &&
           body.velocity.x < speed;
}

static int test_drag_diagonal_direction(void)
{
    Body body = {
        .velocity = { 3.0f, 0.0f, 4.0f },
        .halfSize = 1.0f,
        .mass = 1.0f,
    };
    Vector3 initialVelocity = body.velocity;

    Physics_ApplyDrag(&body, 0.1f);

    Vector3 velocityChange = {
        body.velocity.x - initialVelocity.x,
        body.velocity.y - initialVelocity.y,
        body.velocity.z - initialVelocity.z,
    };

    float dot = initialVelocity.x * velocityChange.x +
                initialVelocity.z * velocityChange.z;
    float cross = initialVelocity.x * velocityChange.z -
                  initialVelocity.z * velocityChange.x;

    printf("  drag diagonal: dot=%.4f, cross=%.4f\n", dot, cross);

    return dot < 0.0f &&
           fabsf(cross) < TEST_TOLERANCE &&
           fabsf(velocityChange.y) < TEST_TOLERANCE &&
           body.velocity.x < initialVelocity.x &&
           body.velocity.z < initialVelocity.z;
}

int main(void)
{
    report("4 N force", test_four_newtons_does_not_move());
    report("+5 m/s slide", test_positive_slide());
    report("-5 m/s slide", test_negative_slide());
    report("Diagonal slide", test_diagonal_slide());
    report("Drag zero velocity", test_drag_zero_velocity());
    report("Drag x formula", test_drag_x_formula());
    report("Drag diagonal direction", test_drag_diagonal_direction());

    if (failures > 0) {
        printf(ANSI_RED "FAIL" ANSI_RESET ": %d physics check(s) failed\n", failures);
        return 1;
    }

    printf(ANSI_GREEN "PASS" ANSI_RESET ": all physics checks passed\n");
    return 0;
}
