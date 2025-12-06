#include <windows.h>
#include "wa_ipc.h"   // Winamp IPC header
#include "main.h"
#include "Timer.h"
#include <string>
#include <fstream>

HWND hwndWinamp;
HINSTANCE hInstance;
Timer g_timer;

std::wstring path = L"/tmp/oli_current_playing.txt";

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
        //MessageBox(g_plugin.hwndParent, title.c_str(), L"Debug", MB_OK);

        HANDLE hFile = CreateFileW(path.c_str(), GENERIC_WRITE, 0, NULL,
                                   CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile != INVALID_HANDLE_VALUE) {
            std::wstring line = title;

            // Convert wide string (UTF‑16) to UTF‑8
            int len = WideCharToMultiByte(CP_UTF8, 0, title.c_str(), -1,
                                          nullptr, 0, nullptr, nullptr);
            std::string utf8line(len - 1, '\0'); // -1 to drop null terminator
            WideCharToMultiByte(CP_UTF8, 0, title.c_str(), -1,
                                &utf8line[0], len, nullptr, nullptr);

            DWORD written;
        WriteFile(hFile, utf8line.c_str(),
                  (DWORD)utf8line.size(), &written, NULL);
            CloseHandle(hFile);
        }

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

