#include "UIManager.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_win32.h"
#include "IMGUI/imgui_impl_opengl2.h"
#include <SDL.h>
#include <iostream>

void UIManager::Init(HWND hwnd) {
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplOpenGL2_Init();
}

void UIManager::ToggleMenu() {
    showMenu = !showMenu;
    std::cout << "[UI] Toggled menu: " << (showMenu ? "ON" : "OFF") << std::endl;
    SDL_ShowCursor(showMenu ? SDL_ENABLE : SDL_DISABLE);
    SDL_SetRelativeMouseMode(showMenu ? SDL_FALSE : SDL_TRUE);
}


void UIManager::AddFeature(Feature* feature) {
    features.push_back(feature);
    feature->Init();
}

void UIManager::Render() {
    for (auto* feature : features)
        feature->Update();

    if (!showMenu) return;

    ImGui::Begin("AssaultCube Mod");
    for (auto* feature : features)
        feature->RenderUI();
    ImGui::End();
}
