/* etObject - The base class for all complex objects
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

#ifndef _H_evillib_etObject
#define _H_evillib_etObject

typedef struct etObject_t {
	etID_STATE		state;
} etObject;



#define etObjectAlloc( objectName, objectVariable ) \
	etMemoryBlockNew = NULL; \
	etMemoryRequest( &etMemoryBlockNew, sizeof(objectName) ); \
	objectVariable = (objectName*)etMemoryBlockDataGet( etMemoryBlockNew ); \

#define etCheckNull( objectVariable ) \
	if( objectVariable == NULL ){ \
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "objectVariable is null" ); \
		etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage ); \
		return etID_STATE_PARAMETER_MISSUSE; \
	}

#define etCheckNullVoid( objectVariable ) \
	if( objectVariable == NULL ){ \
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "objectVariable is null" ); \
		etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage ); \
		return; \
	}

#define etObjectIsOkay( objectVariable ) \
	objectVariable->state != etID_OK

#define etObjectSetOkay( objectVariable ) \
	objectVariable->state = etID_OK

#define etObjectCheck( objectVariable ) \
	if( etObjectIsOkay(objectVariable) ){ \
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "Object objectVariable is not OK and can not be used" ); \
		etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage ); \
		return objectVariable->state; \
	}


#define etObjectSetState( objectVariable, newState ) \
	if( objectVariable->state == etID_OK ){ \
		objectVariable->state = newState; \
	}


#endif

