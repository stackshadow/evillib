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




etDebug 			etDebugEvillib[1] = {
	{
		.Sync = PTHREAD_MUTEX_INITIALIZER,
	// Standart visible levels
		.LevelVisible = etID_LEVEL_ERR
	}
};
int						etDebugTempMessageLen = 256;
char 					etDebugTempMessage[256];


// For snprintf()


struct mallinfo		etDebugPrintMallocInfo_( const char *function, const char *optionalText ){
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

	if( function != NULL ) printf("%s ", function);
	if( optionalText != NULL ) printf("%s ", optionalText);


	printf( "Arena: %i ", ActualMallocInfo.arena );
	printf( "AllocatedSpace: %i ", ActualMallocInfo.uordblks );
	printf( "FreeSpace: %i ", ActualMallocInfo.fordblks );
	printf( "\n\n" );

	return ActualMallocInfo;

#endif
}


etID_STATE			_etDebugStateExtern( etID_STATE state, const char *function, int line ){

// Set default level
	etDebugEvillib->Level = etID_LEVEL_ERR;

// Set info
	etDebugEvillib->Function = function;
	etDebugEvillib->FunctionLine = line;

// Calculate Level
	switch( state ){

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
	
		case etID_STATE_ERROR_INTERNAL:
			etDebugEvillib->Level = etID_LEVEL_WARNING;
			etDebugEvillib->Message = "Internal Error";
			break;

		case etID_STATE_NOTINLIB:
			etDebugEvillib->Level = etID_LEVEL_WARNING;
			etDebugEvillib->Message = "Function not aviable in the library";
			break;
			
		case etID_STATE_SEQERR:
			etDebugEvillib->Level = etID_LEVEL_ERR;
			etDebugEvillib->Message = "Sequencial Error, you need to run another function before this function. Read the documentation about this function.";
			break;

		default:
			etDebugEvillib->Level = etID_LEVEL_ERR;
			etDebugEvillib->Message = NULL;
	}

// Printout the error


// If critical, exit
	if( etDebugEvillib->Level == etID_LEVEL_CRITICAL ){
		exit( EXIT_FAILURE );
	}

	return state;
}


etID_STATE			_etDebugStateIntern( etID_STATE state, const char *function, int line ){

	const char *programName = etDebugEvillib->Program;

	etDebugEvillib->Program = "evillib";
	int ReturnState = _etDebugStateExtern( state, function, line );
	etDebugEvillib->Program = programName;

	return ReturnState;
}





void				_etDebugMessageExtern( etID_LEVEL messageLevel, const char *function, int line, const char *message ){

// Fill struct
	etDebugEvillib->Level = messageLevel;

	etDebugEvillib->Function = function;
	etDebugEvillib->FunctionLine = line;
	etDebugEvillib->Message = message;

// Printout the error

	
// If critical, exit
	if( etDebugEvillib->Level == etID_LEVEL_CRITICAL ){
		exit( EXIT_FAILURE );
	}
}


void				_etDebugMessageIntern( etID_LEVEL messageLevel, const char *function, int line, const char *message ){

	const char *ProgramName = etDebugEvillib->Program;

	etDebugEvillib->Program = "evillib";
	_etDebugMessageExtern( messageLevel, function, line, message );
	etDebugEvillib->Program = ProgramName;

}


etID_STATE			etDebugStateGetLast(){
	return etDebugEvillib->Level;
}


etID_STATE			etDebugProgramNameSet( const char *programName ){
	etDebugEvillib->Program = programName;
	return etID_YES;
}


etID_STATE			etDebugLevelSet( etID_LEVEL debugLevels ){
	etDebugEvillib->LevelVisible = debugLevels;
	return etID_YES;
}






/**
@endcond 
*/



// ######################### NEW DEBUGGING SYSTEM

void                    etDebugDefaultPrintMessage( etDebug* etDebugActual );


