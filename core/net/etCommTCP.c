/* etCommNet - Communication with other apps
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

#ifdef ET_SINGLEMODULE
	#include "evillib_defines.h"
	#include "evillib_depends.h"

	#include "core/etDebug.h"
	#include "core/etObject.h"
	#include "memory/etMemoryBlock.h"
	#include "memory/etMemory.h"
	#include "string/etString.h"
	
	#include "net/etComm.h"
	#include "net/etCommTCP.h"
#endif
///proc/net/if_inet6


int 					etCommTCPGetAddrFamily(const char *addr){
    
//  Vars
	struct addrinfo 		hint;
	struct addrinfo 		*info;

// Clean Memory
    memset(&hint, 0, sizeof(hint));

// Setup Hint ( what we would like to have ? )
	hint.ai_family = AF_UNSPEC;
	//hint.ai_flags = AI_NUMERICHOST;

	int ret = getaddrinfo(addr, 0, &hint, &info);
	if (ret)
		return -1;

	int result = info->ai_family;
	freeaddrinfo(info);
	return result;
}


/** @ingroup grComm
@~english
@brief Communication functions
**/
etID_STATE				etCommTCPSetAddress( etComm *etCommActual, const char *address, int port ){
// Object okay ?
	etCheckNull( etCommActual );

// Vars
	etID_BOOL 		isIPv4 = etID_FALSE;
	etCommTCP		*etCommTCPNew = NULL;

// Check IP-Address
	int addrFamily = etCommTCPGetAddrFamily( address );
	if( addrFamily == -1 ) return etID_STATE_ERROR_INTERNAL;

// IPv4	
	if( addrFamily == AF_INET ){
		isIPv4 = etID_TRUE;
		#ifndef ET_DEBUG_OFF			
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "Address %s is an IPv4-Address", address );
			etDebugMessage( etID_LEVEL_DETAIL_NET, etDebugTempMessage );
		#endif
	}

// IPv6
	if( addrFamily == AF_INET6 ){
		isIPv4 = etID_FALSE;
		#ifndef ET_DEBUG_OFF			
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "Address %s is an IPv6-Address", address );
			etDebugMessage( etID_LEVEL_DETAIL_NET, etDebugTempMessage );
		#endif

	}

// Allocate the connection
	if( etCommActual->connection == NULL ){
		
	// Allocate
		etCommTCPConnectionAlloc( &etCommActual->connection, isIPv4 );
		etCommTCPConnectionSetAddress( etCommActual->connection, address, port );
	
	// Get the TCPConnection
		etCommTCPNew = (etCommTCP*)etCommActual->connection;
	
	// Setup Objects
		etCommTCPNew->etCommActual = etCommActual;
	
		return etID_YES;
	}
	
	return etID_NO;
}

/** @ingroup grComm
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@fn etID_STATE etStringAlloc( etStringActual )
@brief Allocate a new etString object
@see standardFunctions

@param[out] etStringActual The pointer to an etString object pointer
@return If the etString object was correctly allocated \n
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*/
etID_STATE				etCommTCPConnectionAlloc( etCommConnection **p_etCommConnection, etID_BOOL isIPv4 ){
// Object okay ?
	etCheckNull( p_etCommConnection );

// Vars
	etMemoryBlock 			*etMemoryBlockNew = NULL;
	etCommTCP				*etCommTCPNew = NULL;


// Allocate memory
	if( isIPv4 == etID_TRUE ){
		etMemoryAlloc( etMemoryBlockNew, sizeof(etCommTCP) + sizeof(struct sockaddr_in) );
	} else {
		etMemoryAlloc( etMemoryBlockNew, sizeof(etCommTCP) + sizeof(struct sockaddr_in6) );
	}

// Setup Memory Address
	etMemoryBlockDataGet( etMemoryBlockNew, etCommTCPNew );
	etCommTCPNew->socketaddr = &etCommTCPNew[1];

	*p_etCommConnection = etCommTCPNew;
	return etID_YES;
}


