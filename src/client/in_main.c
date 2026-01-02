// in_null.c -- for systems without a mouse

#include "quakedef.h"
#include "input.h"
#include "in_mouse.h"
#include "in_gamepad.h"


void IN_Init(void) {
    IN_InitMouse();
    IN_InitGamepad();
}

void IN_Shutdown(void) {
    IN_ShutdownGamepad();
}

void IN_Move(usercmd_t* cmd) {
    IN_MouseMove(cmd);
    IN_JoyMove(cmd);
}
