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
#include "string/etString.h"
#include "string/etStringChar.h"

#include "dbdriver/etDBDriver.h"
#include "dbdriver/etDBSQL.h"
#include "dbdriver/etDBSQLite.h"
#include "db/etDBColumn.h"




// intarnal function definitions




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

    return etID_YES;
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

    return etID_YES;
}



// private stuff
etID_STATE          etDBSQLiteIsConnected( etDBDriver *dbDriver ){
// check
    etDebugCheckNull( dbDriver );

// get driver-data
    etDBSQLiteDriver *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;

//
    if( sqliteDriver->sqliteHandle != NULL ) return etID_YES;

    return etID_NO;
}






etID_STATE          etDBSQLiteRun( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbDriver->dbDriverData );


// get the sqlite driver data
    etDBSQLiteDriver *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;

// call callback
    if( dbDriver->queryAcknowledge != NULL ){
        if( dbDriver->queryAcknowledge( dbDriver, dbTable, sqliteDriver->sqlquery ) != etID_YES ){

        // show rejected query
            const char *query;
            etStringCharGet( sqliteDriver->sqlquery, query );
            snprintf( etDebugTempMessage, etDebugTempMessageLen, "Rejected query: %s", query );
            etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );

        // return
            return etID_NO;
        }
    }

// reset if already present
    if( sqliteDriver->sqliteStatement != NULL ){
		sqlite3_finalize( sqliteDriver->sqliteStatement );
		sqliteDriver->sqliteStatement = NULL;
    }

// prepare sql
    const char *query;
    etStringCharGet( sqliteDriver->sqlquery, query );

// debug message
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "Run query: %s", query );
    etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );

    sqliteDriver->sqliteState = sqlite3_prepare( sqliteDriver->sqliteHandle, query, -1, &sqliteDriver->sqliteStatement, NULL );
    if( sqliteDriver->sqliteState != SQLITE_OK ){
    // print error
        const char *sqliteErrorMessage = sqlite3_errmsg( sqliteDriver->sqliteHandle );
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%s", sqliteErrorMessage );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return etID_STATE_ERR_INTERR;
    }



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




etID_STATE          etDBSQLiteQueryExecute( etDBDriver* dbDriver, etDBTable* dbTable, const char* sqlCommand ){
// check
    etDebugCheckNull( dbDriver );

// vars
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
	etID_STATE          returnState = etID_STATE_NOTHING;



// set query
	etStringCharSet( sqliteDriver->sqlquery, sqlCommand, -1 );

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbTable );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}




etID_STATE          etDBSQLiteTableExists( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;

// the table Name
    const char* tableName = NULL;
    etDBTableGetName( dbTable, tableName );

// clear
    etStringClean( sqliteDriver->sqlquery );

// create table
    etStringCharSet( sqliteDriver->sqlquery, "SELECT name FROM sqlite_master WHERE type='table' AND name='", -1 );
    etStringCharAdd( sqliteDriver->sqlquery, tableName );
    etStringCharAdd( sqliteDriver->sqlquery, "';" );

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbTable );
    if( returnState != etID_YES ){
        return etID_NO;
    }

// is there a row ?
    if( sqliteDriver->sqliteState == SQLITE_ROW ){
    // clean up
        sqlite3_reset( sqliteDriver->sqliteStatement );
        sqliteDriver->sqliteStatement = NULL;
    // return
        return etID_YES;
    }

    return etID_NO;
}


etID_STATE          etDBSQLiteTableAdd( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbDriver->dbDriverData );

// only create a table if needed
	if( etDBSQLiteTableExists( dbDriver, dbTable )  == etID_YES ){
		return etID_STATE_DATA_AVIABLE;
	}


// vars
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;


// create the query
    if( etDBSQLAddTable( dbDriver, dbTable, sqliteDriver->sqlquery, "'" ) != etID_YES ) return etID_STATE_ERR_INTERR;

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbTable );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}


etID_STATE          etDBSQLiteTableRemove( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;

// create the query
    if( etDBSQLRemoveTable( dbDriver, dbTable, sqliteDriver->sqlquery ) != etID_YES ) return etID_STATE_ERR_INTERR;

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbTable );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}


etID_STATE          etDBSQLiteTableList( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbDriver->dbDriverData );

// vars
    etDBSQLiteDriver*	sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;

// the dbTable should contain the column tableName
	if( etDBColumnSelect( dbTable, "tableName" ) != etID_YES ){
		etDBColumnAppend( dbTable, "tableName", etDBCOLUMN_TYPE_STRING, etDBCOLUMN_OPTION_NOTHING );
	}


