#include "HealthFeature.h"
#include <Windows.h>
#include <iostream>
#include "IMGUI/imgui.h"

#define BASE_ADDRESS 0x0018AC00
#define HEALTH_OFFSET 0xEC

void HealthFeature::Update() {
    if (!enabled) return;

    std::cout << "Update: running...\n";
    uintptr_t moduleBase = (uintptr_t)GetModuleHandleA("ac_client.exe");
    uintptr_t* basePtr = (uintptr_t*)(moduleBase + BASE_ADDRESS);
    if (!basePtr || !*basePtr) return;

    int* health = (int*)(*basePtr + HEALTH_OFFSET);
    if (health) *health = 1337;
}


void HealthFeature::RenderUI() {
    ImGui::Checkbox("Godmode (Health = 1337)", &enabled);
    std::cout << "RenderUI: enabled = " << enabled << std::endl;
}
