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

// WE release directly the memory
#define ET_MEMORY_FREE_ON_RELEASE 1

#include "core/etInit.c"

#include "string/etString.c"
#include "memory/etList.c"

#include "db/etDBTable.c"
#include "db/etDBColumn.c"
//#include "db/etDBFilter.c"
#include "dbdriver/etDBSQL.c"

#include "dbdriver/etDBSQLite.c"
//#include "db/etDBObjectTableColumn.c"
//#include "db/etDBObjectValue.c"


#include "app/etApicheck.h"
#include "app/etApicheck.c"



etID_STATE              etDBObjectTableApiCheck(){
    etApicheckTimer( "etDB: check table" );

    etDBTable*      dbTable = NULL;
    const char*     tempChar = NULL;


    etDBTableAlloc( dbTable );


// table name
    etDBTableSetName( dbTable, "привет мир" );
    etDBTableGetName( dbTable, tempChar );
    etDebugMessage( etID_STATE_WARN, tempChar );
    if( strncmp(tempChar,"привет мир",19) != 0 ){
        etDebugMessage( etID_STATE_CRIT, "Error in etDBObjectTableSetName" );
    }

// table display name
    etDBTableSetDisplayName( dbTable, "A Table with testdata" );
    etDBTableGetDisplayName( dbTable, tempChar );
    if( strncmp(tempChar,"A Table with testdata",21) != 0 ){
        etDebugMessage( etID_STATE_CRIT, "Error in etDBObjectTableGetDisplayName" );
    }

    etDBTableFree( dbTable );
    etMemoryDump( NULL, NULL );
    return etID_YES;
}


etID_STATE              etDBTableColumnTest(){
    etApicheckTimer( "etDB: check table" );

    etDBTable*      dbTable = NULL;




    etDBTableAlloc( dbTable );
    etDBTableSetName( dbTable, "Test" );
    etDBTableSetDisplayName( dbTable, "A Table with testdata" );

    etDBColumnAppend( dbTable, "uuid", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_PRIMARY | etDBCOLUMN_OPTION_NOTNULL | etDBCOLUMN_OPTION_UNIQUE );
    etDBColumnAppend( dbTable, "name", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_NOTHING );
    etDBColumnAppend( dbTable, "prename", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_NOTHING );

// iterate
	int dbTableColumnCounter = 0;
	etDBColumnIterateReset(dbTable);
    while( etDBColumnIterate(dbTable) == etID_YES ){

        const char*			dbColumnName = NULL;
		etDBColumnType		dbColumnType = etDBCOLUMN_TYPE_NOTHING;
		unsigned int		dbColumnOption = etDBCOLUMN_OPTION_NOTHING;

        __etDBColumnGet( dbTable, &dbColumnName, &dbColumnType, &dbColumnOption );

		switch( dbTableColumnCounter ){
			case 0:
				if( strncmp("uuid",dbColumnName,strlen(dbColumnName)) != 0 ){
					etDebugMessage( etID_STATE_CRIT, "Error Column 'uuid' not in table" );
				}
				if( dbColumnType != etDBCOLUMN_TYPE_STRING ){
					etDebugMessage( etID_STATE_CRIT, "Error Column type is not a string" );
				}
				if( dbColumnOption != (etDBCOLUMN_OPTION_PRIMARY | etDBCOLUMN_OPTION_NOTNULL | etDBCOLUMN_OPTION_UNIQUE) ){
					etDebugMessage( etID_STATE_CRIT, "Error Column options incorrect" );
				}
				break;

			case 1:
				if( strncmp("name",dbColumnName,strlen(dbColumnName)) != 0 ){
					etDebugMessage( etID_STATE_CRIT, "Error Column 'name' not in table" );
				}
				break;

			case 2:
				if( strncmp("prename",dbColumnName,strlen(dbColumnName)) != 0 ){
					etDebugMessage( etID_STATE_CRIT, "Error Column 'prename' not in table" );
				}
				break;

			default:
				etDebugMessage( etID_STATE_CRIT, "Error Column not handled." );
				break;
		}


		dbTableColumnCounter++;
    }

// fill values
    etDBColumnSelect( dbTable, "uuid" );
    etDBColumnSetValue( dbTable, "1203713-091273901273" );

    etDBColumnSelect( dbTable, "name" );
    etDBColumnSetValue( dbTable, "Musterman" );

    etDBColumnSelect( dbTable, "prename" );
    etDBColumnSetValue( dbTable, "Max" );

// check values
	const char* dbValueName = NULL;

	etDBColumnSelect( dbTable, "name" );
	etDBColumnGetValue( dbTable, dbValueName );
	if( strncmp(dbValueName,"Musterman",strlen("Musterman")) != 0 ){
		etDebugMessage( etID_STATE_CRIT, "Error: Column dont contains the correct value" );
	}


    etDBTableDump( dbTable );

    etDBTableFree( dbTable );
    etMemoryDump( NULL, NULL );
    return etID_YES;
}

