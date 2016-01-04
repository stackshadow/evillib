/*  Copyright (C) 2015 by Martin Langlotz alias stackshadow

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

#ifndef _H_etObject
#define _H_etObject

#include "core/etIDState.h"
#include "memory/etMemoryBlock.h"
#include "memory/etMemory.h"





typedef struct etObject_s {
    etID_STATE        state;
} etObject_t;

#define etObject etObject_t object

#define     etObjectAlloc( objectName, objectVariable ) \
                etMemoryRequest( objectVariable, sizeof(objectName) ); \

#define     etObjectFree( objectVariable ) \
                etMemoryBlock *etMemoryBlockActual = NULL; \
                etMemoryBlockGet( objectVariable, etMemoryBlockActual ); \
                etMemoryBlockRelease( etMemoryBlockActual, etID_TRUE )

#define     etObjectCheckSetter( objectVariable ) \
                if( objectVariable == NULL ){ \
                    return etID_STATE_ERR_PARAMETER; \
                } \
                if( (objectVariable)->object.state > etID_STATE_ERR ){ \
                    return (objectVariable)->object.state; \
                }

#define     etObjectCheckGetter( objectVariable ) \
                if( objectVariable == NULL ){ \
                    return etID_STATE_ERR_PARAMETER; \
                } \
                if( (objectVariable)->object.state > etID_STATE_WARN ){ \
                    return (objectVariable)->object.state; \
                }

#define     etObjectCheckNull( objectVariable, variable ) \
                if( variable == NULL ){ \
                    (objectVariable)->object.state = etID_STATE_WARN_PARAMETER; \
                    return (objectVariable)->object.state; \
                }

#define     etObjectStateSet( objectVariable, newState ) \
                (objectVariable)->object.state = newState; \
                etDebugStateToMessage( newState, __ET_DEBUG_FUNCTION, __LINE__ ); \
                snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set " #objectVariable " to state %i", newState ); \
                if( newState < 0 ){ \
                    etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage ); \
                } \
                if( newState >= 0 ){ \
                    etDebugMessage( etID_LEVEL_INFO, etDebugTempMessage ); \
                }


#define     etObjectStateSetAndReturn( objectVariable, newState ) \
                (objectVariable)->object.state = newState; \
                etDebugStateToMessage( newState, __ET_DEBUG_FUNCTION, __LINE__ ); \
                snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set " #objectVariable " to state %i", newState ); \
                if( newState < 0 ){ \
                    etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage ); \
                } \
                if( newState >= 0 ){ \
                    etDebugMessage( etID_LEVEL_INFO, etDebugTempMessage ); \
                } \
                return (objectVariable)->object.state;



#endif

