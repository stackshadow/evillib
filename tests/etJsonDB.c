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
#include "core/etObject.h"
#include "etApicheck.h"


#include "memory/etMemoryBlock.h"
#include "memory/etMemory.h"
#include "string/etString.h"
#include "string/etStringChar.h"

#include "etDBObject.h"
#include "etDBValue.h"
#include "etDBFilter.h"
#include "etDB.h"

#ifdef ET_SINGLEMODULE
    #include "core/etObject.h"

    #include "etDB.c"
    #include "etDBObject.c"
    #include "etDBColumn.c"


//    #include "etDBValue.c"
//    #include "etDBFilter.c"
//    #include "etDBRelations.c"
    
//    #include "etDBSQLite.c"
#endif


// Vars
etDebug             dbgInfo_static;
etDebug*            dbgInfo = &dbgInfo_static;
etDB*               etDBActual = NULL;
etDBObject*          dbTableUsers = NULL;
etDBObject*          dbTableCity = NULL;
etDBValue*          dbValues = NULL;


void                etjDBTestTableAlloc(){
    etDebugCaller
    
// Append tables
    etDBObjectAlloc( dbTableUsers );
    
    etDebugStateToMessage( etID_STATE_NODATA, "", 0 );
    
    etDBObjectNameSet( dbTableUsers, "users" );
    etDBObjectVisibleNameSet( dbTableUsers, "UserAccounts" );

    etDBObjectAlloc( dbTableCity );
    etDBObjectNameSet( dbTableCity, "city" );

    etDBObjectDumpf( dbTableUsers );
    etDBObjectDumpf( dbTableCity );
}
void                etjDBTestTable(){
    etDebugCaller

// Vars
    const char *tableName = NULL;
    const char *tableNameVisible = NULL;

// check name
    etDBObjectName( dbTableUsers, tableName );
     if( tableName == NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBObjectNameGet not work as expected" );
    if( strncmp( tableName, "users", 5 ) != 0 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "tableName is wrong" );
    }

// is there an old name ?
    if( etDBObjectNameOldExist( dbTableUsers ) != etID_NO )
        etDebugMessage( etID_LEVEL_CRITICAL, "etDBObjectHasNameOld not work as expected" );

// there should be no old name
    etDBObjectNameOld( dbTableUsers, tableName );
    if( tableName != NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBObjectNameOld not work as expected" );

// reset the object state
    etObjectStateSet( dbTableUsers, etID_YES );

// now there is an old name
    etDBObjectNameSet( dbTableUsers, "clients" );
    etDBObjectNameOld( dbTableUsers, tableName );
    if( tableName == NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBObjectNameOld not work as expected" );
    if( strncmp( tableName, "users", 5 ) != 0 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "tableName is wrong" );
    }

// check new name
    etDBObjectName( dbTableUsers, tableName );
     if( tableName == NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBObjectNameGet not work as expected" );
    if( strncmp( tableName, "clients", 7 ) != 0 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "tableName is wrong" );
    }


    etDBObjectVisibleName( dbTableUsers, tableNameVisible );
    if( tableNameVisible == NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBObjectNameGet not work as expected" );
    if( strncmp( tableNameVisible, "UserAccounts", 12 ) != 0 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "tableNameVisible is wrong" );
    } 

    etDBObjectDumpf( dbTableUsers );
}
void                etjDBTestTableFree(){
    etDebugCaller
    etDBObjectFree( dbTableUsers );
    etDBObjectFree( dbTableCity );
}



void                etjDBTestColumn(){
    etDebugCaller
    
    const char *columnName;
    int tempState;
    
// Add some columns
    etDBColumnNameSet( dbTableUsers, "userID" );
    etDBColumnNameSet( dbTableUsers, "prename" );
    etDBColumnNameSet( dbTableUsers, "name" );
    etDBColumnNameSet( dbTableUsers, "year" );

// check if a column exist
    if( etDBColumnExist( dbTableUsers, "familyname" ) != etID_NO ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "etDBColumnExist find a column which not exist");
    }
    if( etDBColumnExist( dbTableUsers, "name" ) != etID_YES ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "etDBColumnExist dont found a column which should exist");
    }

// go to a column, which not exist
    tempState = etDBColumnSeek( dbTableUsers, "familyname" );
    if( tempState != etID_STATE_NODATA ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "etDBColumnSeek return a column wich not exist !");
    }