etID_STATE              etDebugInit( etDebug* etDebugActual ){

// zero the struct
    memset( etDebugActual, 0, sizeof(etDebug) );

// set the default visible level
    //etDebugActual->LevelVisible = etID_LEVEL_INFO;
    etDebugActual->LevelVisible = etID_LEVEL_ALL;
    
// setup default message function
    etDebugActual->printMessage = etDebugDefaultPrintMessage;
    return etID_YES;
}


etDebug*                etDebugSetCaller( etDebug* etDebugActual, const char *function, int line ){
    etDebugActual->FunctionOriginLine = line;
    etDebugActual->FunctionOrigin = function;
    return etDebugActual;
}



etID_STATE              etDebugMapStateToMessage( etDebug* etDebugActual ){

// Calculate Level
	switch( etDebugActual->state ){
            
        case etID_STATE_NODATA:
			etDebugActual->Level = etID_LEVEL_WARNING;
			etDebugActual->Message = "No data present";
			break;

	// Special things
		case etID_STATE_PARAMETER_MISSUSE:
			etDebugActual->Level = etID_LEVEL_ERR;
			etDebugActual->Message = "Parameter missuse";
			break;

		case etID_STATE_NOMEMORY:
			etDebugActual->Level = etID_LEVEL_CRITICAL;
			etDebugActual->Message = "No More System Memory";
			break;

		case etID_STATE_TIMEOUT:
			etDebugActual->Level = etID_LEVEL_WARNING;
			etDebugActual->Message = "Timeout";
			break;

		case etID_STATE_USED:
			etDebugActual->Level = etID_LEVEL_ERR;
			etDebugActual->Message = "Already in use";
			break;
	
		case etID_STATE_ERROR_INTERNAL:
			etDebugActual->Level = etID_LEVEL_WARNING;
			etDebugActual->Message = "Internal Error";
			break;

		case etID_STATE_NOTINLIB:
			etDebugActual->Level = etID_LEVEL_WARNING;
			etDebugActual->Message = "Function not aviable in the library";
			break;
			
		case etID_STATE_SEQERR:
			etDebugActual->Level = etID_LEVEL_ERR;
			etDebugActual->Message = "Sequencial Error, you need to run another function before this function. Read the documentation about this function.";
			break;

		default:
            if( etDebugActual->state > 0 ){
                etDebugActual->Level = etID_LEVEL_DETAIL;
            
            } else {
                etDebugActual->Level = etID_LEVEL_ERR;
            }
            snprintf( etDebugTempMessage, etDebugTempMessageLen, "etID_STATE: %i", etDebugActual->state );
			etDebugActual->Message = etDebugTempMessage;
	}


	return etDebugActual->state;
}


etID_STATE              __etDebugStateSet( etDebug* etDebugActual, etID_STATE state, const char *function, int line ){

// save infos
    etDebugActual->state = state;
    etDebugActual->Function = function;
    etDebugActual->FunctionLine = line;

// map state to messate
    etDebugMapStateToMessage( etDebugActual );

    return etDebugPrintMessage(etDebugActual);
}


etID_STATE              __etDebugReset( etDebug* etDebugActual, const char *function, int line ){
    etDebugActual->FunctionOriginLine = line;
    etDebugActual->FunctionOrigin = function;

    etDebugActual->state = etID_YES;
    etDebugActual->Level = etID_LEVEL_DETAIL;
    etDebugActual->Message = "Reset debugging state";
    
    etDebugPrintMessage( etDebugActual );

    etDebugActual->state = etID_STATE_NOTHING;
    etDebugActual->Message = NULL;
    
    return etID_YES;
}


