/*  Copyright (C) 2016 by Martin Langlotz alias stackshadow

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

#include "dbdriver/etDBDriver.h"
#include "dbdriver/etDBSQLite.h"
#include "db/etDBObjectTableColumn.h"
#include "db/etDBObjectValue.h"


// intarnal function definitions
etID_STATE          etDBSQLiteColumnTypeAdd( etString *sqlquery, etDBColumnType columnType );
etID_STATE          etDBSQLiteColumnOptionAdd( etString *sqlquery, int option );

etID_STATE          etDBSQLiteRun( etDBDriver *dbDriver, etDBObject *dbObject );
etID_STATE          etDBSQLiteTableAdd( etDBDriver *dbDriver, etDBObject *dbObject );
etID_STATE          etDBSQLiteDataAdd( etDBDriver *dbDriver, etDBObject *dbObject );
etID_STATE          etDBSQLiteDataGet( etDBDriver *dbDriver, etDBObject *dbObject );
etID_STATE          etDBSQLiteDataNext( etDBDriver *dbDriver, etDBObject *dbObject );




// userspace
etID_STATE          etDBSQLiteDriverInit( etDBDriver *dbDriver, const char *filename ){
// check
    etDebugCheckNull(dbDriver);
    etDebugCheckNull(filename);

// clean the driver
    memset( dbDriver, 0, sizeof(etDBDriver) );

// setup function pointer
    dbDriver->queryColumnTypeAdd = etDBSQLiteColumnTypeAdd;
    dbDriver->queryColumnOptionAdd = etDBSQLiteColumnOptionAdd;
    dbDriver->tableAdd = etDBSQLiteTableAdd;
    dbDriver->tableRemove = NULL;
    dbDriver->dataAdd = etDBSQLiteDataAdd;
    dbDriver->dataGet = etDBSQLiteDataGet;
    dbDriver->dataNext = etDBSQLiteDataNext;

// alloc data for driver
    etMemoryAlloc( dbDriver->dbDriverData, sizeof(etDBSQLiteDriver) );
    etDBSQLiteDriver *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;

// alloc query
    etStringAllocLen( sqliteDriver->sqlquery, 128 );

// try to open the sqlite connection
    int sqliteResult = sqlite3_open( filename, &sqliteDriver->sqliteHandle );
    if( sqliteResult == SQLITE_OK ) return etID_YES;


// print error
    const char *sqliteErrorMessage = sqlite3_errmsg( sqliteDriver->sqliteHandle );
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "%s", sqliteErrorMessage );
    etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );

// close handle
    sqlite3_close(sqliteDriver->sqliteHandle);

// return
    return etID_STATE_ERR;
}



// functions for function pointer
etID_STATE          etDBSQLiteColumnTypeAdd( etString *sqlquery, etDBColumnType columnType ){

    switch( columnType ){
        case etDBCOLUMN_TYPE_NOTHING:
            break;

        case etDBCOLUMN_TYPE_INT:
            etStringCharAdd( sqlquery, "INTEGER" );
            break;
        case etDBCOLUMN_TYPE_FLOAT:
            etStringCharAdd( sqlquery, "REAL" );
            break;
        case etDBCOLUMN_TYPE_STRING:
            etStringCharAdd( sqlquery, "TEXT" );
            break;
        case etDBCOLUMN_TYPE_BLOB:
            etStringCharAdd( sqlquery, "BLOB" );
            break;

        default:
            break;
    }

}


etID_STATE          etDBSQLiteColumnOptionAdd( etString *sqlquery, int option ){


    if( option & etDBCOLUMN_OPTION_PRIMARY ){
        etStringCharAdd( sqlquery, " PRIMARY KEY" );
    }

    if( option & etDBCOLUMN_OPTION_NOTNULL ){
        etStringCharAdd( sqlquery, " NOT NULL" );
    }

    if( option & etDBCOLUMN_OPTION_UNIQUE ){
        etStringCharAdd( sqlquery, " UNIQUE" );
    }
}



// private stuff
etID_STATE          etDBSQLiteRun( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBSQLiteDriver *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;

// reset if already present
    if( sqliteDriver->sqliteStatement != NULL ){
        sqlite3_reset( sqliteDriver->sqliteStatement );
    }

// prepare sql
    const char *query;
    etStringCharGet( sqliteDriver->sqlquery, query );
    sqliteDriver->sqliteState = sqlite3_prepare( sqliteDriver->sqliteHandle, query, -1, &sqliteDriver->sqliteStatement, NULL );
    if( sqliteDriver->sqliteState != SQLITE_OK ){
    // print error
        const char *sqliteErrorMessage = sqlite3_errmsg( sqliteDriver->sqliteHandle );
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%s", sqliteErrorMessage );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return etID_STATE_ERR_INTERR;
    }

    snprintf( etDebugTempMessage, etDebugTempMessageLen, "Run query: %s", query );
    etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
    
    sqliteDriver->sqliteState = sqlite3_step(sqliteDriver->sqliteStatement);

// everything okay
    if( sqliteDriver->sqliteState == SQLITE_OK 
    ||  sqliteDriver->sqliteState == SQLITE_DONE 
    ||  sqliteDriver->sqliteState == SQLITE_ROW ){
        return etID_YES;
    }

// there is an error
    const char *sqliteErrorMessage = sqlite3_errmsg( sqliteDriver->sqliteHandle );
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "%s", sqliteErrorMessage );
    etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );


    return etID_STATE_ERR;
}


etID_STATE          etDBSQLiteTableAdd( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;

// create the query
    if( etDBSQLTableCreate( dbDriver, dbObject, sqliteDriver->sqlquery ) != etID_YES ) return etID_STATE_ERR_INTERR;

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbObject );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}


etID_STATE          etDBSQLiteDataAdd( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;


// create the query
    etDBSQLInsertInto( dbDriver, dbObject, sqliteDriver->sqlquery );

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbObject );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}


etID_STATE          etDBSQLiteDataGet( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

// vars
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;


// create the query
    etDBSQLSelect( dbDriver, dbObject, sqliteDriver->sqlquery );

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbObject );

// is there a row ?
    if( sqliteDriver->sqliteState == SQLITE_ROW ){
        return etID_YES;
    }

    return etID_NO;
}


etID_STATE          etDBSQLiteDataNext( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

// vars
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;

// clean calues
    etDBObjectValueClean( dbObject );
    
// iterate
    if( sqliteDriver->sqliteState == SQLITE_ROW ){

    // iterate result columns
        int columnCount = sqlite3_data_count( sqliteDriver->sqliteStatement );
        for( int columnIndex = 0; columnIndex < columnCount; columnIndex++ ){
            
            const char* columnName = sqlite3_column_name( sqliteDriver->sqliteStatement, columnIndex );
            const char* columnValue = sqlite3_column_text( sqliteDriver->sqliteStatement, columnIndex );
            
            etDBObjectValueSet( dbObject, columnName, columnValue );
            
        }

    // next result
        sqliteDriver->sqliteState = sqlite3_step(sqliteDriver->sqliteStatement);
        return etID_YES;
    }

    return etID_NO;
}






