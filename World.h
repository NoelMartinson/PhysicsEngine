#pragma once
#include "Body.h"
#include "Collision.h"
#include <vector>

struct World
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

    std::vector<Body>& GetBodies() { return bodies; }
    const std::vector<Body>& GetBodies() const { return bodies; }

private:
    void UpdateCollision();

private:
    static Vector2 gravity;

    std::vector<Body> bodies;
    std::vector<Contact> contacts;
    std::vector<class Effector*> effectors;
};