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


// intarnal function definitions
etID_STATE          etDBSQLiteRun( etDBDriver *dbDriver, etDBObject *dbObject );



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


etID_STATE          etDBSQLiteTableAdd( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBSQLiteDriver *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;

// create the query
    if( etDBSQLTableCreate( dbDriver, dbObject, sqliteDriver->sqlquery ) != etID_YES ) return etID_STATE_ERR_INTERR;

// run the query
    etDBSQLiteRun( dbDriver, dbObject );

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
    int sqliteResult = sqlite3_prepare( sqliteDriver->sqliteHandle, query, -1, &sqliteDriver->sqliteStatement, NULL );
    if( sqliteResult != SQLITE_OK ){
    // print error
        const char *sqliteErrorMessage = sqlite3_errmsg( sqliteDriver->sqliteHandle );
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%s", sqliteErrorMessage );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return etID_STATE_ERR_INTERR;
    }

    snprintf( etDebugTempMessage, etDebugTempMessageLen, "Run query: %s", query );
    etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
    
    sqlite3_step(sqliteDriver->sqliteStatement);
    
}



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





