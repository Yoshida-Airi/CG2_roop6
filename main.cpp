#include <Windows.h>
#include <cstdint>
#include "Windows.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Create an instance of the Window class
    Window mainWindow(hInstance, "My Window", 800, 600);

    // Show the window
    mainWindow.Show();

    // Start the message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int32_t>(msg.wParam);
}
