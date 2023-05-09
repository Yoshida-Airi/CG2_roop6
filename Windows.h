#pragma once

#include <Windows.h>
#include <cstdint>

class Window
{
public:
    Window(const wchar_t* title, int width, int height);
    ~Window();

    bool Create();
    void Show(int nCmdShow);
    void MessageLoop();

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    const wchar_t* m_title;
    int m_width;
    int m_height;
    HWND m_hwnd;
};

