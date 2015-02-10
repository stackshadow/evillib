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
typedef struct etDebug {
	pthread_mutex_t		Sync;

	const char*			Program; 			/*!< Program-name ( normaly evillib ) */

	etID_LEVEL			Level;				/*!< Warning, debug etc */
	etID_LEVEL			LevelVisible;

	const char*			Function; 			/*!< calling Function */
	int					FunctionLine;		/*!< Line of calling function */
	const char*			Message; 			/*!< Message */
} etDebug;

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




























#endif


/**
@endcond 
*/