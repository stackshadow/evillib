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

#include "db/etDBObject.c"
#include "db/etDBObjectTable.c"


int                     main( int argc, const char* argv[] ){
    etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_ALL );


// vars
    etDBObject      *table;

// alloc
    etDBObjectAlloc( table );

// this should not ! work
    const char *testTableName = NULL;
    if( etDBObjectTableNameGet( table, testTableName ) == etID_YES ){
        return -1;
    }

// append two tables
    etDBObjectTableAdd( table, "table1" );
    etDBObjectTableAdd( table, "table2" );
    etDBObjectTableAdd( table, "table3" );
    
// dump it
    etDBObjectDump( table );

// we go to every table
    etDBObjectSelectionReset( table );


    etDBObjectTableNext( table );
    etDBObjectTableNameGet( table, testTableName );
    if( strncmp(testTableName,"table1",6) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table %s != table1", testTableName );
        etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
        return -1;
    }

    etDBObjectTableNext( table );
    etDBObjectTableNameGet( table, testTableName );
    if( strncmp(testTableName,"table2",6) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table %s != table2", testTableName );
        etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
        return -1;
    }

    etDBObjectTableNext( table );
    etDBObjectTableNameGet( table, testTableName );
    if( strncmp(testTableName,"table3",6) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table %s != table3", testTableName );
        etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
        return -1;
    }

// pick a table
    etDBObjectTablePick( table, "table2" );
    etDBObjectTableNameGet( table, testTableName );
    if( strncmp(testTableName,"table2",6) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table %s != table2", testTableName );
        etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
        return -1;
    }




    etDBObjectDump( table );
    etDBObjectFree( table );
    return 0;
}