/*  Copyright (C) 2014-2016 by Martin Langlotz alias stackshadow

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

/**@defgroup etDebug Debugging
@ingroup grCore

Debuging inside the evillib is simple and can be used by other programs.
Normally you need to use etDebugState() or etDebugMessage() in your program.

@def ET_INTERNAL
This is not needed in your userspace program. Its only to signalize the evillib
to compile the internal Debugging functions

@def ET_DEBUG_OFF
Disable debugging \n
This disable the debugging function during compile time, which results in a smaller library.

*/



#include "core/etDebug.h"



etDebug                 etDebugEvillib[1] = {
    {
        .Sync = PTHREAD_MUTEX_INITIALIZER,
    // Standart visible levels
        .LevelVisible = etID_LEVEL_ERR,

        .printMessage = etDebugPrintMessageDefault
    }
};
int                     etDebugTempMessageLen = 512;
char                    etDebugTempMessage[512];

// For snprintf()

void                    etDebugPrintMessageDefault( etDebug* etDebugActual ){

// show message if level is active and not critical
//    if( etDebugEvillib->Level < etDebugEvillib->LevelVisible && etDebugEvillib->Level != etID_LEVEL_CRITICAL ) return;


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
    if( etDebugActual->FunctionOrigin != NULL ){
        fprintf( OutputStream, " [%s", etDebugActual->FunctionOrigin );
        if( etDebugActual->FunctionOriginLine > 0 ){
            fprintf( OutputStream, ": %i", etDebugActual->FunctionOriginLine );
        }
        fprintf( OutputStream, "]->" );
    }

    // Function and line
    fprintf( OutputStream, "[%s", etDebugActual->Function );
    fprintf( OutputStream, ": %i]", etDebugActual->FunctionLine );


    // Message
    if( etDebugActual->Message != NULL ){
        fprintf( OutputStream, " [%s]\n", etDebugActual->Message );
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


void                    etDebugPrintMessage(){
    if( etDebugEvillib->printMessage != NULL ){
        etDebugEvillib->printMessage( etDebugEvillib );
    }
}




// ################ state stuff ################

etID_STATE              etDebugStateToMessage( etID_STATE state, const char *function, int line ){


// Set info
    etDebugEvillib->Function = function;
    etDebugEvillib->FunctionLine = line;

// set level
    if( state > 0 && state < etID_STATE_WARN ){
        etDebugEvillib->Level = etID_LEVEL_INFO;
    }
    if( state > etID_STATE_WARN && state < etID_STATE_ERR ){
        etDebugEvillib->Level = etID_LEVEL_WARNING;
    }
    if( state > etID_STATE_ERR && state < etID_STATE_CRIT ){
        etDebugEvillib->Level = etID_LEVEL_ERR;
    }
    if( state > etID_STATE_CRIT ){
        etDebugEvillib->Level = etID_LEVEL_CRITICAL;
    }

// Calculate Level
    switch( state ){

    // Special things
        case etID_STATE_ERR_PARAMETER:
            etDebugEvillib->Message = "Parameter missuse";
            break;

        case etID_STATE_CRIT_NOMEMORY:
            etDebugEvillib->Message = "No More System Memory";
            break;

        case etID_STATE_WARN_TIMEOUT:
            etDebugEvillib->Message = "Timeout";
            break;

        case etID_STATE_USED:
            etDebugEvillib->Message = "Already in use";
            break;

        case etID_STATE_WARN_SEQERR:
            etDebugEvillib->Message = "Sequencial Error, you need to run another function before this function. Read the documentation about this function.";
            break;

        default:
            etDebugEvillib->Message = "";
    }

    // Printout the error


    // If critical, exit
    if( etDebugEvillib->Level == etID_LEVEL_CRITICAL ){
        exit( EXIT_FAILURE );
    }

    return state;
}

/** @ingroup etDebug
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@fn etID_STATE etDebugState( etID_STATE state )
@~english
@brief This function maps an etID_STATE to an readable message and print it to stdout

@param[in] state Just an etID_STATE
@return The state just returned
*/
etID_STATE              etDebugStateExtern( etID_STATE state, const char *function, int line ){

// only print message over an amount of visible level
    if( etDebugEvillib->Level < etDebugEvillib->LevelVisible ) return state;


    etDebugStateToMessage( state, function, line );
    etDebugPrintMessage();

    return state;
}


etID_STATE              etDebugStateIntern( etID_STATE state, const char *function, int line ){

// only print message over an amount of visible level
    if( etDebugEvillib->Level < etDebugEvillib->LevelVisible ) return state;


    const char *programName = etDebugEvillib->Program;

    etDebugEvillib->Program = "evillib";
    etDebugStateToMessage( state, function, line );
    etDebugPrintMessage();
    etDebugEvillib->Program = programName;

    return state;
}



// ################ message stuff ################
/** @ingroup etDebug
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@fn void etDebugMessage( etID_LEVEL messageLevel, const char *message );
@~english
@brief This function prints out an message.

If the messageLevel is etID_LEVEL_CRITICAL the application exits !

@param[in] messageLevel The level
@param[in] message The message as const char
@return The state just returned
*/

void                    etDebugMessageExtern( etID_LEVEL messageLevel, const char *function, int line, const char *message ){

    // Fill struct
    etDebugEvillib->Level = messageLevel;

    etDebugEvillib->Function = function;
    etDebugEvillib->FunctionLine = line;
    etDebugEvillib->Message = message;

    // Printout the error
    etDebugPrintMessage();

    // If critical, exit
    if( etDebugEvillib->Level == etID_LEVEL_CRITICAL ){
        exit( EXIT_FAILURE );
    }
}


void                    etDebugMessageIntern( etID_LEVEL messageLevel, const char *function, int line, const char *message ){

    const char *ProgramName = etDebugEvillib->Program;

    etDebugEvillib->Program = "evillib";
    etDebugMessageExtern( messageLevel, function, line, message );
    etDebugEvillib->Program = ProgramName;

}



// ################ settings ################

etID_STATE              etDebugProgramNameSet( const char *programName ){
    etDebugEvillib->Program = programName;
    return etID_YES;
}


etID_STATE              etDebugLevelSet( etID_LEVEL debugLevels ){
    etDebugEvillib->LevelVisible = debugLevels;

    return etID_YES;
}





