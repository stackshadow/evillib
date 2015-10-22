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

#include "evillib-extra_depends.h"

#include <sqlite3.h>

#include "core/etDebug.h"
#include "etApicheck.h"

#include "memory/etMemoryBlock.h"
#include "memory/etMemory.h"
#include "string/etString.h"
#include "string/etStringChar.h"

#include "etDBTable.h"
#include "etDBValue.h"
#include "etDBFilter.h"
#include "etDB.h"

#ifdef ET_SINGLEMODULE
    #include "core/etObject.h"
    #include "etDBTable.c"
    #include "etDBColumn.c"
    #include "etDBValue.c"
    #include "etDBFilter.c"
    #include "etDBRelations.c"
    #include "etDB.c"

	#include "etDBSQLite.c"
#endif


// Vars
etDebug             dbgInfo_static;
etDebug*            dbgInfo = &dbgInfo_static;
etDB*               etDBActual = NULL;
etDBTable*          dbTableUsers = NULL;
etDBTable*          dbTableCity = NULL;
etDBValue*          dbValues = NULL;

void                etjDBTestTable(){

// Vars
    const char *tableName = NULL;
    const char *tableNameVisible = NULL;
	
// Append tables
    etDBTableAlloc( dbTableUsers, dbgInfo );
    etDBTableAppend( dbTableUsers, "users", dbgInfo );
    etDBTableVisibleNameSet( dbTableUsers, "UserAccounts", dbgInfo );

    etDBTableAlloc( dbTableCity, dbgInfo );
    etDBTableAppend( dbTableCity, "city", dbgInfo );
    etDBTableAppend( dbTableCity, "unknown", dbgInfo );
    etDBTableAppend( dbTableCity, "hans", dbgInfo );

// check the correct table name
	etDBTableNameOriginalGet( dbTableUsers, tableName, caller(dbgInfo) );
	if( tableName != NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBTableNameGet not work as expected" );

// on this step, we get an error ( and this time its normal ), so we need to reset it
    etDebugReset( caller(dbgInfo) );

	etDBTableNameGet( dbTableUsers, tableName, caller(dbgInfo) );
 	if( tableName == NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBTableNameGet not work as expected" );
	if( strncmp( tableName, "users", 5 ) != 0 ){
		etDebugMessage( etID_LEVEL_CRITICAL, "tableName is wrong" );
	}


	etDBTableVisibleNameGet( dbTableUsers, tableNameVisible, caller(dbgInfo) );
	if( tableNameVisible == NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBTableNameGet not work as expected" );
	if( strncmp( tableNameVisible, "UserAccounts", 12 ) != 0 ){
		etDebugMessage( etID_LEVEL_CRITICAL, "tableNameVisible is wrong" );
	} 

// seek for the "unknown"-table
    etDBTableSeek( dbTableCity, "unknown", dbgInfo );
    etDBTableNameGet( dbTableCity, tableName, caller(dbgInfo) );
    if( tableName == NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBTableNameGet not work as expected" );
    if( strncmp( tableName, "unknown", 7 ) != 0 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "tableName is wrong" );
    }

    etDBTableSeek( dbTableCity, "city", dbgInfo );
    etDBTableNameGet( dbTableCity, tableName, caller(dbgInfo) );
    if( tableName == NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBTableNameGet not work as expected" );
    if( strncmp( tableName, "city", 4 ) != 0 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "tableName is wrong" );
    }
    
    etDBTableDumpf( dbTableUsers, caller(dbgInfo) );
}
void                etjDBTestColumn(){
	const char *columnName, *columnNameNew;

// Add some columns
	etDBColumnAppend( dbTableUsers, "userID", caller(dbgInfo) );
	etDBColumnIdentifierSet( dbTableUsers, caller(dbgInfo) );
	
	etDBColumnAppend( dbTableUsers, "prename", caller(dbgInfo) );
	etDBColumnTypeSet( dbTableUsers, etDB_COL_TYPE_TEXT, caller(dbgInfo) );
	
	etDBColumnAppend( dbTableUsers, "name", caller(dbgInfo) );
	etDBColumnTypeSet( dbTableUsers, etDB_COL_TYPE_TEXT, caller(dbgInfo) );
	
	etDBColumnAppend( dbTableUsers, "year", caller(dbgInfo) );
	etDBColumnTypeSet( dbTableUsers, etDB_COL_TYPE_INTEGER, caller(dbgInfo) );

    etDBTableDumpf( dbTableUsers, caller(dbgInfo) );
    etDBTableDumpf( dbTableCity, caller(dbgInfo) );

// check correct indexing
// this is not working because jansson not preserve order
/// @todo: encode / decode with preserve order to write this unit test

/*    if( etDBColumnIndexReset(dbTableUsers, caller(dbgInfo)) != etID_YES ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "Could not reset Column-Index" );
    }
    etDBColumnGetNext( dbTableUsers, columnName, caller(dbgInfo) );
    if( strncmp( columnName, "userID", 6 ) != 0 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "columnName is wrong" );
    }
    etDBColumnGetNext( dbTableUsers, columnName, caller(dbgInfo) );
    if( strncmp( columnName, "prename", 7 ) != 0 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "columnName is wrong" );
    }
    etDBColumnGetNext( dbTableUsers, columnName, caller(dbgInfo) );
    if( strncmp( columnName, "name", 4 ) != 0 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "columnName is wrong" );
    }*/

// seek for a column
	if( etDBColumnSeek( dbTableUsers, "prename", caller(dbgInfo) ) != etID_YES ){
		return etDebugMessage( etID_LEVEL_CRITICAL, "Column should be inside the table but is still missing");
	}
	
// check column-original name
// note: the original-name is only set if its verified from the sql-driver
// so it realy should ne NULL
	if( etDBColumnNameOriginalGet( dbTableUsers, columnName, caller(dbgInfo) ) == etID_YES ){
		return etDebugMessage( etID_LEVEL_CRITICAL, "There should be no original name !");
	}
	if( columnName != NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBColumnNameOriginalGet not work as expected" );
    etDebugReset( dbgInfo );

// check column name
// note: this functions just return the column name, doesn't matter if its the original or the new (to-set) name
	if( etDBColumnNameGet( dbTableUsers, columnNameNew, caller(dbgInfo) ) != etID_STATE_NEW ){
		return etDebugMessage( etID_LEVEL_CRITICAL, "Column Names are wrong");
	}
    if( columnNameNew == NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBColumnNameGet not work as expected" );
    if( strncmp( columnNameNew, "prename", 7 ) != 0 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "columnName is wrong" );
    }

// check type
	etDBColumnSeek( dbTableUsers, "year", caller(dbgInfo) );
    etDBColumnType columnType;
    etDBColumnTypeGet( dbTableUsers, columnType, caller(dbgInfo) );
    if( columnType != etDB_COL_TYPE_INTEGER ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "Column type should be etDB_COL_TYPE_INTEGER, but it's not");
    }


// check primary key
    etDBColumnSeek( dbTableUsers, "userID", caller(dbgInfo) );
    etDBColumnIdentifierSet( dbTableUsers, caller(dbgInfo) );
    
    const char* primaryKey = NULL;
    etDBColumnIdentifierGet( dbTableUsers, primaryKey, caller(dbgInfo) );
    if( strcmp("userID",primaryKey) != 0 ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "etDBColumnIdentifierSet not work as expected");
    }

}
void                etjDBTestValues(){

// alloc
    etDBValueAlloc( dbValues );
    
    etDBValueSet( dbValues, "column1", "value1" );
    etDBValueSet( dbValues, "column2", "value2" );
    etDBValueSet( dbValues, "column3", "value3" );

    const char*     columnName = NULL;
    const char*     columnValue = NULL;

    
    etDBValueGetNext( dbValues, columnName, columnValue );
    if( strncmp(columnName,"column1",7) != 0 ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "etDBValueGetNext not return the correct order of values !" );
    }
    if( strncmp(columnValue,"value1",6) != 0 ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "etDBValueGetNext not return the correct order of values !" );
    }


    etDBValueGetNext( dbValues, columnName, columnValue );
    if( strncmp(columnName,"column2",7) != 0 ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "etDBValueGetNext not return the correct order of values !" );
    }
    if( strncmp(columnValue,"value2",6) != 0 ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "etDBValueGetNext not return the correct order of values !" );
    }


    etDBValueGetNext( dbValues, columnName, columnValue );
    if( strncmp(columnName,"column3",7) != 0 ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "etDBValueGetNext not return the correct order of values !" );
    }
    if( strncmp(columnValue,"value3",6) != 0 ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "etDBValueGetNext not return the correct order of values !" );
    }
    
    etDBValueGetNext( dbValues, columnName, columnValue );

