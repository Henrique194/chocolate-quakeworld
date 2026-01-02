// sys_null.h -- null system driver to aid porting efforts

#include "quakedef.h"
#include "in_gamepad.h"
#include "in_keyboard.h"
#include "in_mouse.h"
#include <fcntl.h>
#include <SDL.h>


int Sys_FileTime(char* path) {
    FILE* f = fopen(path, "rb");
    if (f) {
        fclose(f);
        return 1;
    }
    return -1;
}

void Sys_mkdir(char* path) {
}


/*
===============================================================================

SYSTEM IO

===============================================================================
*/


void Sys_DebugLog(char* file, char* fmt, ...) {
    va_list argptr;
    static char data[1024];

    va_start(argptr, fmt);
    vsprintf(data, fmt, argptr);
    va_end(argptr);

    FILE* f = fopen(file, "a");
    if (f) {
        fwrite(data, 1, strlen(data), f);
        fclose(f);
    }
}

void Sys_Error(char* error, ...) {
    va_list argptr;

    printf("Error: ");
    va_start(argptr, error);
    vprintf(error, argptr);
    va_end(argptr);
    printf("\n");

    Host_Shutdown();
    exit(1);
}

void Sys_Printf(char* fmt, ...) {
    va_list argptr;

    va_start(argptr, fmt);
    vprintf(fmt, argptr);
    va_end(argptr);
}

void Sys_Quit(void) {
    Host_Shutdown();
    exit(0);
}

double Sys_DoubleTime(void) {
    static double frequency = 0.0;
    static Uint64 start_time = 0;
    if (start_time == 0) {
        frequency = (double) SDL_GetPerformanceFrequency();
        start_time = SDL_GetPerformanceCounter();
        return 0;
    }
    Uint64 now = SDL_GetPerformanceCounter();
    double time_diff = (double) (now - start_time);
    return time_diff / frequency;
}

char* Sys_ConsoleInput(void) {
    return NULL;
}


/*
================================================================================

SYSTEM EVENT POLLING

================================================================================
*/

static void Sys_QuitEvent(void) {
    if (M_IsInQuitScreen()) {
        // Confirm quit.
        Key_Event('Y', true);
        return;
    }
    // Bring up the quit confirmation screen.
    Cmd_ExecuteString("quit");
}

void Sys_SendKeyEvents(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                IN_KeyboardEvent(&event);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEWHEEL:
                IN_MouseEvent(&event);
                break;
            case SDL_CONTROLLERDEVICEADDED:
            case SDL_CONTROLLERDEVICEREMOVED:
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
            case SDL_CONTROLLERAXISMOTION:
                IN_GamepadEvent(&event);
                break;
            case SDL_QUIT:
                Sys_QuitEvent();
                break;
            default:
                break;
        }
    }
}

//==============================================================================

#define DEFAULT_MEMORY (16 * 1024 * 1024)

void Sys_Init(void) {
}

int main(int argc, char** argv) {
    quakeparms_t parms;

    parms.memsize = DEFAULT_MEMORY;
    if (COM_CheckParm("-heapsize")) {
        int t = COM_CheckParm("-heapsize") + 1;
        if (t < com_argc) {
            parms.memsize = Q_atoi(com_argv[t]) * 1024;
        }
    }
    parms.membase = malloc(parms.memsize);
    parms.basedir = ".";

    COM_InitArgv(argc, argv);

    parms.argc = com_argc;
    parms.argv = com_argv;

    printf("Host_Init\n");
    Host_Init(&parms);

    double oldtime = Sys_DoubleTime();
    while (true) {
        // Find time spent rendering last frame.
        double newtime = Sys_DoubleTime();
        double time = newtime - oldtime;
        Host_Frame((float) time);
        oldtime = newtime;
    }
}
