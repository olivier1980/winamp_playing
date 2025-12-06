#include <windows.h>
#include "wa_ipc.h"   // Winamp IPC header
#include "main.h"
#include "Timer.h"
#include <string>

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
std::wstring current;

void CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD)
{
    //MessageBox(g_plugin.hwndParent, L"cb called", L"Debug", MB_OK);
}


int init()
{
    if (IsWindowUnicode(g_plugin.hwndParent))
        g_lpWndProcOld = (WNDPROC)SetWindowLongW(g_plugin.hwndParent, GWL_WNDPROC, (LONG)WndProc);
    else
        g_lpWndProcOld = (WNDPROC)SetWindowLongA(g_plugin.hwndParent, GWL_WNDPROC, (LONG)WndProc);

	g_timer.Initialize(g_plugin.hwndParent, TimerProc);

    return 0;
}

void config()
{

}

void quit()
{

}

void ReportCurrentSongStatus(PlaybackState playbackState)
{
    std::string detailsMessage;
    std::wstring title = (wchar_t *)SendMessage(g_plugin.hwndParent, WM_WA_IPC, 0, IPC_GET_PLAYING_FILENAME);

    if (current != title) {
        current = title;
        MessageBox(g_plugin.hwndParent, title.c_str(), L"Debug", MB_OK);
    }
}


void UpdateRichPresenceDetails()
{
	LONG isPlayingResult = SendMessage(g_plugin.hwndParent, WM_WA_IPC, 0, IPC_ISPLAYING);

	if (isPlayingResult == Playing)
	{
		ReportCurrentSongStatus(Playing);
		if (true)
		{
			g_timer.Set();
		}
		else
		{
			g_timer.Stop();
		}
	}
	else if (isPlayingResult == Paused)
	{
		g_timer.Stop();
		ReportCurrentSongStatus(Paused);
	}
	else if (isPlayingResult == Stopped)
	{
		g_timer.Stop();
		//ReportIdleStatus();
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_WA_IPC && lParam == IPC_CB_MISC && wParam == IPC_CB_MISC_STATUS)
    {
		// Notification sent from Winamp on any change in playback.
		UpdateRichPresenceDetails();
    }

    return CallWindowProc(g_lpWndProcOld, hwnd, message, wParam, lParam);
}

extern "C" __declspec(dllexport) winampGeneralPurposePlugin *winampGetGeneralPurposePlugin() {
    return &g_plugin;
}