etID_STATE				etCommTCPConnectionSetAddress( etCommConnection *etCommConnectionActual, const char *address, int port ){
// Object okay ?
	etCheckNull( etCommConnectionActual );

// Vars
	etCommTCP		*etCommTCPActual = (etCommTCP*)etCommConnectionActual;

// Create socket
	if( etCommTCPActual->isIPv4 == etID_TRUE ){
		etCommTCPActual->socket = socket( AF_INET, SOCK_STREAM, 0 );
	} else {
		etCommTCPActual->socket = socket( AF_INET6, SOCK_STREAM, 0 );
	}

// Setup socket
	if( etCommTCPActual->isIPv4 == etID_TRUE ){
		struct sockaddr_in *socketTCPv4 = (struct sockaddr_in*)etCommTCPActual->socketaddr;
		socketTCPv4->sin_family = AF_INET;
		socketTCPv4->sin_port = htons( port );

		if( address == NULL ){
			socketTCPv4->sin_addr.s_addr = INADDR_ANY;
		} else {
			inet_pton( AF_INET, address, &(socketTCPv4->sin_addr) );
		}
	} else {
		struct sockaddr_in6 *socketTCPv6 = (struct sockaddr_in6*)etCommTCPActual->socketaddr;
		socketTCPv6->sin6_family = AF_INET6;
		socketTCPv6->sin6_flowinfo = 0;
		socketTCPv6->sin6_port = htons( port );

		if( address == NULL ){
			socketTCPv6->sin6_addr = in6addr_any;
		} else {
			inet_pton( AF_INET6, address, &(socketTCPv6->sin6_addr) );
		}
	}

	return etID_YES;
}


etID_STATE				etCommTCPServe( etComm *etCommActual, void (*newClientConnection)(etCommConnection*) ){
// Object okay ?
	etCheckNull( etCommActual );

// Vars
	etCommTCP				*etCommTCPServer = NULL;
	etCommTCP				*etCommTCPClient = NULL;
	socklen_t				sockaddSize;
	int 					error = -1;

// Basic Check
	if( etCommActual->connection == NULL ) { etDebugMessage( etID_LEVEL_ERR, "No main connection is set. Use etCommTCPSetAddress() !" ); return etID_STATE_PARAMETER_MISSUSE; }
	etCommTCPServer = (etCommTCP*)etCommActual->connection;

// Get size
	if( etCommTCPServer->isIPv4 == etID_TRUE ){
		sockaddSize = sizeof(struct sockaddr_in);
	} else {
		sockaddSize = sizeof(struct sockaddr_in6);
	}


// bind
	error = bind( etCommTCPServer->socket, (struct sockaddr *)(etCommTCPServer->socketaddr), sockaddSize );
	if( error == -1){
		etDebugMessage( etID_LEVEL_ERR, strerror(errno) );
		etCommTCPServer->socket = -1;
		return etID_NO;
	}

// start to listen
	error = listen( etCommTCPServer->socket, 5 );
	if( error == -1 ){
		etDebugMessage( etID_LEVEL_ERR, strerror(errno) );
		etCommTCPServer->socket = -1;
		return etID_NO;
	}


// debug info
#ifndef ET_DEBUG_OFF
	char hostname[255];
	error = getnameinfo( (struct sockaddr *)etCommTCPServer->socketaddr, sockaddSize, hostname, sizeof(hostname), NULL, 0, 0); 
	
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Wait for connection on %s", hostname );
	etDebugMessage( etID_LEVEL_DETAIL_NET, etDebugTempMessage );
#endif

	while( etCommTCPServer->socket > 0 ){


	// Allocate client connection
		etCommTCPClient = NULL;
		etCommTCPConnectionAlloc( (etCommConnection**)&etCommTCPClient, etCommTCPServer->isIPv4 );

		#ifndef ET_DEBUG_OFF
			etDebugMessage( etID_LEVEL_DETAIL_NET, "Wait for new client ..." );
		#endif
	
	// Accept connections
		etCommTCPClient->socket = accept( etCommTCPServer->socket, (struct sockaddr *)etCommTCPClient->socketaddr, &sockaddSize);
		if( etCommTCPClient->socket < 0 ){
			etDebugMessage( etID_LEVEL_ERR, strerror(errno) );
			continue;
		}
		
	// Debugging info
	#ifndef ET_DEBUG_OFF
		char hostname[255];
		/* nt error = */ getnameinfo( (struct sockaddr *)etCommTCPClient->socketaddr, sockaddSize, hostname, sizeof(hostname), NULL, 0, 0);
		
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "Get connection from Host %s", hostname );
		etDebugMessage( etID_LEVEL_DETAIL_NET, etDebugTempMessage );
	#endif
		
	// Call new-client-connection-function
		if( newClientConnection != NULL ){
			newClientConnection( (etCommConnection*)etCommTCPClient );
		}

	}

	return etID_YES;

