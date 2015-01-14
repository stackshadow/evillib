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

/** @cond DEV */
/**@defgroup etDebug Debugging
@ingroup grCore

Debuging inside the evillib is simple and can be used by other programs.
*/
 
#ifdef ET_SINGLEMODULE
	#include "evillib_defines.h"
	#include "evillib_depends.h"

	#include "core/etDebug.h"
#endif




struct etDebug 			etDebugEvillib[1] = {
	{
		.Sync = PTHREAD_MUTEX_INITIALIZER,
	// Standart visible levels
		.LevelVisible = etID_LEVEL_CRITICAL
	}
};
int						etDebugTempMessageLen = 256;
char 					etDebugTempMessage[256];
void 					(*etDebugPrintMessage)() = _etDebugPrintMessage;
// For snprintf()


struct mallinfo		etDebugPrintMallocInfo_( const char *Function, const char *OptionalText ){
#ifdef __unix__
/*
  int arena;    non-mmapped space allocated from system
  int ordblks;  number of free chunks
  int smblks;   number of fastbin blocks

  int hblks;    number of mmapped regions
  int hblkhd;   space in mmapped regions
  int usmblks;  maximum total allocated space

  int fsmblks;  space available in freed fastbin blocks
  int uordblks; total allocated space
  int fordblks; total free space

  int keepcost; top-most, releasable (via malloc_trim) space
*/


	struct mallinfo ActualMallocInfo = mallinfo();

	if( Function != NULL ) printf("%s ", Function);
	if( OptionalText != NULL ) printf("%s ", OptionalText);


	printf( "Arena: %i ", ActualMallocInfo.arena );
	printf( "AllocatedSpace: %i ", ActualMallocInfo.uordblks );
	printf( "FreeSpace: %i ", ActualMallocInfo.fordblks );
	printf( "\n\n" );

	return ActualMallocInfo;

#endif
}


void				_etDebugPrintMessage(){

// Return function if level is not visible
	if( etDebugEvillib->Level < etDebugEvillib->LevelVisible && etDebugEvillib->Level != etID_LEVEL_CRITICAL ) return;


	pthread_mutex_lock( &etDebugEvillib->Sync );

// OutputStream
	_IO_FILE *OutputStream = stderr;
	switch( etDebugEvillib->Level ){
		case etID_LEVEL_ERR:
				OutputStream = stderr; break;
		default:
				OutputStream = stdout;
	}

// Program
	if( etDebugEvillib->Program != NULL ){
		fprintf( OutputStream, "[%s] ", etDebugEvillib->Program );
	}

// Level of message
	switch( etDebugEvillib->Level ){

		case etID_LEVEL_DETAIL:
			fprintf( OutputStream, "\e[0;36m[DEBUG] [TEST]" );
			break;

		case etID_LEVEL_DETAIL_MEM:
			fprintf( OutputStream, "\033[0;36m[DEBUG] [MEMORY]" );
			break;

		case etID_LEVEL_DETAIL_EVENT:
			fprintf( OutputStream, "\033[0;46m[DEBUG] [EVENT]" );
			break;

		case etID_LEVEL_DETAIL_PROCESS:
			fprintf( OutputStream, "\033[0;47m[DEBUG] [PROCESS]" );
			break;

		case etID_LEVEL_INFO:
			fprintf( OutputStream, "[INFO]" );
			break;

		case etID_LEVEL_WARNING:
			fprintf( OutputStream, "\033[1;43m[WARNING]" );
			break;

		case etID_LEVEL_ERR:
			fprintf( OutputStream, "\033[0;41m[ERROR]" );
			break;

		case etID_LEVEL_CRITICAL:
			fprintf( OutputStream, "[CRITICAL]" );
			break;
	// Default is Error
		default:
			fprintf( OutputStream, "[ERROR]" );
	}

// Function and line
	fprintf( OutputStream, " [%s", etDebugEvillib->Function );
	fprintf( OutputStream, ": %i]", etDebugEvillib->FunctionLine );


// Message
	if( etDebugEvillib->Message != NULL ){
		fprintf( OutputStream, "\033[00m %s\n", etDebugEvillib->Message );
	} else {
		fprintf( OutputStream, " " );
	}


// Flush
	fflush( OutputStream );




	pthread_mutex_unlock( &etDebugEvillib->Sync );
	return;
}



etID_STATE			_etDebugStateExtern( etID_STATE State, const char *Function, int Line ){

// Set default level
	etDebugEvillib->Level = etID_LEVEL_ERR;

// Set info
	etDebugEvillib->Function = Function;
	etDebugEvillib->FunctionLine = Line;

// Calculate Level
	switch( State ){

	// Special things
		case etID_STATE_PARAMETER_MISSUSE:
			etDebugEvillib->Level = etID_LEVEL_ERR;
			etDebugEvillib->Message = "Parameter missuse";
			break;

		case etID_STATE_NOMEMORY:
			etDebugEvillib->Level = etID_LEVEL_CRITICAL;
			etDebugEvillib->Message = "No More System Memory";
			break;

		case etID_STATE_TIMEOUT:
			etDebugEvillib->Level = etID_LEVEL_WARNING;
			etDebugEvillib->Message = "Timeout";
			break;

		case etID_STATE_USED:
			etDebugEvillib->Level = etID_LEVEL_ERR;
			etDebugEvillib->Message = "Already in use";
			break;

		case etID_STATE_NOTINLIB:
			etDebugEvillib->Level = etID_LEVEL_WARNING;
			etDebugEvillib->Message = "Function not aviable in the library";
			break;

		default:
			etDebugEvillib->Level = etID_LEVEL_ERR;
			etDebugEvillib->Message = NULL;
	}

// Printout the error
	etDebugPrintMessage();

// If critical, exit
	if( etDebugEvillib->Level == etID_LEVEL_CRITICAL ){
		exit( EXIT_FAILURE );
	}

	return State;
}


etID_STATE			_etDebugStateIntern( etID_STATE State, const char *Function, int Line ){

	const char *ProgramName = etDebugEvillib->Program;

	etDebugEvillib->Program = "evillib";
	int ReturnState = _etDebugStateExtern( State, Function, Line );
	etDebugEvillib->Program = ProgramName;

	return ReturnState;
}





void				_etDebugMessageExtern( etID_LEVEL MessageLevel, const char *Function, int Line, const char *Message ){

// Fill struct
	etDebugEvillib->Level = MessageLevel;

	etDebugEvillib->Function = Function;
	etDebugEvillib->FunctionLine = Line;
	etDebugEvillib->Message = Message;

// Printout the error
	etDebugPrintMessage();
	
// If critical, exit
	if( etDebugEvillib->Level == etID_LEVEL_CRITICAL ){
		exit( EXIT_FAILURE );
	}
}


void				_etDebugMessageIntern( etID_LEVEL MessageLevel, const char *Function, int Line, const char *Message ){

	const char *ProgramName = etDebugEvillib->Program;

	etDebugEvillib->Program = "evillib";
	_etDebugMessageExtern( MessageLevel, Function, Line, Message );
	etDebugEvillib->Program = ProgramName;

}


etID_STATE			etDebugStateGetLast(){
	return etDebugEvillib->Level;
}


etID_STATE			etDebug_ProgramName_set( const char *ProgramName ){
	etDebugEvillib->Program = ProgramName;
	return etID_YES;
}


etID_STATE			etDebug_Level_set( etID_LEVEL DebugLevels ){
	etDebugEvillib->LevelVisible = DebugLevels;
	return etID_YES;
}



/**
@endcond 
*/
