/* etDebug - The evillib debugging subsystem
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

#ifndef _H_evillib_etDebug
#define _H_evillib_etDebug


/** Group
@cond DEV
@ingroup etDebug
*/
typedef struct etDebug_s etDebug;
struct etDebug_s {
    pthread_mutex_t     Sync;

    const char*         Program;                /*!< Program-name ( normaly evillib ) */

    etID_STATE          state;
    etID_LEVEL          Level;                  /*!< Warning, debug etc */
    etID_LEVEL          LevelVisible;

    const char*         FunctionOrigin;
    int                 FunctionOriginLine;
    const char*         Function;               /*!< calling Function */
    int                 FunctionLine;           /*!< Line of calling function */
    const char*         Message;                /*!< Message */
    
    void                (*printMessage)( etDebug* etDebugActual );
};

extern etDebug 			etDebugEvillib[1];
extern int				etDebugTempMessageLen;
extern char 			etDebugTempMessage[256];


#ifdef __WIN32__
	struct mallinfo mallinfo();

	struct mallinfo {
		int uordblks;
	};
#endif


/** @ingroup etDebug
	@def ET_INTERNAL
	This is not needed in your userspace program. Its only to signalize the evillib
	to compile the internal Debugging functions
*/
#ifdef ET_INTERNAL
	#define _etDebugState _etDebugStateIntern
#else
	#define _etDebugState _etDebugStateExtern
#endif

#ifdef ET_INTERNAL
	#define _etDebugMessage _etDebugMessageExtern
#else
	#define _etDebugMessage _etDebugMessageIntern
#endif




#if __GNUC__ >= 2
	#define __ET_DEBUG_FUNCTION __FUNCTION__
#else
	#define __ET_DEBUG_FUNCTION "unknow"
#endif

/** @ingroup etDebug
@def ET_DEBUG_OFF
Disable debugging \n
This disable the debugging function during compile time, which results in a smaller library.
*/
/** @ingroup etDebug
@def etDebugState( messageState )
@brief Set an state for debugging
The etID_STATE will be transform to an message/level. 
If the etID_STATE is not mentioned from this function the level will be set to etID_LEVEL_ERR
@param messageState The etID_STATE from which the Level and the Message is set
*/
/** @ingroup etDebug
@def etDebugMessage( messageLevel, Message )
@brief Set a debug-message with an level
*/
#ifndef ET_DEBUG_OFF
	// debug stuff
	#define etDebugState( messageState ) (_etDebugState( messageState, __ET_DEBUG_FUNCTION, __LINE__ ))
	#define etDebugMessage( messageLevel, Message ) (_etDebugMessage( messageLevel, __ET_DEBUG_FUNCTION, __LINE__, Message ))
#else
	#define etDebugState( messageState ) messageState
	#define etDebugMessage( messageLevel, Message ) 
#endif



void				_etDebugPrintMessage();



etID_STATE			_etDebugStateExtern( etID_STATE state, const char *function, int line );


etID_STATE			_etDebugStateIntern( etID_STATE state, const char *function, int line );



void				_etDebugMessageExtern( etID_LEVEL messageLevel, const char *function, int line, const char *message );


void				_etDebugMessageIntern( etID_LEVEL messageLevel, const char *function, int line, const char *message );


etID_STATE			etDebugStateGetLast();


etID_STATE			etDebugProgramNameSet( const char *programName );


etID_STATE			etDebugLevelSet( etID_LEVEL debugLevels );


etDebug*            __etDebugSetCaller( etDebug* etDebugActual, const char *function, int line );









// ######################### NEW DEBUGGING SYSTEM



etID_STATE              etDebugInit( etDebug* etDebugActual );


etID_STATE              etDebugMapStateToMessage( etDebug* etDebugActual );


etID_STATE              etDebugPrintMessage( etDebug* etDebugActual );

// caller ( inside the function who calls it
etDebug*                etDebugSetCaller( etDebug* etDebugActual, const char *function, int line );
#define __etDebugSetCaller( etDebugActual, function, line ) ( etDebugSetCaller(etDebugActual, function, line) )
#define caller( etDebugActual ) ( __etDebugSetCaller(etDebugActual, __ET_DEBUG_FUNCTION, __LINE__) )


#define                 etDebugStateSet( etDebugActual, state ) __etDebugStateSet( etDebugActual, state, __ET_DEBUG_FUNCTION, __LINE__ )
etID_STATE              __etDebugStateSet( etDebug* etDebugActual, etID_STATE state, const char *function, int line );

#define                 etDebugReset( etDebugActual ) __etDebugReset( etDebugActual, __ET_DEBUG_FUNCTION, __LINE__ )
etID_STATE              __etDebugReset( etDebug* etDebugActual, const char *function, int line );

// check functions
#define                 etDebugReturnOnError( etDebugActual ) \
                        if( (etDebugActual)->state < etID_STATE_NOTHING ){ \
                            return etDebugActual->state; \
                        }

#define                 etDebugCheckNull( etDebugActual, objectVariable) \
                        if( objectVariable == NULL ){ \
                            snprintf( etDebugTempMessage, etDebugTempMessageLen, "%s is null", "objectVariable" ); \
                            etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage ); \
                            etDebugStateSet( etDebugActual, etID_STATE_PARAMETER_MISSUSE ); \
                            return etDebugActual->state; \
                        }

#define                 etDebugCheckMember( etDebugActual, structMember ) \
                        if( structMember == NULL ){ \
                            snprintf( etDebugTempMessage, etDebugTempMessageLen, "Member variable %s is null", "structMember" ); \
                            etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage ); \
                            etDebugStateSet( etDebugActual, etID_STATE_PARAMETER_MISSUSE ); \
                            return etDebugActual->state; \
                        }



// custom messaging
#ifdef ET_INTERNAL
    etID_STATE etDebugPrintCustomMessage_intern( etDebug* etDebugActual, etID_LEVEL messageLevel, const char* message, const char *function, int line );
	#define _etDebugPrintCustomMessage etDebugPrintCustomMessage_intern
#else
	#define _etDebugPrintCustomMessage etDebugPrintCustomMessage_intern
#endif
#ifndef ET_DEBUG_OFF
	// debug stuff
	#define etDebugPrintCustomMessage( etDebugActual, messageLevel, message ) (_etDebugPrintCustomMessage( etDebugActual, messageLevel, message, __ET_DEBUG_FUNCTION, __LINE__ ))
#else
	#define etDebugPrintCustomMessage( etDebugActual, messageLevel, message ) 
#endif




#endif


/**
@endcond 
*/





