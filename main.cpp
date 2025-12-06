#include <windows.h>
#include "wa_ipc.h"   // Winamp IPC header
#include "main.h"
#include "Timer.h"

HWND hwndWinamp;
HINSTANCE hInstance;
Timer g_timer;

winampGeneralPurposePlugin g_plugin = {
    GPPHDR_VER,
    PLUGIN_NAME,
    init,
    config,
    quit,
    0,
    0
};

void CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC g_lpWndProcOld = 0;


void CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD)
{

}


int init()
{
//    if (IsWindowUnicode(g_plugin.hwndParent))
//        g_lpWndProcOld = (WNDPROC)SetWindowLongW(g_plugin.hwndParent, GWL_WNDPROC, (LONG)WndProc);
//    else
//        g_lpWndProcOld = (WNDPROC)SetWindowLongA(g_plugin.hwndParent, GWL_WNDPROC, (LONG)WndProc);

	g_timer.Initialize(g_plugin.hwndParent, TimerProc);

    return 0;
}

void config()
{

}

void quit()
{

}

extern "C" __declspec(dllexport) winampGeneralPurposePlugin *winampGetGeneralPurposePlugin() {
    return &g_plugin;
}

