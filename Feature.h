#pragma once

class Feature {
public:
    virtual ~Feature() = default;

    // Called once during setup
    virtual void Init() {}

    // Called every frame to update logic
    virtual void Update() {}

    // Called to draw settings in the UI
    virtual void RenderUI() = 0;

    // Enable/disable
    bool IsEnabled() const { return enabled; }
    void SetEnabled(bool on) { enabled = on; }

protected:
    bool enabled = false;
};
