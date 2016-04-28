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
    if( dbObject->jsonTable == NULL ){
        etDebugMessage( etID_LEVEL_WARNING, "You did not select a table" );
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
    while( etDBObjectTableColumnNext(dbObject,columnName) == etID_YES ){
        
    // comma
        if( firstColumn == etID_FALSE ){
            etStringCharAdd( sqlquery, "," );
        }

    // column name
        etStringCharAdd( sqlquery, "'" );
        etStringCharAdd( sqlquery, columnName );
        etStringCharAdd( sqlquery, "' " );
        
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

    return etID_YES;
}


etID_STATE          etDBSQLInsertInto( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( sqlquery );


// check if an table is selected
    if( dbObject->jsonTable == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table" );
        return etID_STATE_WARN_SEQERR;
    }

// vars
    etID_BOOL           firstColumn = etID_TRUE;
    const char          *tableName = NULL;
    const char          *columnName = NULL;
    const char          *columnValue = NULL;
    etString            *valueString = NULL;

// we need a value string
    etStringAllocLen( valueString, 128 );

// clear
    etStringClean( sqlquery );

// create table
    etStringCharSet( sqlquery, "INSERT INTO ", 12 );

// add table name
    etDBObjectTableNameGet( dbObject, tableName );
    etStringCharAdd( sqlquery, tableName );

// columns
    etStringCharAdd( sqlquery, "(" );
    etDBObjectIterationReset(dbObject);
    while( etDBObjectValueNext(dbObject,columnName,columnValue) == etID_YES ){

    // comma
        if( firstColumn == etID_FALSE ){
            etStringCharAdd( sqlquery, "," );
            etStringCharAdd( valueString, "," );
        }

    // column name
        etStringCharAdd( sqlquery, "\"" );
        etStringCharAdd( sqlquery, columnName );
        etStringCharAdd( sqlquery, "\"" );

    // column value
        etStringCharAdd( valueString, "\"" );
        etStringCharAdd( valueString, columnValue );
        etStringCharAdd( valueString, "\"" );

        firstColumn = etID_FALSE;
    }

// values
    etStringCharAdd( sqlquery, ") VALUES (" );
    etStringCharGet( valueString, columnValue );
    etStringCharAdd( sqlquery, columnValue );
    etStringCharAdd( sqlquery, ")" );

// free
    etStringFree( valueString );
/*
INSERT INTO table_name (column1,column2,column3,...)
VALUES (value1,value2,value3,...);
*/
    return etID_YES;
}


etID_STATE          etDBSQLUpdate( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( sqlquery );


// check if an table is selected
    if( dbObject->jsonTable == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table" );
        return etID_STATE_WARN_SEQERR;
    }

// vars
    etID_BOOL           firstColumn = etID_TRUE;
    const char          *tableName = NULL;
    const char          *columnName = NULL;
    const char          *columnValue = NULL;
    const char          *primaryColumn = NULL;
    const char          *primaryColumnValue = NULL;


// clear
    etStringClean( sqlquery );

// create table
    etStringCharSet( sqlquery, "UPDATE ", 7 );

// add table name
    etDBObjectTableNameGet( dbObject, tableName );
    etStringCharAdd( sqlquery, tableName );

// columns
    etStringCharAdd( sqlquery, " SET " );
    etDBObjectIterationReset(dbObject);
    while( etDBObjectValueNext(dbObject,columnName,columnValue) == etID_YES ){

    // comma
        if( firstColumn == etID_FALSE ){
            etStringCharAdd( sqlquery, "," );
        }
        firstColumn = etID_FALSE;

    // column name
        etStringCharAdd( sqlquery, "\"" );
        etStringCharAdd( sqlquery, columnName );
        etStringCharAdd( sqlquery, "\" = " );

    // column value
        etStringCharAdd( sqlquery, "\"" );
        etStringCharAdd( sqlquery, columnValue );
        etStringCharAdd( sqlquery, "\"" );

    }

// get the primary key-column
    if( etDBObjectTableColumnPrimaryGet( dbObject, primaryColumn ) != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }
// get the value of the primary column
    if( etDBObjectValueGet( dbObject, primaryColumn, primaryColumnValue ) != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// WHERE
    etStringCharAdd( sqlquery, " WHERE \"" );
    etStringCharAdd( sqlquery, primaryColumn );
    etStringCharAdd( sqlquery, "\" = \"" );
    etStringCharAdd( sqlquery, primaryColumnValue );
    etStringCharAdd( sqlquery, "\"" );



/*
UPDATE `doDB` SET `value`=? WHERE `_rowid_`='2';
*/
    return etID_YES;
}


etID_STATE          etDBSQLSelect( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbObject );
    etDebugCheckNull( sqlquery );


// check if an table is selected
    if( dbObject->jsonTable == NULL ){
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
    while( etDBObjectTableColumnNext(dbObject,columnName) == etID_YES ){

    // comma
        if( firstColumn == etID_FALSE ){
            etStringCharAdd( sqlquery, "," );
        }

    // column name
        etStringCharAdd( sqlquery, "\"" );
        etStringCharAdd( sqlquery, columnName );
        etStringCharAdd( sqlquery, "\"" );

        firstColumn = etID_FALSE;
    }

// FROM
    etDBObjectTableNameGet( dbObject, tableName );
    etStringCharAdd( sqlquery, " FROM " );
    etStringCharAdd( sqlquery, tableName );


// WHERE
    etID_STATE          isWHERE = etID_NO;
    etID_STATE          isFirstOperation = etID_YES;
    int                 actualFilterGroup = -1;
    int                 filterGroup;
    etDBFILTER_OP       filterOperation;
    const char          *filterColumn;
    etDBFILTER_TYPE     filterType;
    const char          *filterString;


    etDBObjectIterationReset( dbObject );
    while( etDBObjectFilterNext( dbObject, &filterGroup, &filterOperation, &filterColumn, &filterType, &filterString ) == etID_YES ){

    // add WHERE
        if( isWHERE == etID_NO ){
            etStringCharAdd( sqlquery, " WHERE" );
            isWHERE = etID_YES;
        }

    // operation
        if( isFirstOperation == etID_NO ){

            if( filterOperation == etDBFILTER_OP_NOTHING ){
                etStringCharAdd( sqlquery, " AND" );
            }
            if( filterOperation == etDBFILTER_OP_AND ){
                etStringCharAdd( sqlquery, " AND" );
            }
            if( filterOperation == etDBFILTER_OP_OR ){
                etStringCharAdd( sqlquery, " OR" );
            }

        }
        isFirstOperation = etID_NO;

    // "("
        if( actualFilterGroup != filterGroup && actualFilterGroup != -1 ){
            etStringCharAdd( sqlquery, " (" );
        }

    // column
        etStringCharAdd( sqlquery, " \"" );
        etStringCharAdd( sqlquery, filterColumn );
        etStringCharAdd( sqlquery, "\"" );

    // type
        if( filterType == etDBFILTER_TYPE_NOTHING ){
            filterType = etDBFILTER_TYPE_EQUAL;
        }

        if( filterType == etDBFILTER_TYPE_EQUAL ){
            etStringCharAdd( sqlquery, " IS '" );
            etStringCharAdd( sqlquery, filterString );
            etStringCharAdd( sqlquery, "'" );
        }

        if( filterType == etDBFILTER_TYPE_NOTEQUAL ){
            filterType = etDBFILTER_TYPE_CONTAIN;
        }

        if( filterType == etDBFILTER_TYPE_CONTAIN ){
            etStringCharAdd( sqlquery, " LIKE '%" );
            etStringCharAdd( sqlquery, filterString );
            etStringCharAdd( sqlquery, "%'" );
        }

        if( filterType == etDBFILTER_TYPE_BEGIN ){
            etStringCharAdd( sqlquery, " LIKE '" );
            etStringCharAdd( sqlquery, filterString );
            etStringCharAdd( sqlquery, "%' " );
        }

        if( filterType == etDBFILTER_TYPE_ENDs ){
            etStringCharAdd( sqlquery, " LIKE '%" );
            etStringCharAdd( sqlquery, filterString );
            etStringCharAdd( sqlquery, "' " );
        }

    // ")"
        if( actualFilterGroup != filterGroup && actualFilterGroup != -1 ){
            etStringCharAdd( sqlquery, " )" );
        }

    // save actual filter group
        if( actualFilterGroup != filterGroup || actualFilterGroup != -1 ){
            actualFilterGroup = filterGroup;
        }

    }


//SELECT * FROM 'main'.'city' WHERE displayName LIKE '%erlin%'

    return etID_YES;
}





