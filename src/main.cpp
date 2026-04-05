#include "Body.h"
#include "World.h"
#include "Random.h"

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"

int main()
{
    SetRandomSeed(5);
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1280, 800, "Hello Raylib");

    SearchAndSetResourceDir("resources");

    Texture wabbit = LoadTexture("wabbit_alpha.png");

    World world;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // Spawn bodies
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
            (IsKeyDown(KEY_K) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
        {
            Body body;
            body.position = GetMousePosition();

            float angle = GetRandomFloat(0.0f, 2 * PI);
            Vector2 direction = { cosf(angle), sinf(angle) };

            body.velocity = direction * (50.0f + GetRandomFloat(300.0f));
            body.acceleration = { 0, 0 };
            body.size = 5.0f + GetRandomFloat(20.0f);
            body.restitution = 0.5f + GetRandomFloat(0.5f);
            body.mass = 1.0f;

            world.AddBody(body);
        }

        // Right-click force
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 position = GetMousePosition();

            for (auto& body : world.bodies)
            {
                Vector2 dir = position - body.position;

                if (Vector2Length(dir) <= 100.0f)
                {
                    Vector2 force = Vector2Normalize(dir) * 5000.0f;
                    body.AddForce(force);
                }
            }

            DrawCircleLinesV(position, 100, WHITE);
        }

        // Update world
        world.Step(dt);

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Hello Raylib", 200, 200, 20, WHITE);
        DrawTexture(wabbit, 400, 200, WHITE);

        DrawCircleV(GetMousePosition(), 5, ORANGE);

        world.Draw();

        EndDrawing();
    }

    UnloadTexture(wabbit);
    CloseWindow();
    return 0;
}