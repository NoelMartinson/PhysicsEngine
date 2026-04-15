#include "body.h"
#include "random.h"
#include "integrator.h"
#include "world.h"
#include "pointeffector.h"
#include "gravitationeffector.h"

#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	
#include <vector>
#include <string>

int main()
{
	SetRandomSeed(5);
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 800, "Hello Raylib");
	SearchAndSetResourceDir("resources");

	World world;

	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f;

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
			(IsKeyDown(KEY_C) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
		{
			Body body;

			body.bodyType = (IsKeyDown(KEY_LEFT_ALT)) ? BodyType::Static : BodyType::Dynamic;

			body.position = GetMousePosition();
			float angle = GetRandomFloat() * (2 * PI);
			Vector2 direction;
			direction.x = cosf(angle);
			direction.y = sinf(angle);

			body.AddForce(direction * (50.0f + (GetRandomFloat() * 500)) * 0.001f, ForceMode::VelocityChange);

			body.size = 5.0f + (GetRandomFloat() * 30.0f);
			body.restitution = 0.5f + (GetRandomFloat() * 0.5f);
			body.mass = body.size;
			body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : 1.0f / body.mass;
			body.gravityScale = 0.0f;
			body.dampening = 0.1f;

			world.AddBody(body);
		}

		timeAccum += dt;
		while (timeAccum > fixedTimeStep)
		{
			world.Step(fixedTimeStep);
			timeAccum -= fixedTimeStep;
		}

		BeginDrawing();

		ClearBackground(BLACK);

		std::string fpsText = "FPS: ";
		fpsText += std::to_string(GetFPS());
		DrawText(fpsText.c_str(), 40, 40, 20, WHITE);

		world.Draw();

		EndDrawing();
	}
	CloseWindow();
	return 0;
}