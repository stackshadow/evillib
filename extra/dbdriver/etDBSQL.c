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
#include "db/etDBObject.h"
#include "db/etDBObjectTable.h"
#include "db/etDBObjectTableColumn.h"

etID_STATE          etDBSQLTableCreate( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( sqlquery );

// check if an table is selected
    if( dbObject->jsonTableActual == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table" );
        return etID_STATE_WARN_SEQERR;
    }

// clear
    etStringClean( sqlquery );

// create table
    etStringCharAdd( sqlquery, "CREATE TABLE " );
    
// add table name
    const char *tableName = NULL;
    etDBObjectTableNameGet( dbObject, tableName );
    etStringCharAdd( sqlquery, tableName );

// columns
    etID_BOOL           firstColumn = etID_TRUE;
    const char          *columnName = NULL;
    etDBColumnType      columnType;
    int                 columnOption = etDBCOLUMN_OPTION_NOTHING;
    
    etStringCharAdd( sqlquery, "( " );
    etDBObjectIterationReset(dbObject);
    while( etDBObjectTableColumnNext(dbObject) == etID_YES ){
        
    // comma
        if( firstColumn == etID_FALSE ){
            etStringCharAdd( sqlquery, "," );
        }

    // column name
        if( etDBObjectTableColumnNameGet( dbObject, columnName ) != etID_YES ) return etID_STATE_ERR;
        etStringCharAdd( sqlquery, columnName );
        etStringCharAdd( sqlquery, " " );
        
    // column type
        etDBObjectTableColumnTypeGet( dbObject, columnType );
        if( dbDriver->queryColumnTypeAdd != NULL ){
            dbDriver->queryColumnTypeAdd( sqlquery, columnType );
        }
        
    // get column specials ( like primary key )
        etDBObjectTableColumnOptionGet( dbObject, columnOption );
        if( dbDriver->queryColumnOptionAdd != NULL ){
            dbDriver->queryColumnOptionAdd( sqlquery, columnOption );
        }
        
        
        firstColumn = etID_FALSE;
    }
    
    etStringCharAdd( sqlquery, ");" );
}


etID_STATE          etDBSQLInsertInto( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( sqlquery );



/*
INSERT INTO table_name (column1,column2,column3,...)
VALUES (value1,value2,value3,...);
*/

}


etID_STATE          etDBSQLSelect( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( sqlquery );


// check if an table is selected
    if( dbObject->jsonTableActual == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table" );
        return etID_STATE_WARN_SEQERR;
    }

// vars
    etID_BOOL           firstColumn = etID_TRUE;
    const char          *tableName = NULL;
    const char          *columnName = NULL;

// clear
    etStringClean( sqlquery );

// create table
    etStringCharSet( sqlquery, "SELECT ", 7 );

    etDBObjectIterationReset(dbObject);
    while( etDBObjectTableColumnNext(dbObject) == etID_YES ){

    // comma
        if( firstColumn == etID_FALSE ){
            etStringCharAdd( sqlquery, "," );
        }

    // column name
        if( etDBObjectTableColumnNameGet( dbObject, columnName ) != etID_YES ) return etID_STATE_ERR;
        etStringCharAdd( sqlquery, "\"" );
        etStringCharAdd( sqlquery, columnName );
        etStringCharAdd( sqlquery, "\"" );

        firstColumn = etID_FALSE;
    }

// FROM
    etDBObjectTableNameGet( dbObject, tableName );
    etStringCharAdd( sqlquery, " FROM " );
    etStringCharAdd( sqlquery, tableName );




}





