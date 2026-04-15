#pragma once
#include "Effector.h"

class GravitationEffector : public Effector
{
public:
    GravitationEffector(float strength) : strength(strength) {}

    void Apply(std::vector<Body>& bodies) override;

private:
    float strength;
};