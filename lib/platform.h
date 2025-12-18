#pragma once

#include <dwmapi.h>
#include <psapi.h>
#include <windows.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace platform::windows
{
    inline WNDPROC g_originalProc = nullptr;

    inline LRESULT CALLBACK CustomWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        return CallWindowProcA(g_originalProc, hwnd, msg, wParam, lParam);
    }

    inline void setWindowDarkMode(const sf::RenderWindow &window)
    {
        HWND hwnd = reinterpret_cast<HWND>(window.getNativeHandle());

        g_originalProc = reinterpret_cast<WNDPROC>(
            SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR)CustomWndProc));

        BOOL dark = TRUE;
        DwmSetWindowAttribute(hwnd, 20, &dark, sizeof(dark));
        DwmSetWindowAttribute(hwnd, 19, &dark, sizeof(dark));

        ShowWindow(hwnd, SW_MINIMIZE);
        ShowWindow(hwnd, SW_RESTORE);

        SendMessageW(hwnd, WM_NCACTIVATE, TRUE, 0);
    }

    inline size_t getMemoryUsage()
    {
        PROCESS_MEMORY_COUNTERS pmc;

        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        {
            return pmc.WorkingSetSize;
        }

        return 0;
    }

}