/*
etID_STATE              etDBDriverTestSQLite(){
    etApicheckTimer( "etDB: check table" );

    etDBDriver*     dbDriver = NULL;
    etDBTable*      dbTable = NULL;
    const char*     tempChar = NULL;


// we need a driver
    etDBDriverAlloc( dbDriver );
    etDBSQLiteDriverInit( dbDriver, "/tmp/test.sqlite" );


// we build the table
    etDBTableAlloc( dbTable );
    etDBTableSetName( dbTable, "Test" );
    etDBTableSetDisplayName( dbTable, "A Table with testdata" );


    etDBColumnAlloc( dbColumn );
    etDBColumnSet( dbColumn, "uuid", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_PRIMARY | etDBCOLUMN_OPTION_NOTNULL | etDBCOLUMN_OPTION_UNIQUE );
    etDBTableAppendColumn( dbTable, dbColumn );

    etDBColumnAlloc( dbColumn );
    etDBColumnSet( dbColumn, "name", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_NOTHING );
    etDBTableAppendColumn( dbTable, dbColumn );

    etDBColumnAlloc( dbColumn );
    etDBColumnSet( dbColumn, "prename", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_NOTHING );
    etDBTableAppendColumn( dbTable, dbColumn );

// add the table
    etDBDriverTableAdd( dbDriver, dbTable );


// we add a bunch of data
    int index, len = 10;
    for( index = 0; index <= len; index++ ){

        char tempString[101];

    // uuid
        memset( tempString, 0, 101 );
        snprintf( tempString, 100, "%s_%i", "uuid", index );
        etDBTableGetColumn( dbTable, "uuid", dbColumn );
        etDBColumnSetValue( dbColumn, tempString );

    // prename
        memset( tempString, 0, 101 );
        snprintf( tempString, 100, "%s_%i", "Max", index );
        etDBTableGetColumn( dbTable, "prename", dbColumn );
        etDBColumnSetValue( dbColumn, tempString );

    // name
        memset( tempString, 0, 101 );
        snprintf( tempString, 100, "%s_%i", "Mustermann", index );
        etDBTableGetColumn( dbTable, "name", dbColumn );
        etDBColumnSetValue( dbColumn, tempString );

    // add it to the db
        etDBDriverDataAdd( dbDriver, dbTable );
    }


// get data
    etDBDriverDataGet( dbDriver, dbTable, NULL );
    etDBDriverDataNext( dbDriver, dbTable );
    etDBTableDumpColumn( dbTable );

// get data
    etDBDriverDataGetWithLimits( dbDriver, dbTable, 5, 1, NULL );
    etDBDriverDataNext( dbDriver, dbTable );
    etDBTableDumpColumn( dbTable );

// with filter
    etDBFilter*     dbFilter = NULL;

    etDBFilterAlloc( dbFilter );
    etDBFilterAppend( dbFilter, 0, etDBFILTER_OP_NOTHING, "prename", etDBFILTER_TYPE_EQUAL, "Max2" );

    etDBDriverDataGet( dbDriver, dbTable, dbFilter );
    etDBDriverDataNext( dbDriver, dbTable );
    etDBTableDumpColumn( dbTable );


    etDBDriverDisConnect( dbDriver );
}

*/

int                     main( int argc, const char* argv[] ){
    etDebugLevelSet( etID_LEVEL_ALL );
	etInit( argc, argv );

/*
	size_t etMemoryBlockSize = sizeof(etMemoryBlock);
	size_t etMemoryBlockListSize = sizeof(etMemoryBlockList);
*/

    etDBObjectTableApiCheck();
    etDBTableColumnTest();

// memcheck
	etMemoryCompact();
	etMemoryDump( NULL, NULL );
	size_t sizeLeft = etMemoryRealSize();
	if( sizeLeft > 113 ){
		fprintf( stderr, "ERROR: Memory leak ! Normaly 113Bytes should be left, but we have: %l", sizeLeft );
		exit(-1);
	}
	
}


