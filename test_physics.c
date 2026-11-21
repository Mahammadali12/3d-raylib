#include <math.h>
#include <stdio.h>

#include "physics.h"

#define TEST_TOLERANCE 0.05f
#define SETTLE_STEPS 240
#define SLIDE_MAX_STEPS 600

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
    printf("%s: %s\n", name, passed ? "PASS" : "FAIL");
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
    return run_axial_slide(5.0f, 5.0f / (0.5f * PHYSICS_GRAVITY),
                           25.0f / (2.0f * 0.5f * PHYSICS_GRAVITY));
}

static int test_negative_slide(void)
{
    return run_axial_slide(-5.0f, 5.0f / (0.5f * PHYSICS_GRAVITY),
                           25.0f / (2.0f * 0.5f * PHYSICS_GRAVITY));
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
           distance > 2.45f &&
           distance < 2.65f &&
           max_line_error < TEST_TOLERANCE;
}

int main(void)
{
    report("4 N force", test_four_newtons_does_not_move());
    report("+5 m/s slide", test_positive_slide());
    report("-5 m/s slide", test_negative_slide());
    report("Diagonal slide", test_diagonal_slide());

    if (failures > 0) {
        printf("FAIL: %d physics check(s) failed\n", failures);
        return 1;
    }

    printf("PASS: all physics checks passed\n");
    return 0;
}
