#pragma once
#include "Body.h"
#include "Collision.h"
#include <vector>

class World
{
public:
    World()
    {
        bodies.reserve(1000);
    }

    void AddBody(const Body& body);
    void AddEffector(class Effector* effector);
    void Step(float dt);
    void Draw();
    void SetBounds(Vector2 min, Vector2 max) { boundsMin = min; boundsMax = max; }


    std::vector<Body>& GetBodies() { return bodies; }
    const std::vector<Body>& GetBodies() const { return bodies; }

    Body* GetBodyIntersect(Vector2 position);

	static void SetGravity(Vector2 newgravity) { gravity = newgravity; }

private:
    void UpdateCollision();

private:
    static Vector2 gravity;

    std::vector<Body> bodies;
    std::vector<Contact> contacts;
    std::vector<class Effector*> effectors;

    Vector2 boundsMin{ -10.0f, -5.0f };
    Vector2 boundsMax{ 10.0f,  5.0f };
};