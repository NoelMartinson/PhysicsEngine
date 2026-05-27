#include "GravitationEffector.h"
#include <iostream>

void GravitationEffector::Apply(std::vector<Body>& ibodies)
{
    std::vector<Body*> bodies;
    CollectBodiesInside(ibodies, bodies);

    for (size_t i = 0; i < bodies.size(); i++)
    {
        for (size_t j = i + 1; j < bodies.size(); j++)
        {
            Body& bodyA = *bodies[i];
            Body& bodyB = *bodies[j];

            // STEP 1: Direction vector
			Vector2 direction = bodyB.position - bodyA.position;

                // STEP 2: Distance
			float distance = Vector2Length(direction);

                // STEP 3: Clamp distance
			distance = std::max(distance, 1.0f); // Prevent division by zero

                // STEP 4: Force magnitude
			float forceMagnitude = strength * (bodyA.mass * bodyB.mass) / (distance * distance);

            // STEP 5: Force vector
			Vector2 force = Vector2Scale(Vector2Normalize(direction), forceMagnitude);

                // STEP 6: Apply equal and opposite forces
			bodyA.AddForce(force);
            bodyB.AddForce(force * -1);
        }
    }
}

void GravitationEffector::Draw()
{
    Effector::Draw();
    DrawCircleV(position, size, Fade(WHITE, 0.5f));
}