// free
    etDBValueFree( dbValues );
}
void                etjDBTestRelation(){

    etDBRelations*      dbRelation;
    
// alloc
    etDBRelationAlloc( dbRelation, caller(dbgInfo) );

// append some relation
    etDBRelationAppend( dbRelation, "tablea", "columna", "partnertable", "partnercolumn", caller(dbgInfo) );
    etDBRelationAppend( dbRelation, "tablec", "columnc", "tabled", "columnd", caller(dbgInfo) );

// vars
    const char*         columnName;
    const char*         partnerTableName;
    const char*         partnerColumnName;


// reset the relation indexer
    etDBRelationsIndexReset( dbRelation, caller(dbgInfo) );
    while( etDBRelationFindNext(dbRelation,"tablea",columnName,partnerTableName,partnerColumnName, caller(dbgInfo)) == etID_YES ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Found Table: %s", partnerTableName );
        etDebugPrintCustomMessage( caller(dbgInfo), etID_LEVEL_TEST, etDebugTempMessage );
    }


    etDBRelationsIndexReset( dbRelation, caller(dbgInfo) );
    while( etDBRelationFindNext(dbRelation,"tablec",columnName,partnerTableName,partnerColumnName, caller(dbgInfo)) == etID_YES ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Found Table: %s", partnerTableName );
        etDebugPrintCustomMessage( caller(dbgInfo), etID_LEVEL_TEST, etDebugTempMessage );
    }

// Try to get an explicit table to table relation
    etDBRelationsIndexReset( dbRelation, caller(dbgInfo) );
    while( etDBRelationFindNextTable(dbRelation,"tablec","tabled",columnName,partnerColumnName, caller(dbgInfo)) == etID_YES ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Found Table with column: %s <-> %s", columnName, partnerColumnName );
        etDebugMessage( etID_LEVEL_TEST, etDebugTempMessage );
    }

}

