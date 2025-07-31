#pragma once
#include <vector>
#include "Feature.h"
#include <wtypes.h>

class UIManager {
public:
    void Init(HWND hwnd);
    void Render();
    void ToggleMenu();
    bool IsMenuOpen() const { return showMenu; }

    void AddFeature(Feature* feature);

private:
    bool showMenu = true;
    std::vector<Feature*> features;
};
