#pragma once
#include "Body.h"
#include <vector>

struct World
{
    std::vector<Body> bodies;
    Vector2 gravity = { 0, 9.8f };

    void AddBody(const Body& body);
    void Step(float dt);
    void Draw() const;
};