// SQLITE-stuff
void                etjDBSQLiteConnection(){
// With sqlite
	etDBSQLiteInit( etDBActual, caller(dbgInfo) );

// connect to an impossible location
	etDBSQLiteConnect( etDBActual, "/tmp/sqlite.sqlite", caller(dbgInfo) );

    etDBValueAlloc( dbValues );

}
void                etjDBTestTableCreate(){
    etDBTableDumpf( dbTableUsers, caller(dbgInfo) );
    
    etDBActionSet( etDBActual, etDB_ACTION_TABLE_ADD, caller(dbgInfo) );
    etDBRun( etDBActual, dbTableUsers, NULL, NULL, caller(dbgInfo) );
    
    
}
void                etjDBTestTableGet(){
// we reset there the debug-state
    etDebugReset( caller(dbgInfo) );
    
    etDBActionSet( etDBActual, etDB_ACTION_TABLELIST_GET, caller(dbgInfo) );
    etDBRun( etDBActual, NULL, NULL, NULL, caller(dbgInfo) );

    while( etDBNextResult( etDBActual, dbValues, caller(dbgInfo) ) == etID_YES ){
        
        const char *tableName = NULL;
        etDBValueGet( dbValues, "table", tableName );
        
        etDebugMessage( etID_LEVEL_DETAIL_DB, tableName );
    }
    
// now we get an specific table
    etDBActionSet( etDBActual, etDB_ACTION_TABLE_GET, caller(dbgInfo) );
    etDBRun( etDBActual, dbTableCity, NULL, NULL, caller(dbgInfo) );
// this table not exist inside the db, so we need to reset the faulty state
    etDebugReset( caller(dbgInfo) );
    etDBTableDumpf( dbTableCity, caller(dbgInfo) );

// get the real table
// clear it before
    etDBTableFree( dbTableUsers, dbgInfo );

// allocate the dbTableUsers
    etDBTableAlloc( dbTableUsers, dbgInfo );
// and we need to know the name
    etDBTableAppend( dbTableUsers, "users", dbgInfo );

    etDBActionSet( etDBActual, etDB_ACTION_TABLE_GET, caller(dbgInfo) );
    etDBRun( etDBActual, dbTableUsers, NULL, NULL, caller(dbgInfo) );
    etDBTableDumpf( dbTableUsers, caller(dbgInfo) );
}

