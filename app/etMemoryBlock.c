/* Apicheck Function for etMemoryBlock
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
#include "etApicheck.h"
#include "memory/etMemoryBlock.h"
#include "memory/etMemoryList.h"
#include "memory/etMemory.h"


etID_STATE			etMemoryBlockTest(){
	etDebugMessage( etID_LEVEL_DETAIL, "Start test.." );

// Start Timer
	etApicheckTimer();

// Vars
	etMemoryBlock		*etMemoryBlockActual = NULL;
	etMemoryBlock		*etMemoryBlockForCopy = NULL;
	etID_STATE			returnState = etID_NO;

// We allocate an Block
	etMemoryBlockAlloc( etMemoryBlockActual, 10 );

// Is it free? ( should not )
	if( etMemoryBlockIsFree( etMemoryBlockActual ) != etID_NO ){
		etDebugMessage( etID_LEVEL_CRITICAL, "Setting the State is incorrect" );
		return etID_LEVEL_CRITICAL;
	}


// Testing etMemoryBlockHasSpace
	if( etMemoryBlockHasSpace(etMemoryBlockActual,2) != etID_YES ){
		etDebugMessage( etID_LEVEL_CRITICAL, "Size error" );
		return etID_LEVEL_CRITICAL;
	}
	if( etMemoryBlockHasSpace(etMemoryBlockActual,20) == etID_YES ){
		etDebugMessage( etID_LEVEL_CRITICAL, "Size error" );
		return etID_LEVEL_CRITICAL;
	}

// Test the data
	char *etMemoryData = NULL;
	etMemoryBlockDataGet( etMemoryBlockActual, etMemoryData );
	memcpy( etMemoryData, "Test\0", 5 );
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds now the String: %s", etMemoryBlockActual, etMemoryData );
	etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );

	etMemoryBlockActual = NULL;
	etMemoryBlockFromData( etMemoryData, etMemoryBlockActual );

// Is it free? ( should not )
	if( etMemoryBlockIsFree( etMemoryBlockActual ) != etID_NO ){
		etDebugMessage( etID_LEVEL_CRITICAL, "Setting the State is incorrect" );
		return etID_LEVEL_CRITICAL;
	}


// CopyMemory
	returnState = etMemoryBlockCopy( etMemoryBlockForCopy, etMemoryBlockActual, 2 );
	if( returnState == etID_STATE_PARAMETER_MISSUSE ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p This is normal ;)", etMemoryBlockActual );
		etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
	}


// Release memory
	etMemoryBlockRelease( etMemoryBlockActual );

// Is it free? ( should be )
	if( etMemoryBlockIsFree( etMemoryBlockActual ) != etID_YES ){
		etDebugMessage( etID_LEVEL_CRITICAL, "Setting the State is incorrect" );
		return etID_LEVEL_CRITICAL;
	}


	etMemoryBlockFree( etMemoryBlockActual );

// Timer End
	etApicheckTimer( __ET_DEBUG_FUNCTION, __LINE__ );


	etDebugMessage( etID_LEVEL_DETAIL, "Test finished" );
	return etID_YES;
}
