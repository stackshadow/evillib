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


#include "core/etDebug.h"
#include "app/apicheck/etApicheck.h"
#include "memory/etMemoryBlock.h"
#include "memory/etMemory.h"



etID_STATE			etMemoryTest(){
	etDebugMessage( etID_LEVEL_DETAIL, "Start test.." );
	etApicheckTimer();


// Vars
	etMemoryBlock		*etMemoryBlockActual = NULL;
	char				*etMemoryBlockActualData = NULL;
	etMemoryBlock		*etMemoryBlock5 = NULL;
	etMemoryBlock		*etMemoryBlock8 = NULL;
	etMemoryBlock		*etMemoryBlock10 = NULL;
	etMemoryBlock		*etMemoryBlock20 = NULL;
	etMemoryBlock		*etMemoryBlock30 = NULL;
	etMemoryBlock		*etMemoryBlock40 = NULL;

// Run all functions with NULL-Pointer
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Error Checking Functions" );
	etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
	__etMemoryAlloc( NULL, 10 );
	__etMemoryRequest( NULL, 10 );
	__etMemoryRelease( NULL );
	__etMemorySet( NULL, NULL, 10 );
	__etMemorySetOffset( NULL, NULL, 10, 10 );
	etMemoryOptFreeUnused();
	etMemoryOptResort();
	etMemoryDump( NULL, NULL );
	

// Prealloc Blocks
	etMemoryAlloc( etMemoryBlock5, 5 );
	etMemoryRequest( etMemoryBlock8, 8 );
	etMemoryRequest( etMemoryBlock10, 10 );
	etMemoryRequest( etMemoryBlock20, 20 );
	etMemoryRequest( etMemoryBlock30, 30 );
	etMemoryRequest( etMemoryBlock40, 40 );

// Dump
	etMemoryDump( NULL, NULL );

// Release two Blocks
	etMemoryBlockActual = etMemoryBlock10;
	etMemoryRelease( etMemoryBlock10 );
	etMemoryRelease( etMemoryBlock20 );
	etMemoryDump( NULL, NULL );
	etMemoryRequest( etMemoryBlock10, 9 );
	if( etMemoryBlockActual != etMemoryBlock10 ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "We dont get the correct Block, etMemory dont work as expected !" );
		etDebugMessage( etID_LEVEL_CRITICAL, etDebugTempMessage );
	}

	etMemoryDump( NULL, NULL );



// We write a string to the block
	etMemorySet( etMemoryBlock10, (void*)"test1\0", 6 );
	etMemoryBlockDataGet( etMemoryBlock10, etMemoryBlockActualData );
	
	if( strncmp( etMemoryBlockActualData, "test1", 5 ) == 0 ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds the String: '%s' this is good", etMemoryBlock10, (char*)etMemoryBlockActualData );
		etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
	} else {
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds not the correct String, memory curruption !", etMemoryBlock10 );
		etDebugMessage( etID_LEVEL_CRITICAL, etDebugTempMessage );
	}

// Append some data
	etMemorySetOffset( etMemoryBlock10, (void*)"ing\0", 4, 4 );
	etMemoryBlockDataGet( etMemoryBlock10, etMemoryBlockActualData );
	
	if( strncmp( etMemoryBlockActualData, "testing", 7 ) == 0 ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds the String: '%s' this is good", etMemoryBlock10, (char*)etMemoryBlockActualData );
		etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
	} else {
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds not the correct String, memory curruption !", etMemoryBlock10 );
		etDebugMessage( etID_LEVEL_CRITICAL, etDebugTempMessage );
	}

// append some more data
	etMemorySetOffset( etMemoryBlock10, (void*)" a new string\0", 7, 13 );
	etMemoryBlockDataGet( etMemoryBlock10, etMemoryBlockActualData );

	if( strncmp( etMemoryBlockActualData, "testing a new string", 20 ) == 0 ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds the String: '%s' this is good", etMemoryBlock10, (char*)etMemoryBlockActualData );
		etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
	} else {
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds not the correct String, memory curruption !", etMemoryBlock10 );
		etDebugMessage( etID_LEVEL_CRITICAL, etDebugTempMessage );
	}


// End Timer
	etApicheckTimer( __ET_DEBUG_FUNCTION, __LINE__ );


	etDebugMessage( etID_LEVEL_DETAIL, "Test finished" );
	return etID_YES;
}













