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
// sys.h -- non-portable functions

#ifndef SYS_H
#define SYS_H

//
// file IO
//

int Sys_FileTime(char* path);
void Sys_mkdir(char* path);

//
// system IO
//
void Sys_DebugLog(char* file, char* fmt, ...);

// an error will cause the entire program to exit
void Sys_Error(char* error, ...);

// send text to the console
void Sys_Printf(char* fmt, ...);

void Sys_Quit(void);

// Return time in seconds since start.
double Sys_DoubleTime(void);

char* Sys_ConsoleInput(void);

// Perform Key_Event () callbacks until the input que is empty
void Sys_SendKeyEvents(void);

void Sys_Init(void);

#endif
