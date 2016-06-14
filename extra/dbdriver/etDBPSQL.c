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

#include "core/etIDState.h"
#include "string/etString.h"
#include "string/etStringChar.h"

#include "dbdriver/etDBDriver.h"
#include "dbdriver/etDBSQL.h"
#include "dbdriver/etDBPSQL.h"
#include "db/etDBObjectTableColumn.h"
#include "db/etDBObjectValue.h"



// intarnal function definitions
etID_STATE          etDBPSQLColumnTypeAdd( etString *sqlquery, etDBColumnType columnType );
etID_STATE          etDBPSQLColumnOptionAdd( etString *sqlquery, int option );

etID_STATE          etDBPSQLIsConnected( etDBDriver *dbDriver );
etID_STATE          etDBPSQLDisconnect( etDBDriver *dbDriver );

etID_STATE          etDBPSQLTableAdd( etDBDriver *dbDriver, etDBObject *dbObject );
etID_STATE          etDBPSQLTableRemove( etDBDriver *dbDriver, etDBObject *dbObject );

etID_STATE          etDBPSQLColumnAdd( etDBDriver *dbDriver, etDBObject *dbObject );

etID_STATE          etDBPSQLDataAdd( etDBDriver *dbDriver, etDBObject *dbObject );
etID_STATE          etDBPSQLDataChange( etDBDriver *dbDriver, etDBObject *dbObject );
etID_STATE          etDBPSQLDataRemove( etDBDriver *dbDriver, etDBObject *dbObject );
etID_STATE          etDBPSQLDataGet( etDBDriver *dbDriver, etDBObject *dbObject );
etID_STATE          etDBPSQLDataNext( etDBDriver *dbDriver, etDBObject *dbObject );


// functions for function pointer
etID_STATE          etDBPSQLColumnTypeAdd( etString *sqlquery, etDBColumnType columnType ){

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


etID_STATE          etDBPSQLColumnOptionAdd( etString *sqlquery, int option ){


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




// userspace
etID_STATE          etDBPSQLDriverInit( etDBDriver *dbDriver, const char *host, const char *hostaddr, const char *port, const char *username, const char *password, const char *database, const char *sslmode ){
// check
    etDebugCheckNull(dbDriver);


// clean the driver
    memset( dbDriver, 0, sizeof(etDBDriver) );


// setup function pointer
    dbDriver->queryColumnTypeAdd = etDBPSQLColumnTypeAdd;
    dbDriver->queryColumnOptionAdd = etDBPSQLColumnOptionAdd;

    dbDriver->isConnected = etDBPSQLIsConnected;
    dbDriver->disconnect = etDBPSQLDisconnect;

    dbDriver->tableAdd = etDBPSQLTableAdd;
    dbDriver->tableRemove = etDBPSQLTableRemove;

    dbDriver->columnAdd = etDBPSQLColumnAdd;

    dbDriver->dataAdd = etDBPSQLDataAdd;
    dbDriver->dataChange = etDBPSQLDataChange;
    dbDriver->dataRemove = etDBPSQLDataRemove;
    dbDriver->dataGet = etDBPSQLDataGet;
    dbDriver->dataNext = etDBPSQLDataNext;



// alloc data for driver
    etMemoryAlloc( dbDriver->dbDriverData, sizeof(etDBPSQLDriver) );
    etDBPSQLDriver *psqlDriver = (etDBPSQLDriver*)dbDriver->dbDriverData;

// alloc query
    etStringAllocLen( psqlDriver->sqlquery, 128 );




// parameter
    if( sslmode == NULL ) sslmode = "prefer";

    const char *keywords[] = {
        "host",
        "hostaddr",
        "port",
        "user",
        "password",
        "dbname",
        "connect_timeout",
        "sslmode",
        NULL
    };

    const char *values[] = {
        host,
        hostaddr,
        port,
        username,
        password,
        database,
        "30",
        sslmode,// "prefer",
        NULL
    };

// connect
    psqlDriver->conn = PQconnectdbParams( keywords, values, 0 );

// connection status
    ConnStatusType connectionStatus = PQstatus(psqlDriver->conn);
    if( connectionStatus != CONNECTION_OK ){

    // error Message
        char *pgerror = PQerrorMessage(psqlDriver->conn);
        etDebugMessage( etID_LEVEL_ERR, pgerror );

    // release memory
        PQfinish( psqlDriver->conn );


        psqlDriver->conn = NULL;
        return etID_STATE_ERR;
    }

// server-version
    int serverVersion = PQserverVersion(psqlDriver->conn);
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "Postresql server version: %i", serverVersion );
    etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );


// ssl ?
    if( PQsslInUse(psqlDriver->conn) == 1 ){
        const char *sslProtocol = PQsslAttribute(psqlDriver->conn, "protocol" );
        const char *sslCipher = PQsslAttribute(psqlDriver->conn, "cipher" );
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Cool we use ssl with protocoll: %s and cipher %s", sslProtocol, sslCipher );
        etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
    }

// return
    return etID_YES;
}

