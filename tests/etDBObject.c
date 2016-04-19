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
#include "evillib-extra_depends.h"

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

#include "string/etString.c"
#include "string/etStringChar.c"

#include "db/etDBObject.c"
#include "db/etDBObjectTable.c"
#include "db/etDBObjectTableColumn.c"
#include "db/etDBObjectValue.c"


etID_BOOL               cb_etDBObjectTableIterate( void *userdata, const char *tableName, const char *tableDisplayName ){
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table name %s displayName %s", tableName, tableDisplayName );
    etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
    return etID_TRUE;
}


etID_STATE              etDBObjectTableApiCheck(){
    etApicheckTimer( "etDB: check table" );

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
    etDBObjectIterationReset( table );

/*
    etDBObjectTableNext( table );
    etDBObjectTableNameGet( table, testTableName );
    if( strncmp(testTableName,"table1",6) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table %s != table1", testTableName );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return -1;
    }

    etDBObjectTableNext( table );
    etDBObjectTableNameGet( table, testTableName );
    if( strncmp(testTableName,"table2",6) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table %s != table2", testTableName );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return -1;
    }

    etDBObjectTableNext( table );
    etDBObjectTableNameGet( table, testTableName );
    if( strncmp(testTableName,"table3",6) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table %s != table3", testTableName );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return -1;
    }
*/

// pick a table
    etDBObjectTablePick( table, "table2" );
    etDBObjectTableNameGet( table, testTableName );
    if( strncmp(testTableName,"table2",6) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table %s != table2", testTableName );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return -1;
    }


// this return just the table name
    const char *displayName = NULL;
    etDBObjectTableDisplayNameGet( table, "", displayName );
    if( strncmp(displayName,"table2",6) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table 'table2' don't return the correct display-name" );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return -1;
    }

// set some display names
    etDBObjectTableDisplayNameSet( table, "de", "Tabelle 2" );
    etDBObjectTableDisplayNameSet( table, "en", "table 2" );
    etDBObjectTableDisplayNameSet( table, "", "default" );

// check if display name work correct
    displayName = NULL;
    etDBObjectTableDisplayNameGet( table, "", displayName );
    if( strncmp(displayName,"default",7) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table 'table2' don't return the correct display-name" );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return -1;
    }

    displayName = NULL;
    etDBObjectTableDisplayNameGet( table, "an", displayName );
    if( strncmp(displayName,"default",7) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table 'table2' don't return the correct display-name" );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return -1;
    }

    displayName = NULL;
    etDBObjectTableDisplayNameGet( table, "de", displayName );
    if( strncmp(displayName,"Tabelle 2",9) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table 'table2' don't return the correct display-name" );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return -1;
    }


// use the iterator function
    etDBObjectTableIterate( table, "de", NULL, cb_etDBObjectTableIterate );


    etDBObjectDump( table );
    etDBObjectFree( table );
}


etID_STATE              etDBObjectTableColumnApiCheck(){
    etApicheckTimer( "etDB: check column" );

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
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return -1;
    }


    if( etDBObjectTableColumnNext( dbObject ) != etID_YES ){
        exit(-1);
    }
    etDBObjectTableColumnNameGet( dbObject, columnName );
    if( strncmp(columnName,"column2",7) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "columnName %s != column2", columnName );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return -1;
    }


    etDBObjectDump( dbObject );


// End Timer
    etApicheckTimer( "OK" );
    return etID_YES;
}


etID_STATE              etDBObjectValueApiCheck(){
    etApicheckTimer( "etDB: check value" );

// get the first value
    etDBObject *dbObject;
    const char *columnName = NULL;
    const char *columnValue = NULL;

// alloc the dbobject
    etDBObjectAlloc( dbObject );

// set some values
    etDBObjectValueSet( dbObject, "column1", "value1" );
    etDBObjectValueSet( dbObject, "column2", "value2" );
    etDBObjectValueSet( dbObject, "column3", "value3" );
    etDBObjectValueSet( dbObject, "column4", "value5" );
    etDBObjectValueSet( dbObject, "column5", "value5" );

// get value
    etDBObjectValueGet( dbObject, "column3", columnValue );
    if( strncmp(columnValue,"value3",6) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "value of 'column3' is '%s', should be 'value3'", columnValue );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return etID_NO;
    }

// reset
    int columnValueCount = 0;
    etDBObjectIterationReset( dbObject );
    while( etDBObjectValueNext(dbObject,columnName,columnValue) == etID_YES ){
        columnValueCount++;
    }
    if( columnValueCount != 5 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "count of columns is '%i' and not 5", columnValueCount );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return etID_NO;
    }

// dump the values
    const char *dumpValues = NULL;
    etDBObjectValueExport( dbObject, dumpValues );

// inject the values
    etDBObjectValueImport( dbObject, "{ \"name\": { \"value\":\"max\" }, \"familyname\": { \"value\":\"mustermann\" } }" );
// get value
    etDBObjectValueGet( dbObject, "name", columnValue );
    if( strncmp(columnValue,"max",3) != 0 ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "value of 'column3' is '%s', should be 'max'", columnValue );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return etID_NO;
    }



// End Timer
    etApicheckTimer( "OK" );
    return etID_YES;
}



etID_STATE              etDBObjectApiCheck(){
    etApicheckTimer( "etString: check" );


    etApicheckTimer( "OK" );
    return etID_YES;
}



int                     main( int argc, const char* argv[] ){
    etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_ALL );


    etDBObjectTableApiCheck();
    etDBObjectTableColumnApiCheck();
    etDBObjectValueApiCheck();


}