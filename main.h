#pragma once

#define GPPHDR_VER 0x10

void quit();
void config();
int init();

char PLUGIN_NAME[] = "Oli plugin";

typedef struct {
    int version;
    char *description;
    int (*init)();
    void (*config)();
    void (*quit)();
    HWND hwndParent;
    HINSTANCE hDllInstance;
} winampGeneralPurposePlugin;
