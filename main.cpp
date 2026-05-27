#include "body.h"
#include "random.h"
#include "integrator.h"
#include "world.h"
#include "pointeffector.h"
#include "gravitationeffector.h"
#include "AreaEffector.h"
#include "DragEffector.h"
#include "world_camera.h"
#include "Spring.h"

#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	
#include <vector>
#include <string>

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"
#define GUI_PHYSICS_IMPLEMENTATION
#pragma warning ( push )
#pragma warning ( disable : 4576)
#include "gui_physics.h"
#pragma warning ( pop )

void AddBody(World& world, WorldCamera& camera);
void AddEffector(World& world, WorldCamera& camera);

GuiPhysicsState state;
int main()
{

	SetRandomSeed(5);
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 720, "Hello Raylib");
	state = InitGuiPhysics();
	GuiLoadStyle("raygui/styles/dark/style_dark.rgs");
	SearchAndSetResourceDir("resources");

	World world; 
    WorldCamera world_camera(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 5);
    world.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));

    Body* selectedBody = nullptr;
    Body* connectedBody = nullptr;

	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f;
	bool simulate = true;

    while (!WindowShouldClose())
    {
        float fixedFPS = 1.0f / state.FPSValue;
        float dt = fminf(GetFrameTime(), 0.1f);

        if (IsKeyPressed(KEY_SPACE)) state.SimulateActive = !state.SimulateActive;
        if (IsKeyPressed(KEY_TAB)) state.PhysicsPanelActive = !state.PhysicsPanelActive;

        World::SetGravity(Vector2{ 0, state.GravityValue });

        bool mouseOverGUI = state.PhysicsPanelActive &&
            CheckCollisionPointRec(GetMousePosition(),
                Rectangle{ state.anchor02.x, state.anchor02.y, 304, 664 });

        if (!mouseOverGUI)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
                (IsKeyDown(KEY_LEFT_CONTROL) &&
                    IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
            {
                if (IsKeyDown(KEY_LEFT_SHIFT))
                {
                    AddEffector(world, world_camera);
                }
                else
                {
                    AddBody(world, world_camera);
                }
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            {
                selectedBody = world.GetBodyIntersect(world_camera.ScreenToWorld(GetMousePosition()));
            }

            if (selectedBody)
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && IsKeyDown(KEY_LEFT_SHIFT))
                {
                    Vector2 position = world_camera.ScreenToWorld(GetMousePosition());
                    Vector2 force = Spring::GetSpringForce(position, selectedBody->position, 1.0f, 3.0f);
                    selectedBody->AddForce(force);

                    DrawLineV(world_camera.WorldToScreen(position), world_camera.WorldToScreen(selectedBody->position), WHITE);
                }
            }
        }

        if (state.SimulateActive)
        {
            timeAccum += dt;

            while (timeAccum > fixedTimeStep)
            {
                world.Step(fixedTimeStep);
                timeAccum -= fixedTimeStep;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        std::string fpsText = "FPS: ";
        fpsText += std::to_string(GetFPS());

        DrawText(fpsText.c_str(), 40, 40, 20, WHITE);

        world_camera.Begin(); 
        world.Draw(); 
		DrawCircleLinesV(world_camera.ScreenToWorld(GetMousePosition()), state.BodySizeValue, RED);

        if (selectedBody)
        {
            DrawCircleLinesV(selectedBody->position, selectedBody->size * 1.05f, RED);
		}

        world_camera.End(); 
        GuiPhysics(&state);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void AddBody(World& world, WorldCamera& camera)
{
	Body body;

	body.bodyType = (BodyType)state.BodyTypeActive;
    body.position = camera.ScreenToWorld(GetMousePosition());

	float angle = GetRandomFloat() * (2 * PI);
	Vector2 direction;
	direction.x = cosf(angle);
	direction.y = sinf(angle);

	body.AddForce((direction * state.BodyVelocityValue), ForceMode::VelocityChange);

	body.size = state.BodySizeValue;
	body.restitution = state.BodyRestitutionValue;
	body.mass = state.BodyMassValue * state.BodyMassValue;
	body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : 1.0f / body.mass;
	body.gravityScale = state.BodyGravityValue;
	body.dampening = state.BodyDampingValue;

	world.AddBody(body);
};

void AddEffector(World& world, WorldCamera& camera)
{
    Vector2 position = camera.ScreenToWorld(GetMousePosition());
    float size = state.BodySizeValue;

    Effector* effector = nullptr;
    switch ((EffectorType)state.EffectorTypeActive)
    {
    case EffectorType::Gravitation:
        effector = new GravitationEffector(position, size, state.EffectorForceValue);
        break;
    case EffectorType::Point:
        effector = new PointEffector(position, size, state.EffectorForceValue);
        break;
    case EffectorType::Area:
        effector = new AreaEffector(position, size, state.EffectorAngleValue, state.EffectorForceValue);
        break;
    case EffectorType::Drag:
        effector = new DragEffector(position, size, state.EffectorForceValue);
        break;
    }

    if (effector) world.AddEffector(effector);
}