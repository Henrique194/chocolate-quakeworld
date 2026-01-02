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
// net_main.c

#include "quakedef.h"
#include <SDL_net.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/errno.h>
#include <sys/param.h>
#include <unistd.h>
#endif

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 256
#endif


netadr_t net_local_adr;

netadr_t net_from;
sizebuf_t net_message;
// int net_socket;      // non blocking, for receives

static UDPsocket net_socket; // non-blocking, for receives
static SDLNet_SocketSet socket_set;

#define MAX_UDP_PACKET 8192
static byte net_message_buffer[MAX_UDP_PACKET];

//=============================================================================

static void NET_NetAdrToSockAdr(const netadr_t* from, IPaddress* to) {
    to->host = *(const Uint32*) &from->ip;
    to->port = from->port;
}

static void NET_SockAdrToNetAdr(const IPaddress* from, netadr_t* to) {
    *(int*) &to->ip = (int) from->host;
    to->port = from->port;
}

qboolean NET_CompareBaseAdr(netadr_t a, netadr_t b) {
    return a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2]
           && a.ip[3] == b.ip[3];
}


qboolean NET_CompareAdr(netadr_t a, netadr_t b) {
    return NET_CompareBaseAdr(a, b) && a.port == b.port;
}

char* NET_AdrToString(netadr_t a) {
    static char s[64];
    sprintf(s, "%i.%i.%i.%i:%i", a.ip[0], a.ip[1], a.ip[2], a.ip[3],
            SDLNet_Read16(&a.port));
    return s;
}

char* NET_BaseAdrToString(netadr_t a) {
    static char s[64];
    sprintf(s, "%i.%i.%i.%i", a.ip[0], a.ip[1], a.ip[2], a.ip[3]);
    return s;
}

/*
=============
NET_StringToAdr

idnewt
idnewt:28000
192.246.40.70
192.246.40.70:28000
=============
*/
qboolean NET_StringToAdr(const char* s, netadr_t* a) {
    char copy[128];
    SDL_strlcpy(copy, s, sizeof(copy));
    IPaddress sock_adr = {0};
    Uint16 port = 0;
    // Strip off a trailing :port if present.
    for (char* colon = copy; *colon; colon++) {
        if (*colon == ':') {
            *colon = 0;
            port = (Uint16) SDL_atoi(colon + 1);
        }
    }
    if (SDLNet_ResolveHost(&sock_adr, copy, port) == -1) {
        return false;
    }
    NET_SockAdrToNetAdr(&sock_adr, a);
    return true;
}

// Returns true if we can't bind the address locally--in other words,
// the IP is NOT one of our interfaces.
qboolean NET_IsClientLegal(netadr_t* adr) {
    return true;
}


//=============================================================================

qboolean NET_GetPacket(void) {
    UDPpacket packet = {0};
    packet.data = net_message_buffer;
    packet.maxlen = sizeof(net_message_buffer);
    SDLNet_UDP_Recv(net_socket, &packet);
    const int ret = packet.status;
    if (ret == -1) {
        if (errno == EWOULDBLOCK) {
            return false;
        }
        if (errno == ECONNREFUSED) {
            return false;
        }
        Sys_Printf("NET_GetPacket: %s\n", strerror(errno));
        return false;
    }
    net_message.cursize = packet.len;
    NET_SockAdrToNetAdr(&packet.address, &net_from);
    return ret;
}

int NET_CheckSocket(int timeout) {
    return SDLNet_CheckSockets(socket_set, timeout);
}

//=============================================================================

void NET_SendPacket(int length, void* data, netadr_t to) {
    UDPpacket packet = {0};
    packet.status = -1;
    packet.data = data;
    packet.len = length;
    NET_NetAdrToSockAdr(&to, &packet.address);
    if (SDLNet_UDP_Send(net_socket, -1, &packet) > 0) {
        return;
    }
    if (packet.status == -1) {
        if (errno == EWOULDBLOCK) {
            return;
        }
        if (errno == ECONNREFUSED) {
            return;
        }
        Sys_Printf("NET_SendPacket: %s\n", strerror(errno));
    }
}

//=============================================================================

static UDPsocket UDP_OpenSocket(int port) {
    UDPsocket socket = SDLNet_UDP_Open((Uint16) port);
    if (!socket) {
        Sys_Error("UDP_OpenSocket: %s\n", SDLNet_GetError());
    }
    return socket;
}

static void NET_GetLocalAddress(void) {
    char buff[MAXHOSTNAMELEN] = "localhost";
    gethostname(buff, MAXHOSTNAMELEN);
    buff[MAXHOSTNAMELEN - 1] = 0;
    NET_StringToAdr(buff, &net_local_adr);
    const IPaddress* address = SDLNet_UDP_GetPeerAddress(net_socket, -1);
    net_local_adr.port = address->port;
    Con_Printf("IP address %s\n", NET_AdrToString(net_local_adr));
}

/*
====================
NET_Init
====================
*/
void NET_Init(int port) {
    if (SDLNet_Init() == -1) {
        Sys_Error("UDP Initialization failed: %s\n", SDLNet_GetError());
        return;
    }

    //
    // Open the single socket to be used for all communications.
    //
    net_socket = UDP_OpenSocket(port);

    //
    // Init the message buffer.
    //
    net_message.maxsize = sizeof(net_message_buffer);
    net_message.data = net_message_buffer;

    //
    // Determine my name & address.
    //
    NET_GetLocalAddress();

    socket_set = SDLNet_AllocSocketSet(1);
    SDLNet_UDP_AddSocket(socket_set, net_socket);

    Con_Printf("UDP Initialized\n");
}

/*
====================
NET_Shutdown
====================
*/
void NET_Shutdown(void) {
    SDLNet_UDP_Close(net_socket);
}
