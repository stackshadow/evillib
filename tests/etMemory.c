/* Apicheck Function for etMemory
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

#include "core/etObject.h"
#include "core/etDebug.h"
#include "memory/etMemoryBlock.h"
#include "memory/etMemory.h"

#include "app/etApicheck.h"
#include "app/etApicheck.c"


// Vars
void                *etMemoryBlockActual = NULL;
char                *etMemoryBlockActualData = NULL;
void                *memoryData05 = NULL;
void                *memoryData08 = NULL;
void                *memoryData10 = NULL;
void                *memoryData20 = NULL;
void                *memoryData30 = NULL;
void                *memoryData40 = NULL;


etID_STATE            etMemoryTestNULL(){
    etApicheckTimer( "etMemory: Test NULL" );

// Run all functions with NULL-Pointer
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "check NULL" );
    etDebugMessage( etID_LEVEL_DETAIL, etDebugTempMessage );

    __etMemoryAlloc( NULL, 10 );
    __etMemoryRequest( NULL, 10 );
    __etMemoryRelease( NULL );
    __etMemorySet( NULL, NULL, 10 );
    __etMemorySetOffset( NULL, NULL, 10, 10 );
    etMemoryBlockGet( NULL, NULL );

    etApicheckTimer( "OK" );
    return etID_YES;
}

etID_STATE            etMemoryTestAlloc(){
    etApicheckTimer( "etMemory: Test alloc" );


// Prealloc Blocks
    etMemoryAlloc( memoryData05, 5 );
    etMemoryRequest( memoryData08, 8 );
    etMemoryRequest( memoryData10, 10 );
    etMemoryRequest( memoryData20, 20 );

// Dump
    etMemoryDump( NULL, NULL );

// Release two Blocks
    etMemoryBlockActual = memoryData10;
    etMemoryRelease( memoryData10 );
    etMemoryRelease( memoryData20 );
    etMemoryDump( NULL, NULL );


// now memoryData10 is enough to get 9 bytes but etMemoryBlock8 is to less
    etMemoryRequest( memoryData10, 9 );
    if( etMemoryBlockActual != memoryData10 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "We dont get the correct Block, etMemory dont work as expected !" );
        etDebugMessage( etID_LEVEL_CRITICAL, etDebugTempMessage );
    }

    etMemoryDump( NULL, NULL );

// cleanup
    etMemoryRelease( memoryData05 );
    etMemoryRelease( memoryData08 );
    etMemoryRelease( memoryData10 );
    etMemoryRelease( memoryData20 );

    etApicheckTimer( "OK" );
    return etID_YES;
}

etID_STATE            etMemoryTestData(){
    etApicheckTimer( "etMemory: Test Data" );

    etMemoryRequest( memoryData05, 5 );
    etMemoryRequest( memoryData08, 8 );
    etMemoryRequest( memoryData10, 10 );
    etMemoryRequest( memoryData20, 20 );
    
    const char *testString = memoryData10;

// We write a string to the block
    etMemorySet( memoryData10, (void*)"test1\0", 6 );
    testString = memoryData10;
    
    if( strncmp( memoryData10, "test1", 5 ) == 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds the String: '%s' this is good", memoryData10, (char*)memoryData10 );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    } else {
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds not the correct String, memory curruption !", memoryData10 );
        etDebugMessage( etID_LEVEL_CRITICAL, etDebugTempMessage );
    }

// Append some data
    etMemorySetOffset( memoryData10, (void*)"ing\0", 4, 4 );
    
    if( strncmp( memoryData10, "testing", 7 ) == 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds the String: '%s' this is good", memoryData10, (char*)etMemoryBlockActualData );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    } else {
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds not the correct String, memory curruption !", memoryData10 );
        etDebugMessage( etID_LEVEL_CRITICAL, etDebugTempMessage );
    }

// append some more data
    etMemorySetOffset( memoryData10, (void*)" a new string\0", 7, 13 );

    if( strncmp( memoryData10, "testing a new string", 20 ) == 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds the String: '%s' this is good", memoryData10, (char*)etMemoryBlockActualData );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    } else {
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds not the correct String, memory curruption !", memoryData10 );
        etDebugMessage( etID_LEVEL_CRITICAL, etDebugTempMessage );
    }

    etApicheckTimer( "OK" );
    return etID_YES;
}


etID_STATE            etMemoryApiCheck(){
    //etApicheckTimerInit();

    etInit( 0, NULL );
    etDebugLevelSet( etID_LEVEL_ALL );

    //etMemoryTestNULL();
    //etMemoryTestAlloc();
    etMemoryTestData();

    return etID_YES;
}












