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

#include "evillib_depends.h"
#include "evillib-extra_depends.h"

#ifndef etDBSQL_C
#define etDBSQL_C

#ifdef __cplusplus
extern "C" {
#endif

#include "core/etDebug.c"
#include "core/etObject.c"
#include "memory/etMemory.c"
#include "string/etString.c"
#include "string/etStringChar.c"

#include "dbdriver/etDBDriver.c"
//#include "db/etDBObject.h"
#include "db/etDBTable.c"
#include "db/etDBColumn.c"
#include "db/etDBFilter.c"


// Table

etID_STATE          etDBSQLAddTable( etDBDriver* dbDriver, etDBTable* dbTable, etString* sqlquery, const char* columnEnclose ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( sqlquery );

// clear
    etStringClean( sqlquery );

// create table
    etStringCharSet( sqlquery, "CREATE TABLE ", -1 );

// add table name
    const char *tableName = NULL;
    etDBTableGetName( dbTable, tableName );
    etStringCharAdd( sqlquery, "\"" );
    etStringCharAdd( sqlquery, tableName );
    etStringCharAdd( sqlquery, "\"" );

// columns
    etID_BOOL           firstColumn = etID_TRUE;
    const char          *columnName = NULL;
    etDBColumnType      columnType;
    unsigned int        columnOption = etDBCOLUMN_OPTION_NOTHING;

    etStringCharAdd( sqlquery, " ( " );

    etDBColumnIterateReset(dbTable);
    while( etDBColumnIterate( dbTable ) == etID_YES ){

    // comma
        if( firstColumn == etID_FALSE ){
            etStringCharAdd( sqlquery, "," );
        }

    // get column name
        etDBColumnGet( dbTable, columnName, columnType, columnOption );

    // column name
        etStringCharAdd( sqlquery, columnEnclose );
        etStringCharAdd( sqlquery, columnName );
        etStringCharAdd( sqlquery, columnEnclose );
        etStringCharAdd( sqlquery, " " );

    // column type
        if( dbDriver->queryColumnTypeAdd != NULL ){
            dbDriver->queryColumnTypeAdd( sqlquery, columnType );
        }

    // get column specials ( like primary key )
        if( dbDriver->queryColumnOptionAdd != NULL ){
            dbDriver->queryColumnOptionAdd( sqlquery, columnOption );
        }


        firstColumn = etID_FALSE;
    }

    etStringCharAdd( sqlquery, ");" );

    return etID_YES;
}


etID_STATE          etDBSQLRemoveTable( etDBDriver* dbDriver, etDBTable* dbTable, etString* sqlquery ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( sqlquery );


// clear
    etStringClean( sqlquery );

// create table
    etStringCharSet( sqlquery, "DROP TABLE ", -1 );

// add table name
    const char *tableName = NULL;
    etDBTableGetName( dbTable, tableName );
    etStringCharAdd( sqlquery, tableName );
    etStringCharAdd( sqlquery, ";" );

    return etID_YES;

}


// Column

etID_STATE          etDBSQLAddColumn( etDBDriver* dbDriver, etDBTable* dbTable, const char* columnName, etString* sqlquery, const char* columnEnclose ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( columnName );
    etDebugCheckNull( sqlquery );

// clear
    etStringClean( sqlquery );

// create table
    etStringCharSet( sqlquery, "ALTER TABLE ", -1 );

// add table name
    const char *tableName = NULL;
    etDBTableGetName( dbTable, tableName );
    etStringCharAdd( sqlquery, "\"" );
    etStringCharAdd( sqlquery, tableName );
    etStringCharAdd( sqlquery, "\"" );

    etStringCharAdd( sqlquery, " ADD COLUMN " );

// columns
    etDBColumnType      dbColumnType;
    unsigned int        dbColumnOption = etDBCOLUMN_OPTION_NOTHING;

// find the column
    if( etDBColumnSelect( dbTable, columnName ) != etID_YES ) return etID_STATE_ERR_INTERR;

// get the values
    __etDBColumnGet( dbTable, NULL, &dbColumnType, &dbColumnOption );

// column name
    etStringCharAdd( sqlquery, columnEnclose );
    etStringCharAdd( sqlquery, columnName );
    etStringCharAdd( sqlquery, columnEnclose );
    etStringCharAdd( sqlquery, " " );

// column type
    if( dbDriver->queryColumnTypeAdd != NULL ){
        dbDriver->queryColumnTypeAdd( sqlquery, dbColumnType );
    }

// get column specials ( like primary key )
    if( dbDriver->queryColumnOptionAdd != NULL ){
        dbDriver->queryColumnOptionAdd( sqlquery, dbColumnOption );
    }



    etStringCharAdd( sqlquery, ";" );

    return etID_YES;
}


// Data

etID_STATE          etDBSQLAddData( etDBDriver* dbDriver, etDBTable* dbTable, etString* sqlquery ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( sqlquery );


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
    etDBTableGetName( dbTable, tableName );
    etStringCharAdd( sqlquery, "\"" );
    etStringCharAdd( sqlquery, tableName );
    etStringCharAdd( sqlquery, "\"" );

// columns
    etStringCharAdd( sqlquery, " (" );
    etDBColumnIterateReset( dbTable );
    while( etDBColumnIterate( dbTable ) == etID_YES ){

    // get the value
        __etDBColumnGet( dbTable, &columnName, NULL, NULL );
        etDBColumnGetValue( dbTable, columnValue );

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
        etStringCharAdd( valueString, "\'" );
        etStringCharAdd( valueString, columnValue );
        etStringCharAdd( valueString, "\'" );

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


etID_STATE          etDBSQLChangeData( etDBDriver* dbDriver, etDBTable* dbTable, etString* sqlquery ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( sqlquery );



// vars
    etID_BOOL           firstColumn = etID_TRUE;
    const char*         tableName = NULL;
    const char*         columnName = NULL;
    const char*         columnValue = NULL;
    const char*         primaryColumnName = NULL;
    const char*         primaryColumnValue = NULL;


// clear
    etStringClean( sqlquery );

// create table
    etStringCharSet( sqlquery, "UPDATE \0", 8 );

// add table name
    etDBTableGetName( dbTable, tableName );
    etStringCharAdd( sqlquery, "\"" );
    etStringCharAdd( sqlquery, tableName );
    etStringCharAdd( sqlquery, "\"" );

// columns
    etStringCharAdd( sqlquery, " SET " );
    etDBColumnIterateReset( dbTable );
    while( etDBColumnIterate( dbTable ) == etID_YES ){

    // get the value
        __etDBColumnGet( dbTable, &columnName, NULL, NULL );
        etDBColumnGetValue( dbTable, columnValue );

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
        etStringCharAdd( sqlquery, "\'" );
        etStringCharAdd( sqlquery, columnValue );
        etStringCharAdd( sqlquery, "\'" );

    }


// get the primary key-column
    if( etDBColumnSelectWithOption( dbTable, etDBCOLUMN_OPTION_PRIMARY ) != etID_YES ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "No Primary Column for table '%s'", tableName );
		etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return etID_STATE_ERR_INTERR;
    }

    __etDBColumnGet( dbTable, &primaryColumnName, NULL, NULL );

// get the value of the primary column
    if( etDBColumnGetValue( dbTable, primaryColumnValue ) != etID_YES ){
		
		
		
        return etID_STATE_ERR_INTERR;
    }

// WHERE
    etStringCharAdd( sqlquery, " WHERE \"" );
    etStringCharAdd( sqlquery, primaryColumnName );
    etStringCharAdd( sqlquery, "\" = '" );
    etStringCharAdd( sqlquery, primaryColumnValue );
    etStringCharAdd( sqlquery, "'" );


/*
UPDATE `doDB` SET `value`=? WHERE `_rowid_`='2';
*/
    return etID_YES;
}


etID_STATE          etDBSQLRemoveData( etDBDriver* dbDriver, etDBTable* dbTable, etString* sqlquery, const char* columnEnclose ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( sqlquery );


// vars
    const char*         tableName = NULL;
    const char*         primaryColumnName = NULL;
    const char*         primaryColumnValue = NULL;



// clear
    etStringClean( sqlquery );

// create table
    etStringCharSet( sqlquery, "DELETE FROM \0", 13 );

// add table name
    etDBTableGetName( dbTable, tableName );
    etStringCharAdd( sqlquery, "\"" );
    etStringCharAdd( sqlquery, tableName );
    etStringCharAdd( sqlquery, "\"" );


// get the primary key-column
    if( etDBColumnSelectWithOption( dbTable, etDBCOLUMN_OPTION_PRIMARY ) != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// get column name
    __etDBColumnGet( dbTable, &primaryColumnName, NULL, NULL );

// get the value of the primary column
    if( etDBColumnGetValue( dbTable, primaryColumnValue ) != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// WHERE
    etStringCharAdd( sqlquery, " WHERE " );
    etStringCharAdd( sqlquery, columnEnclose );
    etStringCharAdd( sqlquery, primaryColumnName );
    etStringCharAdd( sqlquery, columnEnclose );
    etStringCharAdd( sqlquery, " = '" );
    etStringCharAdd( sqlquery, primaryColumnValue );
    etStringCharAdd( sqlquery, "'" );

    return etID_YES;
}


etID_STATE          etDBSQLGetData( etDBDriver* dbDriver, etDBTable* dbTable, etDBFilter* dbFilter, etString* sqlquery ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( sqlquery );

// vars
    etID_BOOL           firstColumn = etID_TRUE;
    const char          *tableName = NULL;
    const char          *columnName = NULL;

// clear
    etStringClean( sqlquery );

// create table
    etStringCharSet( sqlquery, "SELECT ", 7 );

    etDBColumnIterateReset(dbTable);
    while( etDBColumnIterate( dbTable ) == etID_YES ){

    // get
        __etDBColumnGet( dbTable, &columnName, NULL, NULL );

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
    etDBTableGetName( dbTable, tableName );
    etStringCharAdd( sqlquery, " FROM " );
    etStringCharAdd( sqlquery, "\"" );
    etStringCharAdd( sqlquery, tableName );
    etStringCharAdd( sqlquery, "\"" );

// if we dont have a filter, we return
    if( dbFilter == NULL ) return etID_YES;

// WHERE
    etID_STATE          isWHERE = etID_NO;
    etID_STATE          isFirstOperation = etID_YES;
    int                 actualFilterGroup = -1;
    int                 filterGroup;
    etDBFILTER_OP       filterOperation;
    const char*         filterColumn;
    etDBFILTER_TYPE     filterType;
    const char*         filterString;

    etDBFilterIterateReset( dbFilter );
    while( etDBFilterIterate( dbFilter ) == etID_YES ){

    // get
        etDBFilterElementGet( dbFilter, filterGroup, filterOperation, filterColumn, filterType, filterString );

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
            etStringCharAdd( sqlquery, " = '" );
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


etID_STATE          etDBSQLGetDataWithOffset( etDBDriver* dbDriver, etDBTable* dbTable, int offset, int amount, etDBFilter* dbFilter, etString* sqlquery ){
// check
    etDebugCheckNull( dbDriver );
    etDebugCheckNull( dbTable );
    etDebugCheckNull( sqlquery );

    if( etDBSQLGetData(dbDriver,dbTable,dbFilter,sqlquery) != etID_YES ){
        return etID_STATE_ERR_INTERR;
    }

// temp
    // UINT_MAX 	4294967295 = 10 digits
    char numberString[11];
    memset( numberString, 0, 11 );

// append the limit to the end of the query
    etStringCharAdd( sqlquery, " LIMIT " );

    snprintf( numberString, 10, "%i", amount );
    etStringCharAdd( sqlquery, numberString );

    etStringCharAdd( sqlquery, " OFFSET " );

    snprintf( numberString, 10, "%i", offset );
    etStringCharAdd( sqlquery, numberString );

    return etID_YES;
}






#ifdef __cplusplus
}
#endif


#endif



