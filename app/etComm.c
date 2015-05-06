/* Apicheck Function for etString
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


#include "evillib_defines.h"
#include "evillib_depends.h"




#ifdef ET_SINGLEMODULE
	#include <stdio.h>
	#include <stdlib.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <sys/socket.h>

// Headers
	#include "core/etDebug.h"
	#include "core/etVersion.h"
	#include "etApicheck.h"
	#include "memory/etMemoryBlock.h"
	#include "memory/etMemory.h"
	#include "string/etString.h"
	#include "system/etThread.h"
	#include "net/etComm.h"
	#include "net/etCommTCP.h"

// Sources
	#include "core/etObject.h"
	#include "net/etComm.c"
	#include "net/etCommTCP.c"
#endif


 
#ifndef   NI_MAXHOST
#define   NI_MAXHOST 1025
#endif



const char msgHello[] = "HELLO\0";
const char msgVersion[] = "VERSION\0";
const char msgQuit[] = "QUIT\0";

etCommTCP *etCommTCPServer = NULL;

void				etCommServerGetClientConnection( etCommConnection *etCommConnectionActual ){
	
	etDebugMessage( etID_LEVEL_DETAIL_NET, "Hello client, i read some data..." );
	
	char buffer[256];

	while ( 1 ){

		memset( &buffer, 0, 256 );
		
		etCommTCPRead( etCommConnectionActual, &buffer[0], 255 );

		if( strncmp( buffer, msgHello, sizeof(msgHello) ) == 0 ){
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "Hello client ;)" );
			etDebugMessage( etID_LEVEL_DETAIL_NET, etDebugTempMessage );
		}

		if( strncmp( buffer, msgVersion, sizeof(msgVersion) ) == 0 ){
			const char *versionString = (char*)etLibVersion();
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "evillib-version: %s", versionString );
			etDebugMessage( etID_LEVEL_DETAIL_NET, etDebugTempMessage );
		}
		
		if( strncmp( buffer, msgQuit, sizeof(msgQuit) ) == 0 ){
			etCommTCPDisconnect( etCommConnectionActual );
			break;
		}

	}


}

void				etCommServerThread( void *etThreadVoid ){
	etThread *etThreadActual = (etThread*)etThreadVoid;

// Get userdate
	void		*threadUserData;
	etComm 		*etCommActual = NULL;
	
	etThreadGetUserdata( etThreadActual, threadUserData );
	etCommActual = threadUserData;
	

	etCommTCPSetAddress( etCommActual, "::1", 10000 );
	etCommTCPServe( etCommActual, etCommServerGetClientConnection );

	etThreadFinish( etThreadActual );
}



void				etCommClientThread( void *etThreadVoid ){
	etThread *etThreadActual = (etThread*)etThreadVoid;

// Get userdate
	etComm *etCommActual = NULL;
	//etThreadGetUserdata( etThreadActual, etCommActual );


// The client
	etCommAlloc( etCommActual );
	etCommTCPSetAddress( etCommActual, "::1", 10000 );
	etCommTCPConnect( etCommActual );

// write
	etCommTCPWrite( etCommActual->connection, msgHello, sizeof(msgHello) );
	sleep(2);
	
	etCommTCPWrite( etCommActual->connection, msgVersion, sizeof(msgVersion) );
	sleep(2);

	etCommTCPWrite( etCommActual->connection, msgQuit, sizeof(msgQuit) );
	sleep(2);
	


	etThreadFinish( etThreadActual );
}

etID_STATE			etCommTest(){


// The client
	etComm *etCommActual = NULL;
	etCommAlloc( etCommActual );
	//etCommSet( etCommActual, "TCP" );


// Error testing TCP
	etCommTCPServe( etCommActual, etCommServerGetClientConnection );



// Our server thread
	etThread *etThreadServer = NULL;
	etThreadAlloc( etThreadServer );
	etThreadSetFunction( etThreadServer, etCommServerThread );
	etThreadSetUserdata( etThreadServer, etCommActual );
	etThreadRun( etThreadServer );
	sleep(5);



// Client
	etThread *etThreadClient = NULL;
	etThreadAlloc( etThreadClient );
	etThreadSetFunction( etThreadClient, etCommClientThread );
	etThreadSetUserdata( etThreadClient, etCommActual );
	etThreadRun( etThreadClient );


	etThreadWait( etThreadClient );
	etThreadFree( etThreadClient );


	etThreadFree( etThreadServer );


    return 0;
}


