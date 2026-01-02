/*
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) Henrique Barateli, <henriquejb194@gmail.com>, et al.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
// winquake.h: Win32-specific Quake header file

#ifndef WINQUAKE_H
#define WINQUAKE_H

#ifdef _WIN32

#include <windows.h>
#define WM_MOUSEWHEEL 0x020A

#ifndef SERVERONLY
#include <ddraw.h>
#include <dsound.h>
#endif

extern HINSTANCE global_hInstance;
extern int global_nCmdShow;

#ifndef SERVERONLY

extern LPDIRECTDRAW lpDD;
extern qboolean DDActive;
extern LPDIRECTDRAWSURFACE lpPrimary;
extern LPDIRECTDRAWSURFACE lpFrontBuffer;
extern LPDIRECTDRAWSURFACE lpBackBuffer;
extern LPDIRECTDRAWPALETTE lpDDPal;
extern LPDIRECTSOUND pDS;
extern LPDIRECTSOUNDBUFFER pDSBuf;

extern DWORD gSndBufSize;
//#define SNDBUFSIZE 65536

void VID_LockBuffer(void);
void VID_UnlockBuffer(void);

#endif

typedef enum { MS_WINDOWED, MS_FULLSCREEN, MS_FULLDIB, MS_UNINIT } modestate_t;

extern modestate_t modestate;

extern HWND mainwindow;
extern qboolean ActiveApp;
extern qboolean Minimized;

extern qboolean WinNT;

int VID_ForceUnlockedAndReturnState(void);
void VID_ForceLockState(int lk);

extern qboolean winsock_lib_initialized;

extern int window_center_x;
extern int window_center_y;
extern RECT window_rect;

extern qboolean mouseinitialized;
extern HWND hwnd_dialog;

void IN_UpdateClipCursor(void);
void CenterWindow(HWND hWndCenter, int width, int height, BOOL lefttopjustify);

void S_BlockSound(void);
void S_UnblockSound(void);

void VID_SetDefaultMode(void);

#endif

#endif