/*

// bind process to port
	if( etCommTCPActual->isIPv6 == etID_TRUE ) {
		if( bind( etCommTCPActual->socket, (struct sockaddr *)(etCommTCPActual->socketIn), sizeof(struct sockaddr_in6)) == -1){
			etDebugMessage( etID_LEVEL_DETAIL_NET, strerror(errno) );
			//etID_LEVEL_DETAIL_NET
		}
	}

// start to listen
	if( listen( etCommTCPActual->socket, 5 ) == -1 ){
		etDebugMessage( etID_LEVEL_DETAIL_NET, strerror(errno) );
	}


    struct sockaddr_in6 cli_addr;
	socklen_t clilen = sizeof(cli_addr);


	int newsockfd = accept( etCommTCPActual->socket, (struct sockaddr *)&cli_addr, &clilen);
	if (newsockfd < 0){
		etDebugMessage( etID_LEVEL_DETAIL_NET, strerror(errno) );
	}
*/
}


etID_STATE				etCommTCPConnect( etComm *etCommActual ){

// Vars
	etCommTCP				*etCommTCPClient = NULL;
	socklen_t				sockaddSize;
	int 					error = -1;

// Basic Check
	if( etCommActual->connection == NULL ) { etDebugMessage( etID_LEVEL_ERR, "No main connection is set. Use etCommTCPSetAddress() !" ); return etID_STATE_PARAMETER_MISSUSE; }
	etCommTCPClient = etCommActual->connection;


// Get size
	if( etCommTCPClient->isIPv4 == etID_TRUE ){
		sockaddSize = sizeof(struct sockaddr_in);
	} else {
		sockaddSize = sizeof(struct sockaddr_in6);
	}	


// Connect to client
	error = connect( etCommTCPClient->socket, (struct sockaddr *)etCommTCPClient->socketaddr, sockaddSize );
	if( error != 0 ){
		etDebugMessage( etID_LEVEL_ERR, strerror(errno) );
		etCommTCPClient->socket = -1;
		return etID_NO;
	}

	return etID_YES;
}


etID_STATE				etCommTCPRead( etCommConnection *etCommConnectionActual, char *buffer, int bufferLen ){
// Object okay ?
	etCheckNull( etCommConnectionActual );

// Vars
	etCommTCP				*etCommTCPActual = (etCommTCP*)etCommConnectionActual;
	ssize_t 				bytesReaded = -1;

	if( etCommTCPActual->socket > 0 ){
		bytesReaded = read( etCommTCPActual->socket, buffer, bufferLen );
		
	// No data
		if( bytesReaded == 0 ){
			return etID_STATE_NODATA;
		}
	
	// Data
		if( bytesReaded > 0 ){
			return etID_STATE_DATA_AVIABLE;
		}
		
	// Error
		if( bytesReaded < 0 ){
			return etID_STATE_ERROR_INTERNAL;
		}
	}

	return etID_NO;
}


etID_STATE				etCommTCPWrite( etCommConnection *etCommConnectionActual, const char *data, int dataLen ){
// Object okay ?
	etCheckNull( etCommConnectionActual );

// Vars
	etCommTCP				*etCommTCPActual = (etCommTCP*)etCommConnectionActual;

	if( etCommTCPActual->socket > 0 ){
		
		#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "Sennd string %s", data );
			etDebugMessage( etID_LEVEL_DETAIL_NET, etDebugTempMessage );
		#endif
		
		write( etCommTCPActual->socket, data, dataLen );
		return etID_YES;
	}

	return etID_NO;
}


etID_STATE				etCommTCPDisconnect( etCommConnection *etCommConnectionActual ){
// Object okay ?
	etCheckNull( etCommConnectionActual );

// Vars
	etCommTCP				*etCommTCPActual = (etCommTCP*)etCommConnectionActual;

	close( etCommTCPActual->socket );
	etCommTCPActual->socket = -1;
	
	return etID_YES;
}