// private stuff
etID_STATE          etDBPSQLIsConnected( etDBDriver *dbDriver ){
// check
    etDebugCheckNull( dbDriver );

// get driver-data
    etDBPSQLDriver *psqlDriver = (etDBPSQLDriver*)dbDriver->dbDriverData;

// already null
    if( psqlDriver->conn == NULL ) return etID_NO;

// connection status
    ConnStatusType connectionStatus = PQstatus(psqlDriver->conn);
    if( connectionStatus != CONNECTION_OK ){

    // release memory
        PQfinish( psqlDriver->conn );

        psqlDriver->conn = NULL;
        return etID_NO;
    }

    return etID_YES;
}


etID_STATE          etDBPSQLDisconnect( etDBDriver *dbDriver ){
// check
    etDebugCheckNull( dbDriver );

    etDBPSQLDriver *psqlDriver = (etDBPSQLDriver*)dbDriver->dbDriverData;

// already disconnected ?
    if( psqlDriver->conn == NULL ) return etID_YES;

// disconnect
    PQfinish( psqlDriver->conn );
    psqlDriver->conn = NULL;

// free the query
    etStringFree( psqlDriver->sqlquery );

    return etID_YES;
}



etID_STATE          etDBPSQLRun( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );


// get the sqlite driver data
    etDBPSQLDriver*     psqlDriver = (etDBPSQLDriver*)dbDriver->dbDriverData;

// call callback
    if( dbDriver->queryAcknowledge != NULL ){
        if( dbDriver->queryAcknowledge( dbDriver, dbObject, psqlDriver->sqlquery ) != etID_YES ){

        // show rejected query
            const char *query;
            etStringCharGet( psqlDriver->sqlquery, query );
            snprintf( etDebugTempMessage, etDebugTempMessageLen, "Rejected query: %s", query );
            etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );

        // return
            return etID_NO;
        }
    }

// reset if already present
    if( psqlDriver->result != NULL ){
        PQclear( psqlDriver->result );
    }

// prepare sql
    const char *query;
    etStringCharGet( psqlDriver->sqlquery, query );

// debug message
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "Run query: %s", query );
    etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );

// execute
    psqlDriver->result = PQexec( psqlDriver->conn, query );

// check return state
    ExecStatusType resultStatus = PQresultStatus(psqlDriver->result);
    if( resultStatus != PGRES_COMMAND_OK && resultStatus != PGRES_TUPLES_OK ){

        const char *errorMessage = PQresultErrorMessage( psqlDriver->result );

        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%s", errorMessage );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return etID_STATE_ERR_INTERR;
    }

// print out ok
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "Query was okay" );
    etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );

    return etID_YES;
}