// the query
    etStringClean( sqliteDriver->sqlquery );
    etStringCharSet( sqliteDriver->sqlquery, "SELECT name AS tableName FROM sqlite_master WHERE type='table'", -1 );

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbTable );
    if( returnState != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// is there a row ?
    if( sqliteDriver->sqliteState == SQLITE_ROW ){
        return etID_YES;
    }

    return etID_NO;
}




etID_STATE          etDBSQLiteColumnExist( etDBDriver* dbDriver, etDBTable* dbTable, const char* columnName ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbDriver->dbDriverData );

// get the sqlite driver data
    etDBSQLiteDriver*	sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
	etID_STATE          returnState = etID_STATE_NOTHING;

// get table name
    const char *tableName = NULL;
    etDBTableGetName( dbTable, tableName );

// build the query
	etStringCharSet( sqliteDriver->sqlquery, "SELECT `", -1 );
	etStringCharAdd( sqliteDriver->sqlquery, columnName );
	etStringCharAdd( sqliteDriver->sqlquery, "` FROM '" );
	etStringCharAdd( sqliteDriver->sqlquery, tableName );
	etStringCharAdd( sqliteDriver->sqlquery, "' LIMIT 1;" );

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbTable );
    if( returnState != etID_YES ){
        return etID_NO;
    }
/*
// is there a row ?
    if( sqliteDriver->sqliteState == SQLITE_ROW ){
    // clean up
        sqlite3_reset( sqliteDriver->sqliteStatement );
        sqliteDriver->sqliteStatement = NULL;
    // return
        return etID_YES;
    }
*/
// return no
	return etID_YES;
}


etID_STATE          etDBSQLiteColumnAdd( etDBDriver* dbDriver, etDBTable* dbTable, const char* columnName ){

// if column exist return etID_YES
	if( etDBSQLiteColumnExist( dbDriver, dbTable, columnName ) == etID_YES ){
		return etID_YES;
	}

// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;

// create the query
    if( etDBSQLAddColumn( dbDriver, dbTable, columnName, sqliteDriver->sqlquery, "'" ) != etID_YES ) return etID_STATE_ERR_INTERR;

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbTable );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}


etID_STATE          etDBSQLiteColumnRemove( etDBDriver *dbDriver, etDBTable* dbTable, const char* columnName ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );



// the driver
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;

// clear
    etStringClean( sqliteDriver->sqlquery );

// create table
    etStringCharSet( sqliteDriver->sqlquery, "BEGIN TRANSACTION; ", -1 );

// get the old name
/*


CREATE TABLE t1_backup( 'postalcode' INTEGER,'uuid' TEXT  UNIQUE,'displayName' TEXT );
INSERT INTO t1_backup SELECT postalcode,uuid,displayName FROM city;
DROP TABLE city;
ALTER TABLE t1_backup RENAME TO city
*/

    return etID_STATE_ERR;
}




etID_STATE          etDBSQLiteDataAdd( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;


// create the query
    if( etDBSQLAddData( dbDriver, dbTable, sqliteDriver->sqlquery ) != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbTable );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}


etID_STATE          etDBSQLiteDataChange( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;


// create the query
    if( etDBSQLChangeData( dbDriver, dbTable, sqliteDriver->sqlquery ) != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbTable );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}


etID_STATE          etDBSQLiteDataRemove( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;


// create the query
    if( etDBSQLRemoveData( dbDriver, dbTable, sqliteDriver->sqlquery, "\"" ) != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbTable );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}


etID_STATE          etDBSQLiteDataGet( etDBDriver* dbDriver, etDBTable* dbTable, etDBFilter* dbFilter ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbDriver->dbDriverData );

// vars
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;


// create the query
    etDBSQLGetData( dbDriver, dbTable, dbFilter, sqliteDriver->sqlquery );

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbTable );
    if( returnState != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// is there a row ?
    if( sqliteDriver->sqliteState == SQLITE_ROW ){
        return etID_YES;
    }

    return etID_NO;
}


etID_STATE          etDBSQLiteDataGetWithLimit( etDBDriver* dbDriver, etDBTable* dbTable, int start, int amount, etDBFilter* dbFilter ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbDriver->dbDriverData );

// vars
    etDBSQLiteDriver    *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;


// create the query
    etDBSQLGetDataWithOffset( dbDriver, dbTable, start, amount, dbFilter, sqliteDriver->sqlquery );

// run the query
    returnState = etDBSQLiteRun( dbDriver, dbTable );
    if( returnState != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// is there a row ?
    if( sqliteDriver->sqliteState == SQLITE_ROW ){
        return etID_YES;
    }

    return etID_NO;
}





etID_STATE          etDBSQLiteDataNext( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbDriver->dbDriverData );

// vars
    etDBSQLiteDriver*   sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;


