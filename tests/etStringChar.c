/* Apicheck Function for etString
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


#include "evillib_defines.h"
#include "evillib_depends.h"


#include "core/etDebug.h"
#include "memory/etMemoryBlock.h"
#include "memory/etMemory.h"
#include "string/etString.h"
#include "string/etStringChar.h"

#ifdef ET_SINGLEMODULE
    #include "string/etStringChar.c"
#endif

etID_STATE            etStringCharTest(){
    etDebugMessage( etID_LEVEL_DETAIL, "Start test.." );



// Vars
    etString    *etStringActual = NULL;
    const char    *etStringChar = NULL;

// Allocate a new etString
    etStringAlloc( etStringActual );

// Try to get an etString from an empty etString
    etStringCharGet( etStringActual, etStringChar );
    
// Set it with an char
    etStringCharSet( etStringActual, "An test String", -1 );
    etStringCharGet( etStringActual, etStringChar );
    if( strncmp( etStringChar, "An test String", 14 ) == 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds the String: '%s' this is good", etStringActual, (char*)etStringChar );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    } else {
        etDebugMessage( etID_LEVEL_CRITICAL, "String is incorrect" );
    }

// Copy an smaler string
    etStringCharSet( etStringActual, "An test String", 7 );
    etStringCharGet( etStringActual, etStringChar );
    if( strncmp( etStringChar, "An test", 7 ) == 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds the String: '%s' this is good", etStringActual, (char*)etStringChar );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    } else {
        etDebugMessage( etID_LEVEL_CRITICAL, "String is incorrect" );
    }



// Add something to the string
    etStringCharAdd( etStringActual, " for an longer String" );
    etStringCharGet( etStringActual, etStringChar );
    if( strncmp( etStringChar, "An test for an longer String", 28 ) == 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds the String: '%s' this is good", etStringActual, (char*)etStringChar );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    } else {
        etDebugMessage( etID_LEVEL_CRITICAL, "String is incorrect" );
    }

// Find something inside the string
    int foundPosition = etStringCharFind( etStringActual, "longer", 0 );
    if( foundPosition != 15 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "Found the search string 'longer' not instide the string" );
    }

// End Timer
    etDebugMessage( etID_LEVEL_DETAIL, "Test finished" );
    return etID_YES;
}

