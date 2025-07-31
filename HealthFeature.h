#pragma once
#include "Feature.h"

class HealthFeature : public Feature {
public:
    void Update() override;
    void RenderUI() override;

private:
    int* healthAddr = nullptr;
};
