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
    etID_BOOL firstColumn = etID_TRUE;
    const char *columnName = NULL;
    etDBColumnType columnType;
    int columnOption = etDBCOLUMN_OPTION_NOTHING;
    
    etStringCharAdd( sqlquery, "( " );
    etDBObjectIterationReset(dbObject);
    while( etDBObjectTableColumnNext(dbObject) == etID_YES ){
        
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


etID_STATE          etDBSQLSelectAll( etDBObject *dbObject, etString *sqlquery ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( sqlquery );

// clear
    etStringClean( sqlquery );

/*
CREATE TABLE Persons
(
PersonID int,
LastName varchar(255),
FirstName varchar(255),
Address varchar(255),
City varchar(255)
);
*/


}


