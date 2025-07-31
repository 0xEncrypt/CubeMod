#include <Windows.h>
#include <iostream>
#include <gl/GL.h>

#include "MINHOOK/MinHook.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_win32.h"
#include "IMGUI/imgui_impl_opengl2.h"

#include <SDL.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "SDL2.lib")

#include "UIManager.h"
#include "HealthFeature.h"

typedef BOOL(WINAPI* wglSwapBuffers_t)(HDC hdc);
wglSwapBuffers_t oSwapBuffers = nullptr;

WNDPROC oWndProc = nullptr;

static UIManager uiManager;
static HealthFeature healthFeature;


LRESULT CALLBACK hkWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui::GetCurrentContext()) {
        ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
    }

    return CallWindowProc(oWndProc, hwnd, msg, wParam, lParam);
}



BOOL WINAPI hkSwapBuffers(HDC hdc) {
    static bool initialized = false;

    if (!initialized) {
        HWND hwnd = WindowFromDC(hdc);
        uiManager.Init(hwnd);
        uiManager.AddFeature(&healthFeature);

        // Hook WndProc here
        oWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)hkWndProc);

        initialized = true;
    }


    static bool menuKeyDown = false;

    if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
        if (!menuKeyDown) {
            menuKeyDown = true;
            uiManager.ToggleMenu();
        }
    }
    else {
        menuKeyDown = false;
    }


    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    uiManager.Render();

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    return oSwapBuffers(hdc);
}

DWORD WINAPI MainThread(LPVOID lpParam) {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    SetConsoleTitleA("AC MOD Console");

    if (MH_Initialize() != MH_OK) return 1;

    void* target = GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
    if (!target || MH_CreateHook(target, &hkSwapBuffers, reinterpret_cast<void**>(&oSwapBuffers)) != MH_OK)
        return 1;

    if (MH_EnableHook(target) != MH_OK) return 1;

    std::cout << "[+] ImGui Hook Ready\n";

    while (true) {
        if (GetAsyncKeyState(VK_DELETE) & 1) break;
        Sleep(100);
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    FreeConsole();

    FreeLibraryAndExitThread((HMODULE)lpParam, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(NULL, 0, MainThread, hModule, 0, NULL);
    return TRUE;
}
