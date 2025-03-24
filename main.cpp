#include <windows.h>
#include <vector>
#include <algorithm> // For std::min and std::max

// Window dimensions
const int width = 800;
const int height = 600;

// Color structure (RGB)
struct Color {
    unsigned char r, g, b;
};

// Framebuffer type (2D vector of colors)
using Framebuffer = std::vector<std::vector<Color>>;

// Global framebuffer
Framebuffer framebuffer;

// Clear the framebuffer with a background color
void ClearFramebuffer(Framebuffer& framebuffer, int width, int height, const Color& bgColor) {
    framebuffer.resize(height, std::vector<Color>(width, bgColor));
}

// Draw a triangle using barycentric coordinates
void DrawTriangle(Framebuffer& framebuffer, int x0, int y0, int x1, int y1, int x2, int y2, const Color& color) {
    auto edgeFunction = [](int ax, int ay, int bx, int by, int cx, int cy) {
        return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
    };

    // Calculate bounding box
    int minX = std::min(std::min(x0, x1), x2);
    int maxX = std::max(std::max(x0, x1), x2);
    int minY = std::min(std::min(y0, y1), y2);
    int maxY = std::max(std::max(y0, y1), y2);

    // Rasterize the triangle
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            int w0 = edgeFunction(x1, y1, x2, y2, x, y);
            int w1 = edgeFunction(x2, y2, x0, y0, x, y);
            int w2 = edgeFunction(x0, y0, x1, y1, x, y);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                framebuffer[y][x] = color;
            }
        }
    }
}

// Draw the framebuffer to the window
void DrawFramebufferToWindow(HDC hdc, const Framebuffer& framebuffer) {
    for (int y = 0; y < framebuffer.size(); ++y) {
        for (int x = 0; x < framebuffer[y].size(); ++x) {
            const Color& pixel = framebuffer[y][x];
            SetPixel(hdc, x, y, RGB(pixel.r, pixel.g, pixel.b));
        }
    }
}

// Window procedure (handles window events I think)
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Draw the framebuffer to the window
            DrawFramebufferToWindow(hdc, framebuffer);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Main entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class
    const char* CLASS_NAME = "SoftwareRenderWindowClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, hInstance, nullptr
    );
    if (hwnd == nullptr) {
        return 1;
    }

    // Initialize the framebuffer
    ClearFramebuffer(framebuffer, width, height, {0, 0, 0}); // Clear to black

    // Why did this take so long to get right
    DrawTriangle(framebuffer, 100, 100, 700, 100, 400, 500, {255, 0, 0}); // Red triangle

    // Show the window
    ShowWindow(hwnd, nCmdShow);

    // Force stuff to happen I guess
    InvalidateRect(hwnd, nullptr, TRUE);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}