/* etCommNet - Communication over TCP/IP
	Copyright (C) 2015 by Martin Langlotz alias stackshadow

	This file is part of evillib.

	evillib is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	evillib is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with evillib.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _H_etCommNet
#define _H_etCommNet


#ifdef ET_SINGLEMODULE
	#include "net/etComm.h"
#endif

typedef struct etCommTCP {
	etComm					*etCommActual;

// Network Type ( IPv4 / IPv6 )
	etID_BOOL 				isIPv4;

// socket
	int						socket;
	void					*socketaddr;

// connections
	int						connectionMax;
	int						connectionAct;

} etCommTCP;



etID_STATE				etCommTCPSetAddress( etComm *etCommActual, const char *address, int port );


etID_STATE				etCommTCPConnectionAlloc( etCommConnection **p_etCommConnection, etID_BOOL isIPv4 );


etID_STATE				etCommTCPConnectionSetAddress( etCommConnection *etCommConnectionActual, const char *address, int port );


etID_STATE				etCommTCPServe( etComm *etCommActual, void (*newClientConnection)(etCommConnection*) );


etID_STATE				etCommTCPConnect( etComm *etCommActual );


etID_STATE				etCommTCPRead( etCommConnection *etCommConnectionActual, char *buffer, int bufferLen );


etID_STATE				etCommTCPWrite( etCommConnection *etCommConnectionActual, const char *data, int dataLen );


etID_STATE				etCommTCPDisconnect( etCommConnection *etCommConnectionActual );






#endif