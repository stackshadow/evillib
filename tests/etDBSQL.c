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

// WE release directly the memory
#define ET_MEMORY_FREE_ON_RELEASE 1

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


    return sqliteDriver;
}

void					etDBSQLiteRaw( etDBDriver* sqliteDriver, etDBTable* dbTable ){

// insert
	etDBSQLiteQueryExecute( sqliteDriver, dbTable, "INSERT INTO `testTable`(`uuid`,`username`,`data`,`index`) VALUES ( 'asdad','buhja','exit','3')" );

	
	etDBSQLiteQueryExecute( sqliteDriver, dbTable, "SELECT * FROM testTable WHERE `uuid` = 'asdad';" );
	while( etDBDriverDataNext( sqliteDriver, dbTable ) != etID_YES ){
		exit(-1);
	}

}



// ############# generic #############

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

// table should not exist
	//if( etDBSQLiteTableExists( driver, dbTable ) == etID_YES ){
	if( etDBSQLiteTableExists( driver, dbTable ) == etID_YES ){
		etDebugMessage( etID_STATE_CRIT, "Error 'testTable' exist in DB..." );
		exit(-1);
	}

// add the table
	etDBDriverTableAdd( driver, dbTable );

// check if table exist
	if( etDBSQLiteTableExists( driver, dbTable ) != etID_YES ){
		etDebugMessage( etID_STATE_CRIT, "Error 'testTable' don't exist in DB..." );
		exit(-1);
	}

// add a column
	etDBColumnAppend( dbTable, "index", etDBCOLUMN_TYPE_INT, etDBCOLUMN_OPTION_NOTHING );
	etDBDriverColumnAdd( driver, dbTable, "index" );


	return dbTable;
}


void					etDBDataTest( etDBDriver* driver, etDBTable* dbTable ){

// vars
	etDBFilter*		dbFilter;
	const char*		dbValueCharArray;

// set values and create some data
	etDBColumnSelect( dbTable, "uuid" );
	etDBColumnSetValue( dbTable, "b4681c03-3a91-4dcb-baee-f6cb9c08e1d1" );
	etDBColumnSelect( dbTable, "username" );
	etDBColumnSetValue( dbTable, "root" );
	etDBColumnSelect( dbTable, "data" );
	etDBColumnSetValue( dbTable, "The root user" );
	etDBColumnSelect( dbTable, "index" );
	etDBColumnSetValue( dbTable, "0" );
// Add the Data itselfe
	etDBDriverDataAdd( driver, dbTable );



	etDBColumnSelect( dbTable, "uuid" );
	etDBColumnSetValue( dbTable, "8b8a1380-a251-49b4-b4df-c56863f6c78b" );
	etDBColumnSelect( dbTable, "username" );
	etDBColumnSetValue( dbTable, "mainUser" );
	etDBColumnSelect( dbTable, "data" );
	etDBColumnSetValue( dbTable, "The main user account" );
	etDBColumnSelect( dbTable, "index" );
	etDBColumnSetValue( dbTable, "1" );
// Add the Data itselfe
	etDBDriverDataAdd( driver, dbTable );



	etDBColumnSelect( dbTable, "uuid" );
	etDBColumnSetValue( dbTable, "93d12ebb-9f2f-4368-bdb6-64674f279118" );
	etDBColumnSelect( dbTable, "username" );
	etDBColumnSetValue( dbTable, "guest" );
	etDBColumnSelect( dbTable, "data" );
	etDBColumnSetValue( dbTable, "The guest" );
	etDBColumnSelect( dbTable, "index" );
	etDBColumnSetValue( dbTable, "2" );
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


	etDBFilterFree( dbFilter );
}





int                     main( int argc, const char* argv[] ){
    etDebugLevelSet( etID_LEVEL_ALL );
	etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_DETAIL_DB );

// vars
	etDBDriver* sqliteDriver = NULL;
	etDBTable* dbTable = NULL;



// ######################### Test SQLITE #########################
	sqliteDriver = etDBSQLiteTest();
	dbTable = etDBCreateTableTest( sqliteDriver );
	etDBDataTest( sqliteDriver, dbTable );
	etDBSQLiteRaw( sqliteDriver, dbTable );
	etDBTableFree( dbTable );
	etDBDriverFree( sqliteDriver );




	


// compact memory
	etMemoryDump( NULL, NULL );
	etMemoryCompact();
	
// normaly only 113Bytes should be left, if its more, we have an memory leak
// inside evillib functions
	size_t sizeLeft = etMemoryRealSize();
	if( sizeLeft > 113 ){
		fprintf( stderr, "ERROR: Memory leak ! Normaly 113Bytes should be left, but we have: %li", sizeLeft );
		etMemoryDump( NULL, NULL );
		exit(-1);
	}
	
	
	return 0;
}











