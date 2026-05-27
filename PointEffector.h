#pragma once
#include "Effector.h"

class PointEffector : public Effector
{
public:
	PointEffector(Vector2 position, float size, float forceMagnatude) :
		Effector(position, size),
		forceMagnatude(forceMagnatude) {}

	void Apply(std::vector<Body>& bodies) override;
	void Draw() override;

private:
	float forceMagnatude;
};
