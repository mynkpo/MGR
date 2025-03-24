//Created 9:20 PM CST, 3.23.2025

#include <windows.h>
#include <stdio.h>
#include <vector>

const int width = 800;
const int height = 600;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:

            PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    const char* CLASS_NAME = "SoftwareRenderWindowClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, hInstance, nullptr
    );
    if (hwnd == nullptr) {
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

struct Color {
    unsigned char r, g, b;
};

using Framebuffer = std::vector<std::vector<Color>>;

void ClearFramebuffer(Framebuffer& framebuffer, int width, int height, const Color& bgColor) {
    framebuffer.resize(height, std::vector<Color>(width, bgColor));
} // 10:45 PM CST
