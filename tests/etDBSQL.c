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
#include "db/etDBObjectFilter.c"
#include "db/etDBObjectValue.c"
#include "dbdriver/etDBSQL.c"
#include "dbdriver/etDBDriver.c"
#include "dbdriver/etDBSQLite.c"
#include "dbdriver/etDBPSQL.c"


etDBDriver              dbDriver;
etDBObject*             dbObject;




etDBDriver*             etDBSQLiteTest(){



// init the driver
    etDBSQLiteDriverInit( &dbDriver, "/tmp/test.sqlite" );

    return NULL;
}


void                    etDBDriverTest( etDBDriver *dbDriver ){

// is connected ?
    if( etDBDriverIsConnect(dbDriver) != etID_YES ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Not Connected" );
        etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
        return;
    }


// we add  dummy and delete it
    etDBObjectTablePick( dbObject, "dummy" );
    etDBDriverTableAdd( dbDriver, dbObject );

// we select the table we would like to add
    etDBObjectTablePick( dbObject, "contacts" );
    etDBDriverTableAdd( dbDriver, dbObject );

// we select the table we would like to add
    etDBObjectTablePick( dbObject, "city" );
    etDBDriverTableAdd( dbDriver, dbObject );

// delete table
    etDBObjectTablePick( dbObject, "dummy" );
    etDBDriverTableRemove( dbDriver, dbObject );


// oh no, we forget something for the table, we need an adittional column
    etDBObjectTablePick( dbObject, "city" );
    etDBObjectTableColumnAdd( dbObject, "inhabitants", etDBCOLUMN_TYPE_INT, etDBCOLUMN_OPTION_NOTHING );
    etDBDriverColumnAdd( dbDriver, dbObject );


/*
SELECT uuid,postalcode,displayName,inhabitants  FROM city;

DROP TABLE IF EXISTS t1_backup;
CREATE TABLE t1_backup( 'postalcode' INTEGER,'uuid' TEXT  UNIQUE,'displayName' TEXT );
INSERT INTO t1_backup SELECT postalcode,uuid,displayName FROM city;
DROP TABLE city;
ALTER TABLE t1_backup RENAME TO city

*/

// we add some data
    etDBObjectTablePick( dbObject, "city" );

    etDBObjectValueClean( dbObject );
    etDBObjectValueSet( dbObject, "uuid", "000001" );
    etDBObjectValueSet( dbObject, "displayName", "Berlin" );
    etDBObjectValueSet( dbObject, "postalcode", "10115" );
    etDBObjectDump( dbObject );
    etDBDriverDataAdd( dbDriver, dbObject );

    etDBObjectValueClean( dbObject );
    etDBObjectValueSet( dbObject, "uuid", "000002" );
    etDBObjectValueSet( dbObject, "displayName", "Berlin" );
    etDBObjectValueSet( dbObject, "postalcode", "10116" );
    etDBObjectDump( dbObject );
    etDBDriverDataAdd( dbDriver, dbObject );

    etDBObjectValueClean( dbObject );
    etDBObjectValueSet( dbObject, "uuid", "000003" );
    etDBObjectValueSet( dbObject, "displayName", "Hannover" );
    etDBObjectValueSet( dbObject, "postalcode", "30159" );
    etDBObjectDump( dbObject );
    etDBDriverDataAdd( dbDriver, dbObject );

    etDBObjectValueClean( dbObject );
    etDBObjectValueSet( dbObject, "uuid", "000004" );
    etDBObjectValueSet( dbObject, "displayName", " Iter \"The big\" City" );
    etDBObjectValueSet( dbObject, "postalcode", "0" );
    etDBDriverDataAdd( dbDriver, dbObject );


// we change some data
    etDBObjectValueClean( dbObject );
    etDBObjectValueSet( dbObject, "uuid", "000002" );
    etDBObjectValueSet( dbObject, "displayName", "IterCity" );
    etDBObjectValueSet( dbObject, "postalcode", "0" );
    etDBDriverDataChange( dbDriver, dbObject );

// we delete some data
    etDBObjectValueClean( dbObject );
    etDBObjectValueSet( dbObject, "uuid", "000004" );
    etDBDriverDataRemove( dbDriver, dbObject );

// vars
    const char *tempValue;

// try to get NOTHING
    etDBObjectFilterClear( dbObject );
    etDBObjectFilterAdd( dbObject, 0, etDBFILTER_OP_AND, "displayName", etDBFILTER_TYPE_CONTAIN, "Ahorn" );
    etDBDriverDataGet( dbDriver, dbObject );                   // run the query
    etDBDriverDataNext( dbDriver, dbObject );                  // get the first resultset
    etDBObjectValueGet( dbObject, "displayName", tempValue );


// we filter our data
    etDBObjectFilterClear( dbObject );
    etDBObjectFilterAdd( dbObject, 0, etDBFILTER_OP_AND, "uuid", etDBFILTER_TYPE_CONTAIN, "000001" );

// get data
    etDBDriverDataGet( dbDriver, dbObject );                   // run the query
    etDBDriverDataNext( dbDriver, dbObject );                  // get the first resultset
    etDBObjectValueGet( dbObject, "displayName", tempValue );
    etDBObjectValueGet( dbObject, "displayName", tempValue );


}


int                     main( int argc, const char* argv[] ){
    etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_ALL );
    etDebugLevelSet( etID_LEVEL_DETAIL_DB );

// alloc the dbobject
    etDBObjectAlloc( dbObject );

// contact table
    etDBObjectTableAdd( dbObject, "contacts" );
    etDBObjectTableColumnAdd( dbObject, "uuid", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_PRIMARY | etDBCOLUMN_OPTION_UNIQUE );
    etDBObjectTableColumnAdd( dbObject, "prename", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_NOTHING );
    etDBObjectTableColumnAdd( dbObject, "familyname", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_NOTHING );
    etDBObjectTableColumnAdd( dbObject, "city_uuid", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_NOTHING );

// city table
    etDBObjectTableAdd( dbObject, "city" );
    etDBObjectTableColumnAdd( dbObject, "uuid", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_PRIMARY | etDBCOLUMN_OPTION_UNIQUE );
    etDBObjectTableColumnPrimarySet( dbObject, "uuid" );
    etDBObjectTableColumnAdd( dbObject, "displayName", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_NOTHING );
    etDBObjectTableColumnAdd( dbObject, "postalcode", etDBCOLUMN_TYPE_INT, etDBCOLUMN_OPTION_NOTHING );

// dummy
    etDBObjectTableAdd( dbObject, "dummy" );
    etDBObjectTableColumnAdd( dbObject, "uuid", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_PRIMARY | etDBCOLUMN_OPTION_UNIQUE );
    etDBObjectTableColumnPrimarySet( dbObject, "uuid" );


    etDBObjectDump( dbObject );


// init the driver
    etDBPSQLDriverInit( &dbDriver, "localhost", "127.0.0.1", "5435", "root", "root", "doDB", "require" );
    //etDBSQLiteTest();
    etDBDriverTest( &dbDriver );

    etDBDriverDisConnect( &dbDriver );



/*
    PGresult *PQexec(PGconn *conn, const char *command);
    ExecStatusType PQresultStatus(const PGresult *res);
    PGRES_COMMAND_OK PGRES_TUPLES_OK
    char *PQresultErrorMessage(const PGresult *res);
    PQclear(PGresult *res);
*/



}