// reset the object state
    etObjectStateSet( dbTableUsers, etID_YES );

// go to a column and check name
    etDBColumnSeek( dbTableUsers, "prename" );
    etDBColumnName( dbTableUsers, columnName );
     if( columnName == NULL ) etDebugMessage( etID_LEVEL_CRITICAL, "etDBColumnName not work as expected" );
    if( strncmp( columnName, "prename", 7 ) != 0 ){
        etDebugMessage( etID_LEVEL_CRITICAL, "etDBColumnName dont return the correct column" );
    }

// there should be no old name
    if( etDBColumnNameOldExist( dbTableUsers ) != etID_NO ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "etDBColumnNameOldExist return that a old name exist, but it should not !");
    }

// now there exist an old name
    etDBColumnNameSet( dbTableUsers, "pname" );
    if( etDBColumnNameOldExist( dbTableUsers ) != etID_YES ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "etDBColumnNameOldExist return that no old name exist, but there should be one !");
    }



}



/*
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
    etDBRelationAlloc( dbRelation );

// append some relation
    etDBRelationAppend( dbRelation, "tablea", "columna", "partnertable", "partnercolumn" );
    etDBRelationAppend( dbRelation, "tablec", "columnc", "tabled", "columnd" );

// vars
    const char*         columnName;
    const char*         partnerTableName;
    const char*         partnerColumnName;


// reset the relation indexer
    etDBRelationsIndexReset( dbRelation );
    while( etDBRelationFindNext(dbRelation,"tablea",columnName,partnerTableName,partnerColumnName) == etID_YES ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Found Table: %s", partnerTableName );
        etDebugMessage( etID_LEVEL_TEST, etDebugTempMessage );
    }


    etDBRelationsIndexReset( dbRelation );
    while( etDBRelationFindNext(dbRelation,"tablec",columnName,partnerTableName,partnerColumnName) == etID_YES ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Found Table: %s", partnerTableName );
        etDebugMessage( etID_LEVEL_TEST, etDebugTempMessage );
    }

// Try to get an explicit table to table relation
    etDBRelationsIndexReset( dbRelation );
    while( etDBRelationFindNextTable(dbRelation,"tablec","tabled",columnName,partnerColumnName) == etID_YES ){
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Found Table with column: %s <-> %s", columnName, partnerColumnName );
        etDebugMessage( etID_LEVEL_TEST, etDebugTempMessage );
    }

}

// SQLITE-stuff
void                etjDBSQLiteConnection(){
// With sqlite
    etDBSQLiteInit( etDBActual );

// connect to an impossible location
    etDBSQLiteConnect( etDBActual, "/tmp/evillib/tmp/db.sqlite" );

    etDBValueAlloc( dbValues );

}
void                etjDBTestTableCreate(){
    etDBObjectDumpf( dbTableUsers );
    
    etDBActionSet( etDBActual, etDB_ACTION_TABLE_ADD );
    etDBRun( etDBActual, dbTableUsers, NULL, NULL );
    
    
}
void                etjDBTestTableGet(){
// we reset there the debug-state

    
    etDBActionSet( etDBActual, etDB_ACTION_TABLELIST_GET );
    etDBRun( etDBActual, NULL, NULL, NULL );

    while( etDBNextResult( etDBActual, dbValues ) == etID_YES ){
        
        const char *tableName = NULL;
        etDBValueGet( dbValues, "table", tableName );
        
        etDebugMessage( etID_LEVEL_DETAIL_DB, tableName );
    }
    
// now we get an specific table
    etDBActionSet( etDBActual, etDB_ACTION_TABLE_GET );
    etDBRun( etDBActual, dbTableCity, NULL, NULL );
// this table not exist inside the db, so we need to reset the faulty state

    etDBObjectDumpf( dbTableCity );

// get the real table
// clear it before
    etDBObjectFree( dbTableUsers );

// allocate the dbTableUsers
    etDBObjectAlloc( dbTableUsers );
// and we need to know the name
    etDBObjectNameSet( dbTableUsers, "users" );

    etDBActionSet( etDBActual, etDB_ACTION_TABLE_GET );
    etDBRun( etDBActual, dbTableUsers, NULL, NULL );
    etDBObjectDumpf( dbTableUsers );
}

void                etjDBTestDataAdd(){
    
    etDBValuesClean( dbValues );
    
    etDBValueSet( dbValues, "userID", "9c8910c2-232f-11e5-8b54-34e6d7289d44" );
    etDBValueSet( dbValues, "prename", "Homer" );
    
    etDBActionSet( etDBActual, etDB_ACTION_DATA_ADD );
    etDBRun( etDBActual, dbTableUsers, NULL, dbValues );



    etDBValuesClean( dbValues );
    
    etDBValueSet( dbValues, "userID", "97515fc0-3111-11e5-bd17-34e6d7289d44" );
    etDBValueSet( dbValues, "prename", "Sid" );
    etDBValueSet( dbValues, "year", "2015" );
    
    etDBActionSet( etDBActual, etDB_ACTION_DATA_ADD );
    etDBRun( etDBActual, dbTableUsers, NULL, dbValues );



    etDBValuesClean( dbValues );
    
    etDBValueSet( dbValues, "userID", "9aefc09a-3111-11e5-a8e8-34e6d7289d44" );
    etDBValueSet( dbValues, "prename", "Sid" );
    
    etDBActionSet( etDBActual, etDB_ACTION_DATA_ADD );
    etDBRun( etDBActual, dbTableUsers, NULL, dbValues );
    
}
void                etjDBTestDataGet(){
    

// clean the values
    etDBValuesClean( dbValues );

    etDBActionSet( etDBActual, etDB_ACTION_DATA_GET );
    etDBRun( etDBActual, dbTableUsers, NULL, NULL );

// we need a new filter
    etDBFilter*     dbFilter = NULL;
    etDBFilterAlloc( dbFilter );
    etDBFilterAppend( dbFilter, etID_TYPE_AND, 1, "prename", etDB_FILTER_EQUAL, "Homer" );
    etDBFilterAppend( dbFilter, etID_TYPE_AND, 2, "name", etDB_FILTER_ISNULL, NULL );
    etDBFilterAppend( dbFilter, etID_TYPE_AND, 2, "year", etDB_FILTER_ISNULL, NULL );

    etDBValuesClean( dbValues );
    

    etDBActionSet( etDBActual, etDB_ACTION_DATA_GET );
    etDBRun( etDBActual, dbTableUsers, dbFilter, NULL );
}
void                etjDBTestDataChange(){
    

// clean the values
    etDBValuesClean( dbValues );

// we need a new filter
    etDBFilter*     dbFilter = NULL;
    etDBFilterAlloc( dbFilter );
    etDBFilterAppend( dbFilter, etID_TYPE_AND, 1, "prename", etDB_FILTER_EQUAL, "Homer" );
    etDBFilterAppend( dbFilter, etID_TYPE_AND, 2, "name", etDB_FILTER_ISNULL, NULL );
    etDBFilterAppend( dbFilter, etID_TYPE_AND, 2, "year", etDB_FILTER_ISNULL, NULL );

    etDBActionSet( etDBActual, etDB_ACTION_DATA_GET );
    etDBRun( etDBActual, dbTableUsers, dbFilter, NULL );


    etDBValuesClean( dbValues );
    if( etDBNextResult( etDBActual, dbValues ) != etID_YES ){
        return etDebugMessage( etID_LEVEL_CRITICAL, "There should be an result inside the db");
    }

// Change Homer to Marge
    etDBValueSet( dbValues, "prename", "Marge" );
    etDBActionSet( etDBActual, etDB_ACTION_DATA_CHANGE );
    etDBRun( etDBActual, dbTableUsers, dbFilter, dbValues );
}

*/


etID_STATE          etjDBTest(){


// Allocate the etDB-Object, this we need for all etDB-Functions !
    //etDBAlloc( etDBActual );
    etDebugLevelSet( etID_LEVEL_ALL );

// init debugging structure
    dbgInfo->Program = "evillib";

// test table functions
    etjDBTestTableAlloc();
    etjDBTestTable();
    etjDBTestTableFree();

// test columns
    etjDBTestTableAlloc();
    etjDBTestColumn();
    etjDBTestTableFree();


    //etjDBTestValues();
    //etjDBTestRelation();


    //etjDBSQLiteConnection();
    //etjDBTestTableCreate();
    //etjDBTestTableGet();
    //etjDBTestDataAdd();
    //etjDBTestDataGet();
    //etjDBTestDataChange();

    //etDBFree( etDBActual );
    return etID_YES;
}