void                etjDBTestDataAdd(){
	
    etDBValuesClean( dbValues );
    
    etDBValueSet( dbValues, "userID", "9c8910c2-232f-11e5-8b54-34e6d7289d44" );
    etDBValueSet( dbValues, "prename", "Homer" );
    
    etDBActionSet( etDBActual, etDB_ACTION_DATA_ADD, dbgInfo );
    etDBRun( etDBActual, dbTableUsers, NULL, dbValues, dbgInfo );



    etDBValuesClean( dbValues );
    
    etDBValueSet( dbValues, "userID", "97515fc0-3111-11e5-bd17-34e6d7289d44" );
    etDBValueSet( dbValues, "prename", "Sid" );
    etDBValueSet( dbValues, "year", "2015" );
    
    etDBActionSet( etDBActual, etDB_ACTION_DATA_ADD, dbgInfo );
    etDBRun( etDBActual, dbTableUsers, NULL, dbValues, dbgInfo );



    etDBValuesClean( dbValues );
    
    etDBValueSet( dbValues, "userID", "9aefc09a-3111-11e5-a8e8-34e6d7289d44" );
    etDBValueSet( dbValues, "prename", "Sid" );
    
    etDBActionSet( etDBActual, etDB_ACTION_DATA_ADD, dbgInfo );
    etDBRun( etDBActual, dbTableUsers, NULL, dbValues, dbgInfo );
    
}
void                etjDBTestDataGet(){
    

// clean the values
    etDBValuesClean( dbValues );

    etDBActionSet( etDBActual, etDB_ACTION_DATA_GET, caller(dbgInfo) );
    etDBRun( etDBActual, dbTableUsers, NULL, NULL, caller(dbgInfo) );

// we need a new filter
    etDBFilter*     dbFilter = NULL;
    etDBFilterAlloc( dbFilter, caller(dbgInfo) );
    etDBFilterAppend( dbFilter, etID_TYPE_AND, 1, "prename", etDB_FILTER_EQUAL, "Homer", caller(dbgInfo) );
    etDBFilterAppend( dbFilter, etID_TYPE_AND, 2, "name", etDB_FILTER_ISNULL, NULL, caller(dbgInfo) );
    etDBFilterAppend( dbFilter, etID_TYPE_AND, 2, "year", etDB_FILTER_ISNULL, NULL, caller(dbgInfo) );

    etDBValuesClean( dbValues );
    

    etDBActionSet( etDBActual, etDB_ACTION_DATA_GET, dbgInfo );
    etDBRun( etDBActual, dbTableUsers, dbFilter, NULL, dbgInfo );
}
void                etjDBTestDataChange(){
    

// clean the values
    etDBValuesClean( dbValues );

// we need a new filter
    etDBFilter*     dbFilter = NULL;
    etDBFilterAlloc( dbFilter, caller(dbgInfo) );
    etDBFilterAppend( dbFilter, etID_TYPE_AND, 1, "prename", etDB_FILTER_EQUAL, "Homer", caller(dbgInfo) );
    etDBFilterAppend( dbFilter, etID_TYPE_AND, 2, "name", etDB_FILTER_ISNULL, NULL, caller(dbgInfo) );
    etDBFilterAppend( dbFilter, etID_TYPE_AND, 2, "year", etDB_FILTER_ISNULL, NULL, caller(dbgInfo) );

    etDBActionSet( etDBActual, etDB_ACTION_DATA_GET, caller(dbgInfo) );
    etDBRun( etDBActual, dbTableUsers, dbFilter, NULL, caller(dbgInfo) );


    etDBValuesClean( dbValues );
    if( etDBNextResult( etDBActual, dbValues, caller(dbgInfo) ) != etID_YES ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "There should be an result inside the db");
    }

// Change Homer to Marge
    etDBValueSet( dbValues, "prename", "Marge" );
    etDBActionSet( etDBActual, etDB_ACTION_DATA_CHANGE, caller(dbgInfo) );
    etDBRun( etDBActual, dbTableUsers, dbFilter, dbValues, caller(dbgInfo) );
}




etID_STATE          etjDBTest(){

    etDebugLevelSet( etID_LEVEL_DETAIL_DB );

// Allocate the etDB-Object, this we need for all etDB-Functions !
    etDBAlloc( etDBActual, etDebugEvillib );

// init debugging structure
    etDebugInit( dbgInfo );
    dbgInfo->Program = "evillib";

// test table functions
    etjDBTestTable();
    etjDBTestColumn();
    //etjDBTestValues();
    //etjDBTestRelation();


    etjDBSQLiteConnection();
    //etjDBTestTableCreate();
    etjDBTestTableGet();
    //etjDBTestDataAdd();
    etjDBTestDataGet();
    etjDBTestDataChange();

	return etID_YES;
}


