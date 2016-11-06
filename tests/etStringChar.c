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


#include "evillib_defines.h"
#include "evillib_depends.h"

#include "core/etObject.h"

#include "core/etDebug.h"
#include "core/etDebug.c"

#include "memory/etMemoryBlock.h"
#include "memory/etMemoryBlock.c"
#include "memory/etMemoryBlockList.h"
#include "memory/etMemoryBlockList.c"
#include "memory/etMemory.h"
#include "memory/etMemory.c"

#include "core/etVersion.h"
#include "core/etVersion.c"
#include "core/etInit.h"
#include "core/etInit.c"

#include "app/etApicheck.h"
#include "app/etApicheck.c"

#include "string/etString.h"
#include "string/etString.c"
#include "string/etStringChar.h"
#include "string/etStringChar.c"



etID_STATE              etStringCharTest(){
    etApicheckTimer( "etStringChar: Start test.." );



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



// copy to an char array
//! [etStringCharCopy]
    // define an tempArray
    char tempArray[100];

    // copy from etString to tempArray
    etStringCharCopy( etStringActual, tempArray, 100 );

    // compare if this is correct
    if( strncmp( tempArray, "An test for an longer String", 28 ) == 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds the String: '%s' this is good", etStringActual, (char*)tempArray );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    } else {
        etDebugMessage( etID_LEVEL_CRITICAL, "String is incorrect" );
    }
//! [etStringCharCopy]

// Find something inside the string
    int foundPosition = etStringCharFind( etStringActual, "longer", 0 );
    if( foundPosition != 15 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "Found the search string 'longer' not instide the string" );
    }



    etStringClean( etStringActual );
    etStringCharAdd( etStringActual, " )" );
    etStringCharGet( etStringActual, etStringChar );



// End Timer
    etApicheckTimer( "OK" );
    return etID_YES;
}

etID_STATE              etStringWCharTest(){
    etApicheckTimer( "etStringChar: Start test.." );


// End Timer
    etApicheckTimer( "OK" );
    return etID_YES;
}


int                     main( int argc, const char* argv[] ){
    etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_ALL );

    etStringCharTest();
    etStringWCharTest();
    
    
}