void                    etDebugDefaultPrintMessage( etDebug* etDebugActual ){

// show message if level is active and not critical
	if( etDebugActual->Level < etDebugActual->LevelVisible && etDebugActual->Level != etID_LEVEL_CRITICAL ) return;
    if( etDebugActual->state == etID_STATE_NOTHING ) return;


	pthread_mutex_lock( &etDebugActual->Sync );

// OutputStream
	_IO_FILE *OutputStream = stderr;
	switch( etDebugActual->Level ){
		case etID_LEVEL_ERR:
				OutputStream = stderr; break;
		default:
				OutputStream = stdout;
	}

// Program
	if( etDebugActual->Program != NULL ){
		fprintf( OutputStream, "[%s] ", etDebugActual->Program );
	}

// Level of message
	switch( etDebugActual->Level ){

		case etID_LEVEL_DETAIL:
			fprintf( OutputStream, "\e[0;36m[DEBUG]\033[00m [DETAIL]" );
			break;

		case etID_LEVEL_DETAIL_MEM:
			fprintf( OutputStream, "\033[0;36m[DEBUG]\033[00m [MEMORY]" );
			break;

		case etID_LEVEL_DETAIL_EVENT:
			fprintf( OutputStream, "\033[0;46m[DEBUG]\033[00m [EVENT]" );
			break;

		case etID_LEVEL_DETAIL_PROCESS:
			fprintf( OutputStream, "\033[0;47m[DEBUG]\033[00m [PROCESS]" );
			break;

		case etID_LEVEL_DETAIL_NET:
			fprintf( OutputStream, "\033[0;48m[DEBUG]\033[00m [NET]" );
			break;
			
		case etID_LEVEL_DETAIL_THREAD:
			fprintf( OutputStream, "\033[0;48m[DEBUG]\033[00m [THREAD]" );
			break;
			
		case etID_LEVEL_DETAIL_DB:
			fprintf( OutputStream, "\033[0;48m[DEBUG]\033[00m [DB]" );
			break;
            
        case etID_LEVEL_TEST:
            fprintf( OutputStream, "\033[0;48m[DEBUG]\033[00m [TEST]" );
			break;

		case etID_LEVEL_INFO:
			fprintf( OutputStream, "[INFO]" );
			break;

		case etID_LEVEL_WARNING:
			fprintf( OutputStream, "\033[1;43m[WARNING]\033[00m" );
			break;

		case etID_LEVEL_ERR:
			fprintf( OutputStream, "\033[0;41m[ERROR]\033[00m" );
			break;

		case etID_LEVEL_CRITICAL:
			fprintf( OutputStream, "[CRITICAL]" );
			break;
	// Default is Error
		default:
			fprintf( OutputStream, "[ERROR]" );
	}

// caller function
	fprintf( OutputStream, " [%s", etDebugActual->FunctionOrigin );
	fprintf( OutputStream, ": %i]->", etDebugActual->FunctionOriginLine );

// Function and line
	fprintf( OutputStream, "[%s", etDebugActual->Function );
	fprintf( OutputStream, ": %i]", etDebugActual->FunctionLine );


// Message
	if( etDebugActual->Message != NULL ){
		fprintf( OutputStream, " %s\n", etDebugActual->Message );
	} else {
		fprintf( OutputStream, " " );
	}


// Flush
	fflush( OutputStream );




	pthread_mutex_unlock( &etDebugActual->Sync );


// If critical, exit
	if( etDebugActual->Level == etID_LEVEL_CRITICAL ){
		exit( EXIT_FAILURE );
	}

}


etID_STATE              etDebugPrintMessage( etDebug* etDebugActual ){
    if( etDebugActual->printMessage == NULL ) return etDebugActual->state;
    
    etDebugActual->printMessage( etDebugActual );
    return etDebugActual->state;
}


etID_STATE              etDebugPrintCustomMessage_intern( etDebug* etDebugActual, etID_LEVEL messageLevel, const char* message, const char *function, int line ){

// the actual program name
	const char *ProgramName = etDebugActual->Program;

// but we are inside the evillib ;)
	etDebugActual->Program = "evillib";

// print message
    etDebugActual->state = etID_YES;
    etDebugActual->Level = messageLevel;
    etDebugActual->Message = message;
    etDebugActual->Function = function;
    etDebugActual->FunctionLine = line;
	etDebugPrintMessage( etDebugActual );

// set the original program name back
	etDebugActual->Program = ProgramName;
    
    return etID_YES;
}




