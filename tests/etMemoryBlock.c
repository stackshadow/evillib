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

// binarys we need
//#include "core/etInit.c"
#include "memory/etMemoryBlock.c"

#include "app/etApicheck.c"

etID_STATE              etApichecketMemoryBlock(){
    etApicheckTimer( "etMemoryBlock: check" );


// Vars
    etMemoryBlock       *etMemoryBlockActual = NULL;
    etMemoryBlock       *etMemoryBlockForCopy = NULL;
    etID_STATE          returnState = etID_NO;
    char                *etMemoryData = NULL;

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
    etMemoryData = NULL;
    etMemoryBlockDataGet( etMemoryBlockActual, etMemoryData );
    memcpy( etMemoryData, "Test\0", 5 );
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p holds now the String: %s", etMemoryBlockActual, etMemoryData );
    etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );




// Is it free? ( should not )
    if( etMemoryBlockIsFree( etMemoryBlockActual ) != etID_NO ){
        etDebugMessage( etID_LEVEL_CRITICAL, "Setting the State is incorrect" );
        return etID_LEVEL_CRITICAL;
    }


// CopyMemory
    etMemoryBlockAlloc( etMemoryBlockForCopy, 10 );
    returnState = etMemoryBlockCopy( etMemoryBlockForCopy, etMemoryBlockActual, 10 );
    if( returnState == etID_STATE_ERR_PARAMETER ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p This is normal ;)", etMemoryBlockActual );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    }
    etMemoryData = NULL;
    etMemoryBlockDataGet( etMemoryBlockForCopy, etMemoryData );
    if( strncmp(etMemoryData,"Test",4) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Memory not correctly copied" );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    }

// Release memory
    etMemoryBlockSetReleaseState( etMemoryBlockActual, etID_TRUE );

// Is it free? ( should be )
    if( etMemoryBlockIsFree( etMemoryBlockActual ) != etID_YES ){
        etDebugMessage( etID_LEVEL_CRITICAL, "Setting the State is incorrect" );
        return etID_LEVEL_CRITICAL;
    }


    etMemoryBlockFree( etMemoryBlockActual );
    etMemoryBlockFree( etMemoryBlockForCopy );


    etApicheckTimer( "OK" );
    return 0;
}


etID_STATE              etMemoryBlockTestRef(){
    etApicheckTimer( "etMemory: Test alloc" );

// Prealloc Blocks
    etMemoryBlock       *etMemoryBlockA = NULL;
    etMemoryBlock       *etMemoryBlockB = NULL;
    etMemoryBlock       *etMemoryBlockC = NULL;

// We allocate an Block
    etMemoryBlockAlloc( etMemoryBlockA, 10 );
    etMemoryBlockAlloc( etMemoryBlockB, 10 );
    etMemoryBlockAlloc( etMemoryBlockC, 10 );

// own
    etMemoryBlockOwn( etMemoryBlockA, etMemoryBlockB );
    etMemoryBlockOwn( etMemoryBlockA, etMemoryBlockC );

// release a owned block
    etMemoryBlockSetReleaseState( etMemoryBlockC, etID_YES );
    etMemoryBlockIsFree( etMemoryBlockA );
    etMemoryBlockIsFree( etMemoryBlockC );

// release the parent
    etMemoryBlockSetReleaseState( etMemoryBlockA, etID_YES );

// missed block
    etMemoryBlockIsFree( etMemoryBlockB );

}


int                     main( int argc, const char* argv[] ){
    //etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_ALL );

    //etApichecketMemoryBlock();
    etMemoryBlockTestRef();
}
