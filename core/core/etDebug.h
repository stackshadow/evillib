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

#ifndef _H_evillib_etDebug
#define _H_evillib_etDebug


#include "evillib_depends.h"
#include "evillib_defines.h"

#include "core/etIDState.h"


typedef struct etDebug_s etDebug;
struct etDebug_s {
    pthread_mutex_t     Sync;

    const char*         Program;                /*!< Program-name ( normaly evillib ) */

    etID_LEVEL          Level;                  /*!< Warning, debug etc */
    etID_LEVEL          LevelVisible;

    const char*         FunctionOrigin;
    int                 FunctionOriginLine;
    const char*         Function;               /*!< calling Function */
    int                 FunctionLine;           /*!< Line of calling function */
    const char*         Message;                /*!< Message */
    
    void                (*printMessage)( etDebug* etDebugActual );
};

extern etDebug             etDebugEvillib[1];
extern int                etDebugTempMessageLen;
extern char             etDebugTempMessage[256];


#if __GNUC__ >= 2
    #define __ET_DEBUG_FUNCTION __FUNCTION__
#else
    #define __ET_DEBUG_FUNCTION "unknow"
#endif



// ################ state stuff ################

#ifndef ET_DEBUG_OFF

    #ifdef ET_INTERNAL
        #define _etDebugState etDebugStateIntern
    #else
        #define _etDebugState etDebugStateExtern
    #endif

    #define etDebugState( messageState ) (_etDebugState( messageState, __ET_DEBUG_FUNCTION, __LINE__ ))

#else
    #define etDebugState( messageState ) messageState
#endif

void                    etDebugPrintMessage();


etID_STATE              etDebugStateToMessage( etID_STATE state, const char *function, int line );
etID_STATE              etDebugStateExtern( etID_STATE state, const char *function, int line );
etID_STATE              etDebugStateIntern( etID_STATE state, const char *function, int line );


// ################ message stuff ################

#ifndef ET_DEBUG_OFF

    #ifdef ET_INTERNAL
        #define etDebugMessage( messageLevel, Message ) (etDebugMessageIntern( messageLevel, __ET_DEBUG_FUNCTION, __LINE__, Message ))
    #else
        #define etDebugMessage( messageLevel, Message ) (etDebugMessageExtern( messageLevel, __ET_DEBUG_FUNCTION, __LINE__, Message ))
    #endif

    

#else
    #define etDebugMessage( messageLevel, Message ) 
#endif

void                    etDebugMessageExtern( etID_LEVEL messageLevel, const char *function, int line, const char *message );
void                    etDebugMessageIntern( etID_LEVEL messageLevel, const char *function, int line, const char *message );



// ################ settings ################

etID_STATE              etDebugProgramNameSet( const char *programName );
etID_STATE              etDebugLevelSet( etID_LEVEL debugLevels );


#define                 etDebugCaller \
                            etDebugEvillib->FunctionOrigin = __ET_DEBUG_FUNCTION; \
                            etDebugEvillib->FunctionOriginLine = 0;


#define     etDebugCheckNull( objectVariable ) \
                if( objectVariable == NULL ){ \
                    snprintf( etDebugTempMessage, etDebugTempMessageLen, "objectVariable is null" ); \
                    etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage ); \
                    return  etID_STATE_ERR_PARAMETER; \
                }

#define     etDebugCheckNullVoid( objectVariable ) \
                if( objectVariable == NULL ){ \
                    snprintf( etDebugTempMessage, etDebugTempMessageLen, "objectVariable is null" ); \
                    etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage ); \
                    return; \
                }



#endif


/**
@endcond 
*/