etID_STATE          etDBPSQLTableAdd( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBPSQLDriver*     psqlDriver = (etDBPSQLDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;

// create the query
    if( etDBSQLTableCreate( dbDriver, dbObject, psqlDriver->sqlquery, "\"" ) != etID_YES ) return etID_STATE_ERR_INTERR;

// run the query
    returnState = etDBPSQLRun( dbDriver, dbObject );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}


etID_STATE          etDBPSQLTableRemove( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBPSQLDriver*     psqlDriver = (etDBPSQLDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;

// create the query
    if( etDBSQLTableRemove( dbDriver, dbObject, psqlDriver->sqlquery ) != etID_YES ) return etID_STATE_ERR_INTERR;

// run the query
    returnState = etDBPSQLRun( dbDriver, dbObject );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}




etID_STATE          etDBPSQLColumnAdd( etDBDriver *dbDriver, etDBObject *dbObject ){

// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBPSQLDriver*     psqlDriver = (etDBPSQLDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;

// create the query
    if( etDBSQLColumnAdd( dbDriver, dbObject, psqlDriver->sqlquery, "\"" ) != etID_YES ) return etID_STATE_ERR_INTERR;

// run the query
    returnState = etDBPSQLRun( dbDriver, dbObject );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}







etID_STATE          etDBPSQLDataAdd( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBPSQLDriver*     psqlDriver = (etDBPSQLDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;


// create the query
    if( etDBSQLDataAdd( dbDriver, dbObject, psqlDriver->sqlquery ) != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// run the query
    returnState = etDBPSQLRun( dbDriver, dbObject );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}


etID_STATE          etDBPSQLDataChange( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBPSQLDriver*     psqlDriver = (etDBPSQLDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;


// create the query
    if( etDBSQLDataChange( dbDriver, dbObject, psqlDriver->sqlquery ) != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// run the query
    returnState = etDBPSQLRun( dbDriver, dbObject );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}


etID_STATE          etDBPSQLDataRemove( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

//
    etDBPSQLDriver*     psqlDriver = (etDBPSQLDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;


// create the query
    if( etDBSQLDataRemove( dbDriver, dbObject, psqlDriver->sqlquery, "\"" ) != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// run the query
    returnState = etDBPSQLRun( dbDriver, dbObject );
    if( returnState == etID_YES ){
        return etID_YES;
    }

// return
    return etID_NO;
}


etID_STATE          etDBPSQLDataGet( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

// vars
    etDBPSQLDriver*     psqlDriver = (etDBPSQLDriver*)dbDriver->dbDriverData;
    etID_STATE          returnState = etID_STATE_NOTHING;


// create the query
    etDBSQLSelect( dbDriver, dbObject, psqlDriver->sqlquery );

// run the query
    returnState = etDBPSQLRun( dbDriver, dbObject );
    if( returnState != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// check return state
    ExecStatusType resultStatus = PQresultStatus(psqlDriver->result);
    if( resultStatus == PGRES_TUPLES_OK ){

    // count the results
        psqlDriver->resultRowCount = PQntuples( psqlDriver->result );
        psqlDriver->resultRowIndex = -1;

        if( psqlDriver->resultRowCount == 0 ) return etID_NO;
        return etID_YES;
    }

    return etID_NO;
}


etID_STATE          etDBPSQLDataNext( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( dbDriver->dbDriverData );

// vars
    etDBPSQLDriver*     psqlDriver = (etDBPSQLDriver*)dbDriver->dbDriverData;

// clean calues
    etDBObjectValueClean( dbObject );

// iterate
    psqlDriver->resultRowIndex++;

// iterate
    if( psqlDriver->resultRowIndex < psqlDriver->resultRowCount ){

    // iterate result columns
        int columnCount = PQnfields( psqlDriver->result );
        for( int columnIndex = 0; columnIndex < columnCount; columnIndex++ ){

            const char* columnName = PQfname( psqlDriver->result, columnIndex );
            const char* columnValue = PQgetvalue( psqlDriver->result, psqlDriver->resultRowIndex, columnIndex );

            if( columnValue == NULL ) columnValue = ""; // the "NULL" char inside sqlite

            etDBObjectValueSet( dbObject, columnName, columnValue );

        }

        return etID_YES;
    }

    return etID_NO;
}













