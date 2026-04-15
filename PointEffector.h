#pragma once
#include "Effector.h"

class PointEffector : public Effector
{
public:
	PointEffector(Vector2 position, float size, float forceMagnatude) :
		position{ position },
		size{ size },
		forceMagnatude{ forceMagnatude } {}

	void Apply(std::vector<Body>& bodies) override;
	void Draw();

private:
	Vector2 position;
	float size;
	float forceMagnatude;
};
