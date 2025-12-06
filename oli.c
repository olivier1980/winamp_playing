#include <windows.h>
#include "wa_ipc.h"   // Winamp IPC header

// Forward declarations
int init() {
    //hwndWinamp = FindWindow("Winamp 5.666 Build 3516", NULL);
    // Example: print current song filename to debug console
    timerId = SetTimer(NULL, 1, 500, (TIMERPROC)TimerProc); // check every 500ms

    return 0;
}

void config();
void quit();

typedef struct {
    int version;
    char *description;
    int (*init)();
    void (*config)();
    void (*quit)();
    HWND hwndParent;
    HINSTANCE hDllInstance;
} winampGeneralPurposePlugin;

winampGeneralPurposePlugin plugin = {
    0x10,
    "Oli Plugin",
    init,
    config,
    quit,
    0,
    0
};

HWND hwndWinamp;
HINSTANCE hInstance;
UINT_PTR timerId;
int lastState = 0;

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    if (!plugin.hwndParent) return; // safety check

    wchar_t orig[MAX_PATH] = {0};
    int curpos = SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_GETLISTPOS);
    lstrcpynW(orig,(wchar_t*)SendMessage(plugin.hwndParent,WM_WA_IPC, curpos,IPC_GETPLAYLISTFILEW),sizeof(orig));
}


void config() {
    // No GUI, leave empty
}

void quit() {
    // Cleanup if needed
    KillTimer(NULL, timerId);
}

__declspec(dllexport) winampGeneralPurposePlugin *winampGetPluginInfo() {
    return &plugin;
}

