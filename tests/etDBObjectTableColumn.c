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
#include "db/etDBObjectTableColumn.c"


int                     main( int argc, const char* argv[] ){
    etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_ALL );

// alloc the dbobject
    etDBObject *dbObject;
    etDBObjectAlloc( dbObject );

// add some tables
    etDBObjectTableAdd( dbObject, "table1" );
    etDBObjectTableAdd( dbObject, "table2" );
    etDBObjectTableAdd( dbObject, "table3" );
    etDBObjectTableAdd( dbObject, "table4" );
    etDBObjectDump( dbObject );


// pick table2
    etDBObjectTablePick( dbObject, "table2" );


// add some columns
    etDBObjectTableColumnAdd( dbObject, "column1", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_NOTHING );
    etDBObjectTableColumnAdd( dbObject, "column2", etDBCOLUMN_TYPE_INT, etDBCOLUMN_OPTION_NOTHING );
    etDBObjectTableColumnAdd( dbObject, "column3", etDBCOLUMN_TYPE_FLOAT, etDBCOLUMN_OPTION_NOTHING );


    etDBObjectTablePick( dbObject, "table1" );
    etDBObjectIterationReset( dbObject );
    if( etDBObjectTableColumnNext( dbObject ) == etID_YES ){
        exit(-1);
    }

    etDBObjectTablePick( dbObject, "table2" );
    etDBObjectIterationReset( dbObject );
    if( etDBObjectTableColumnNext( dbObject ) != etID_YES ){
        exit(-1);
    }

// now we check the column names
    const char *columnName = NULL;
    etDBObjectTableColumnNameGet( dbObject, columnName );
    if( strncmp(columnName,"column1",7) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "columnName %s != column1", columnName );
        etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
        return -1;
    }


    if( etDBObjectTableColumnNext( dbObject ) != etID_YES ){
        exit(-1);
    }
    etDBObjectTableColumnNameGet( dbObject, columnName );
    if( strncmp(columnName,"column2",7) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "columnName %s != column2", columnName );
        etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
        return -1;
    }


    etDBObjectDump( dbObject );
}