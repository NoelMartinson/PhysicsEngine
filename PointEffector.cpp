#include "PointEffector.h"

void PointEffector::Apply(std::vector<Body>& bodies)
{
    for (auto& body : bodies)
    {
        Vector2 dir = body.position - position;

        float dist = Vector2Length(dir);
        if (dist <= size && dist > 0.0001f)
        {
            Vector2 force = Vector2Normalize(dir) * forceMagnatude;
            body.AddForce(force);
        }
    }
}

void PointEffector::Draw()
{
	DrawCircleLinesV(position, size, WHITE);
}
