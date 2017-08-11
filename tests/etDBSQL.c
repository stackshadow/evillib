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

#include "db/etDBTable.c"
#include "db/etDBColumn.c"
#include "dbdriver/etDBSQL.c"
#include "dbdriver/etDBSQLite.c"
//#include "dbdriver/etDBDriver.c"

//#include "dbdriver/etDBPSQL.c"


etDBDriver              dbDriver;




etDBDriver*             etDBSQLiteTest(){

// vars
	etDBDriver*		sqliteDriver;

// sqlite-driver
	etDBDriverAlloc( sqliteDriver );
	etDBSQLiteDriverInit( sqliteDriver, "/tmp/test.sqlite" );
	etDBDriverConnect( sqliteDriver );


// list tables
	//etDBDriverTableList( sqliteDriver, sqliteMaster );

//SELECT name FROM my_db.sqlite_master WHERE type='table';

    return sqliteDriver;
}


etDBTable*				etDBCreateTableTest( etDBDriver* driver ){

// vars
	etDBTable*		dbTable = NULL;

// a test table
	etDBTableAlloc( dbTable );
	etDBTableSetName( dbTable, "testTable" );
	etDBColumnAppend( dbTable, "uuid", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_PRIMARY | etDBCOLUMN_OPTION_NOTNULL );
	etDBColumnAppend( dbTable, "username", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_UNIQUE );
	etDBColumnAppend( dbTable, "data", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_NOTHING );

// Run the query which create the table
	etDBDriverTableRemove( driver, dbTable );
	etDBDriverTableAdd( driver, dbTable );

// check if table exist
	if( etDBSQLiteTableExists( driver, dbTable ) != etID_YES ){
		etDebugMessage( etID_STATE_CRIT, "Error 'testTable' don't exist in DB..." );
	}


	return dbTable;
}


void					etDBDataTest( etDBDriver* driver, etDBTable* dbTable ){

// vars
	etDBFilter*		dbFilter;
	etString*		dbValueString;
	const char*		dbValueCharArray;

// set values and create some data
	etDBColumnSelect( dbTable, "uuid" );
	etDBColumnSetValue( dbTable, "b4681c03-3a91-4dcb-baee-f6cb9c08e1d1" );
	etDBColumnSelect( dbTable, "username" );
	etDBColumnSetValue( dbTable, "root" );
	etDBColumnSelect( dbTable, "data" );
	etDBColumnSetValue( dbTable, "The root user" );
// Add the Data itselfe
	etDBDriverDataAdd( driver, dbTable );



	etDBColumnSelect( dbTable, "uuid" );
	etDBColumnSetValue( dbTable, "8b8a1380-a251-49b4-b4df-c56863f6c78b" );
	etDBColumnSelect( dbTable, "username" );
	etDBColumnSetValue( dbTable, "mainUser" );
	etDBColumnSelect( dbTable, "data" );
	etDBColumnSetValue( dbTable, "The main user account" );
// Add the Data itselfe
	etDBDriverDataAdd( driver, dbTable );



	etDBColumnSelect( dbTable, "uuid" );
	etDBColumnSetValue( dbTable, "93d12ebb-9f2f-4368-bdb6-64674f279118" );
	etDBColumnSelect( dbTable, "username" );
	etDBColumnSetValue( dbTable, "guest" );
	etDBColumnSelect( dbTable, "data" );
	etDBColumnSetValue( dbTable, "The guest" );
// Add the Data itselfe
	etDBDriverDataAdd( driver, dbTable );



// we create a filter which match multiple entrys
	etDBFilterAlloc( dbFilter );
	etDBFilterAppend( dbFilter, 0, etDBFILTER_OP_AND, "data", etDBFILTER_TYPE_CONTAIN, "The" );

	etDBColumnSelect( dbTable, "uuid" );
	etDBDriverDataGet( driver, dbTable, dbFilter );
	while( etDBDriverDataNext( driver, dbTable ) == etID_YES ){
		etDBColumnGetValue( dbTable, dbValueCharArray );

	}

// make a more specific filter
	etDBFilterAppend( dbFilter, 0, etDBFILTER_OP_AND, "username", etDBFILTER_TYPE_CONTAIN, "User" );
	etDBColumnSelect( dbTable, "uuid" );
	etDBDriverDataGet( driver, dbTable, dbFilter );
	while( etDBDriverDataNext( driver, dbTable ) == etID_YES ){
		etDBColumnGetValue( dbTable, dbValueCharArray );
	}

}

/*
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



// SELECT uuid,postalcode,displayName,inhabitants  FROM city;
// DROP TABLE IF EXISTS t1_backup;
// CREATE TABLE t1_backup( 'postalcode' INTEGER,'uuid' TEXT  UNIQUE,'displayName' TEXT );
// INSERT INTO t1_backup SELECT postalcode,uuid,displayName FROM city;
// DROP TABLE city;
// ALTER TABLE t1_backup RENAME TO city


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
*/

int                     main( int argc, const char* argv[] ){
    etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_ALL );
    etDebugLevelSet( etID_LEVEL_DETAIL_DB );


	etDBDriver* sqliteDriver = etDBSQLiteTest();
	etDBTable* dbTable = etDBCreateTableTest( sqliteDriver );
	etDBDataTest( sqliteDriver, dbTable );

/*
    PGresult *PQexec(PGconn *conn, const char *command);
    ExecStatusType PQresultStatus(const PGresult *res);
    PGRES_COMMAND_OK PGRES_TUPLES_OK
    char *PQresultErrorMessage(const PGresult *res);
    PQclear(PGresult *res);
*/



}











