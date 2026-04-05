#include "Body.h"
#include "raymath.h"

void Body::AddForce(Vector2 force)
{
    acceleration += force / mass;
}

void Body::Step(float dt)
{
    velocity += acceleration * dt;
    position += velocity * dt;
}

void Body::Draw() const
{
    DrawCircleV(position, size, ORANGE);
}