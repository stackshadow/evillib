/* etComm - Communication with other apps
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
	#include "string/etStringChar.h"
	
	#include "net/etComm.h"
#endif


/** @ingroup grComm
@~english
@brief Communication functions
**/

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
etID_STATE				__etCommAlloc( etComm **p_etCommActual ){
// Object okay ?
	etCheckNull( p_etCommActual );


// Vars
	etMemoryBlock 	*etMemoryBlockNew = NULL;
	etComm		 	*etCommNew = NULL;

// Alloc
	etMemoryAlloc( etMemoryBlockNew, sizeof(etComm) );
	etMemoryBlockDataGet( etMemoryBlockNew, etCommNew );			
	if( etCommNew == NULL ){
		return etDebugState(etID_STATE_NOMEMORY);
	}

// Return
	*p_etCommActual = etCommNew;
	return etID_YES;
}


etID_STATE				etCommSet( etComm *etCommActual, const char *commType ){
// Object okay ?
	etCheckNull( etCommActual );
	

// Vars
	etComm		*etCommNew = NULL;
	etString	*functionName = NULL;

// Setup
	etStringAllocLen( functionName, 64 );
	etStringInit( functionName );
	etStringCharSet( functionName, "etComm", 6 );
	etStringCharAdd( functionName, commType );
	etStringCharAdd( functionName, "Alloc" );

// Load aviable library functions
	void *dlHandle = dlopen( NULL, RTLD_LAZY );
	if( dlHandle == NULL ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "%s", dlerror() );
		etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
		return etID_LEVEL_ERR;
	}
	
	dlerror(); //clear any error
	
	const char *functionNameChar;
	etStringCharGet( functionName, functionNameChar );
	void (*function)(etComm **etCommActual) = dlsym( dlHandle, functionNameChar );

// Run the function
	if( function != NULL ){
		function( &etCommNew );
	}


// Cleanup
	etStringFree( functionName );

	return etID_YES;
}


etID_STATE				etCommServe( etComm *etCommActual, etID_STATE (*newClientConnection)(etComm*) ){
// Object okay ?
	etCheckNull( etCommActual );

	if( etCommActual->serve != NULL ){
		return etCommActual->serve( etCommActual, newClientConnection );
	}

	return etID_NO;
}