// iterate
    if( sqliteDriver->sqliteState == SQLITE_ROW ){

    // iterate result columns
        int columnCount = sqlite3_data_count( sqliteDriver->sqliteStatement );
        for( int columnIndex = 0; columnIndex < columnCount; columnIndex++ ){

            const char* columnName = sqlite3_column_name( sqliteDriver->sqliteStatement, columnIndex );
            const char* columnValue = (const char*)sqlite3_column_text( sqliteDriver->sqliteStatement, columnIndex );
			int columnType = sqlite3_column_type( sqliteDriver->sqliteStatement, columnIndex );

            if( columnValue == NULL ) columnValue = ""; // the "NULL" char inside sqlite

        // get the column and set the value
            if( etDBColumnSelect( dbTable, columnName ) != etID_YES ){

				etDBColumnType newColumnType = etDBCOLUMN_TYPE_NOTHING;
				if( columnType == SQLITE_INTEGER ) newColumnType = etDBCOLUMN_TYPE_INT;
				if( columnType == SQLITE_FLOAT ) newColumnType = etDBCOLUMN_TYPE_FLOAT;
				if( columnType == SQLITE_TEXT ) newColumnType = etDBCOLUMN_TYPE_STRING;
				if( columnType == SQLITE_BLOB ) newColumnType = etDBCOLUMN_TYPE_BLOB;

				etDBColumnAppend( dbTable, columnName, newColumnType, etDBCOLUMN_OPTION_NOTHING );

			// try again to select it
				if( etDBColumnSelect( dbTable, columnName ) != etID_YES ){
					continue;
				}
			}

		//finally set the value
			etDBColumnSetValue( dbTable, columnValue );

        }

    // next result
        sqliteDriver->sqliteState = sqlite3_step(sqliteDriver->sqliteStatement);
        return etID_YES;
    }

// finished ?
	if( sqliteDriver->sqliteState == SQLITE_DONE ){
		sqlite3_finalize( sqliteDriver->sqliteStatement );
		sqliteDriver->sqliteStatement = NULL;
	}

    return etID_NO;
}


etID_STATE 			etDBSQLiteDriverDestroy( etDBDriver *dbDriver ){
// check
    etDebugCheckNull(dbDriver);

// get our driver-data
	etDBSQLiteDriver *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;

// free some stuff
	etStringFree( sqliteDriver->sqlquery );

// reset if already present
    if( sqliteDriver->sqliteStatement != NULL ){
        sqlite3_finalize( sqliteDriver->sqliteStatement );
    }

// close handle
    sqlite3_close(sqliteDriver->sqliteHandle);
    sqliteDriver->sqliteHandle = NULL;

// free driver-data
	etMemoryRelease( sqliteDriver );

	return etID_YES;
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

    dbDriver->connect = NULL;
    dbDriver->isConnected = etDBSQLiteIsConnected;

	dbDriver->queryExecute = etDBSQLiteQueryExecute;

    dbDriver->tableAdd = etDBSQLiteTableAdd;
    dbDriver->tableRemove = etDBSQLiteTableRemove;
    dbDriver->tableExists = etDBSQLiteTableExists;
	dbDriver->tableList = etDBSQLiteTableList;

    dbDriver->columnAdd = etDBSQLiteColumnAdd;
    dbDriver->columnRemove = NULL;

    dbDriver->dataAdd = etDBSQLiteDataAdd;
    dbDriver->dataChange = etDBSQLiteDataChange;
    dbDriver->dataRemove = etDBSQLiteDataRemove;
    dbDriver->dataGet = etDBSQLiteDataGet;
    dbDriver->dataGetWithLimit = etDBSQLiteDataGetWithLimit;
    dbDriver->dataNext = etDBSQLiteDataNext;

	dbDriver->destroy = etDBSQLiteDriverDestroy;


// alloc data for driver
    etMemoryAlloc( dbDriver->dbDriverData, sizeof(etDBSQLiteDriver) );
    etDBSQLiteDriver *sqliteDriver = (etDBSQLiteDriver*)dbDriver->dbDriverData;

// alloc query
    etStringAllocLen( sqliteDriver->sqlquery, 128 );

// try to open the sqlite connection
    int sqliteResult = sqlite3_open( filename, &sqliteDriver->sqliteHandle );
    if( sqliteResult == SQLITE_OK ){
		sqliteDriver->sqliteStatement = NULL;
		return etID_YES;
	}


// print error
    const char *sqliteErrorMessage = sqlite3_errmsg( sqliteDriver->sqliteHandle );
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "%s", sqliteErrorMessage );
    etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );

// close handle
    sqlite3_close(sqliteDriver->sqliteHandle);
    sqliteDriver->sqliteHandle = NULL;

// return
    return etID_STATE_ERR;
}


