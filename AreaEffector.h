#pragma once
#include "Effector.h"

class AreaEffector : public Effector
{
public:
	AreaEffector(Vector2 position, float size, float angle, float forceMagnatude) :
		Effector(position, size),
		angle{ angle },
		forceMagnatude{ forceMagnatude } {
	}

	void Apply(std::vector<Body>& bodies) override;
	void Draw() override;

private:
	float angle;
	float forceMagnatude;
};
