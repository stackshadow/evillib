/* etDB - evillib json DB representation
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



#ifdef ET_SINGLEMODULE
    #include "evillib_defines.h"
    #include "evillib_depends.h"
	#include "evillib-extra_depends.h"

    #include "core/etDebug.h"
    #include "core/etObject.h"
    #include "memory/etMemoryBlock.h"
    #include "memory/etMemory.h"
    #include "string/etString.h"
    #include "string/etStringChar.h"


    #include "db/etDBTable.h"
    #include "db/etDBColumn.h"
    #include "db/etDBValue.h"
    #include "db/etDBFilter.h"
    #include "db/etDB.h"
    #include  "etDBSQLite.h"
#endif


const char* etjDB_TYPE_SQLITE[etjDB_TYPE_SQLITE_MAX] = {
    "TEXT\0",
    "TEXT\0",
    "INTEGER\0"
};
const char* etjDB_COMP_SQLITE[etjDB_COMP_SQLITE_MAX] = {
    "==\0",
    "!=\0"
};
etID_STATE     (*etDBSQLiteActions[etDB_ACTION_COUNT])( etDB* etDBActual );


// ############################ Additional ############################
etID_STATE          etDBSQLitePrintState( etDBSQLite* etDBSQLiteActual, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, etDBSQLiteActual );

    if( etDBSQLiteActual->dbSQLiteState == SQLITE_OK ){
     etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, "Everything is OK with SQLITE" );
     return etID_YES;
    }
    if( etDBSQLiteActual->dbSQLiteState == SQLITE_DONE ){
     etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, "SQLITE is done with query" );
     return etID_YES;
    }
    
    if( etDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){
     etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, "SQLITE has data for you" );
     return etID_YES;
    }

    etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_ERR, sqlite3_errmsg( etDBSQLiteActual->dbSqlite ) );
    return etID_LEVEL_ERR;
}


etID_STATE          etDBSQLiteDumpResult( etDBSQLite* etDBSQLiteActual, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, etDBSQLiteActual );

// Vars

    int         dataIndex = 0;
    int         dataCount = sqlite3_data_count( etDBSQLiteActual->dbSqliteResult );
    
    if( etDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){
     
     while( etDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){

         etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, "[ROW]" );
         
         for( dataIndex = 0; dataIndex < dataCount; dataIndex++ ){
          const unsigned char* columnTest = sqlite3_column_text( etDBSQLiteActual->dbSqliteResult, dataIndex );
          etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, (const char*)columnTest );
         }
         
         etDBSQLiteActual->dbSQLiteState = sqlite3_step( etDBSQLiteActual->dbSqliteResult );

     }

    }

    return etID_YES;
}


etDBColumnType      etDBSQLiteColumnTypeGet( const char* SQLiteColumnType, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, SQLiteColumnType );


    int columnTypeIndex;
    int columnTypeMax = etjDB_TYPE_SQLITE_MAX;
    
// NUMERIC
    if( strncmp( SQLiteColumnType, "NUMERIC", 7 ) == 0 ){
        #ifndef ET_DEBUG_OFF
            snprintf( etDebugTempMessage, etDebugTempMessageLen, "Map SQLITE-Type '%s' to columnType '%i'", SQLiteColumnType, etDB_COL_TYPE_INTEGER );
            etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
        #endif
         
         return etDB_COL_TYPE_INTEGER;
    }
    
// We start with 1, because 0 is the default column type
    for( columnTypeIndex = 1; columnTypeIndex < columnTypeMax; columnTypeIndex++ ){
     
     if( strncmp( etjDB_TYPE_SQLITE[columnTypeIndex], SQLiteColumnType, 50) == 0 ){
         #ifndef ET_DEBUG_OFF
          snprintf( etDebugTempMessage, etDebugTempMessageLen, "Map SQLITE-Type '%s' to columnType '%i'", SQLiteColumnType, columnTypeIndex );
          etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
         #endif
         return columnTypeIndex;
     }
     
    }

// Warning if default column is used
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "Columntype '%s' not found, use default", SQLiteColumnType );
    etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_WARNING, etDebugTempMessage );

    
    return etDB_COL_TYPE_DEFAULT;
}


etID_STATE          etDBSQLiteQueryRun( etDB* etDBActual, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, etDBActual );
    
// Vars
    etDBSQLite*     etDBSQLiteActual = (etDBSQLite*)etDBActual->data;
    etString*       sqlquery = etDBSQLiteActual->query;
    const char*     queryChar;

// Get the query
    etStringCharGet( sqlquery, queryChar );
    etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, queryChar );

// run callback
    if( etDBActual->preRun != NULL ){
        if( etDBActual->preRun( etDBActual, etDebugActual ) != etID_YES ){
            return etID_STATE_ERROR_INTERNAL;
        }
    }


// Clear the last statement
    sqlite3_finalize( etDBSQLiteActual->dbSqliteResult );

// Run the query
    sqlite3_prepare( 
     etDBSQLiteActual->dbSqlite, 
     queryChar, sqlquery->lengthActual+1, 
     &etDBSQLiteActual->dbSqliteResult, 
     NULL 
    );

    etDBSQLiteActual->dbSQLiteState = sqlite3_step( etDBSQLiteActual->dbSqliteResult );
    etDBSQLitePrintState( etDBSQLiteActual, etDebugActual );
    etDebugReturnOnError( etDebugActual );

//
    etDBSQLiteActual->sqliteResultIndex = 0;

// Finished with this action
    etDBActionSet( etDBActual, etDB_ACTION_DONE, etDebugActual );

    return  etID_YES;
}


etID_STATE          etDBSQLiteWhereAdd( etDB* etDBActual, etDBFilter* dbFilter, etString* sqlQuery, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
    if( dbFilter == NULL ) return etID_YES;


// vars
    int                 filterCount = 0;
    int                 filterIndex = 0;
    etID_TYPE           filterGroupMode;
    int                 filterGroupOld = 0;
    int                 filterGroup = 0;
    const char*         filterColumn;
    etDBFilterMode      filterCompareMode;
    const char*         filterValue;


// Count the filter
    filterCount = etDBFilterCount( dbFilter, etDebugActual );
    if( filterCount <= 0 ){ return etID_YES; }



    etStringCharAdd( sqlQuery, " WHERE " );
    
    for( filterIndex = 0; filterIndex < filterCount; filterIndex++ ){
     

     
     etDBFilterGet(     dbFilter, filterIndex, 
                &filterGroupMode, &filterGroup, &filterColumn, &filterCompareMode, &filterValue, etDebugActual );
     
    // If the filter is in the same group
     if( filterGroupOld == filterGroup ){
         switch( filterGroupMode ){
          case etID_TYPE_AND:
              etStringCharAdd( sqlQuery, " AND " );
              break;
          
          default:
              snprintf( etDebugTempMessage, etDebugTempMessageLen, "Filter group '%i' not handled by driver yet", filterGroupMode );
              etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_WARNING, etDebugTempMessage );
              etStringCharAdd( sqlQuery, " AND " );
         }
     }

    // filter group changed
    if( filterGroupOld != filterGroup ){
        if( filterGroupOld != 0 ){
            etStringCharAdd( sqlQuery, " )" );
          
            switch( filterGroupMode ){
                case etID_TYPE_AND:
                    etStringCharAdd( sqlQuery, " AND " );
                    break;
              
                default:
                    snprintf( etDebugTempMessage, etDebugTempMessageLen, "Filter group '%i' not handled by driver yet", filterGroupMode );
                    etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_WARNING, etDebugTempMessage );
                    etStringCharAdd( sqlQuery, " AND " );
            }

         }
         etStringCharAdd( sqlQuery, " ( " );
         filterGroupOld = filterGroup;
     }
     
     etStringCharAdd( sqlQuery, " \"" );
     etStringCharAdd( sqlQuery, filterColumn );
     etStringCharAdd( sqlQuery, "\" " );
    
    switch( filterCompareMode ){
        case etDB_FILTER_EQUAL:
            etStringCharAdd( sqlQuery, "==" );
            etStringCharAdd( sqlQuery, " '" );
            etStringCharAdd( sqlQuery, filterValue );
            etStringCharAdd( sqlQuery, "' " );
            break;

        case etDB_FILTER_ISNULL:
            etStringCharAdd( sqlQuery, "IS NULL" );
            break;

        case etDB_FILTER_NOTNULL:
            etStringCharAdd( sqlQuery, "NOT NULL" );
            break;

        default:
            snprintf( etDebugTempMessage, etDebugTempMessageLen, "Filter group '%i' not handled by driver yet", filterGroupMode );
            etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_WARNING, etDebugTempMessage );
            etStringCharAdd( sqlQuery, "==" );
            etStringCharAdd( sqlQuery, " '" );
            etStringCharAdd( sqlQuery, filterValue );
            etStringCharAdd( sqlQuery, "' " );
        }

    }

    etStringCharAdd( sqlQuery, " )" );

    return etID_YES;
}



// ############################ ACTIONS ############################

etID_STATE          etDBSQLiteTableListGet( etDB* etDBActual, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, etDBActual );
    
// Vars
    etDBSQLite*     etDBSQLiteActual = (etDBSQLite*)etDBActual->data;
    etID_STATE      tempState = etID_NO;
    etString*    sqlquery = etDBSQLiteActual->query;


// Build Query
    tempState = etID_YES;
    tempState |= etStringCharSet( sqlquery, "SELECT name AS 'table' FROM sqlite_master WHERE type = 'table'\0", 63 );
    if( tempState != etID_YES ){    return etID_STATE_ERROR_INTERNAL;    }


// Run the query
    etDBSQLiteQueryRun( etDBActual, etDebugActual );
    etDebugReturnOnError( etDebugActual );

    return etID_YES;
}


etID_STATE          etDBSQLiteTableAdd( etDB* etDBActual, etDBTable* dbTable, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, etDBActual );
    etDebugCheckNull( etDebugActual, dbTable );
    
// Vars
    etDBSQLite*      etDBSQLiteActual = (etDBSQLite*)etDBActual->data;
    etID_STATE       tempState = etID_NO;
    etString*        sqlquery = etDBSQLiteActual->query;
    const char*      tableName;
    const char*      columnName;
    etDBColumnType      columnType;
    const char*      columnTypeName;



// Basic query
    etStringCharSet( sqlquery, "CREATE TABLE IF NOT EXISTS ", 27 );

// Table name
    etDBTableNameGet( dbTable, tableName, etDebugActual );
    etStringCharAdd( sqlquery, "'" );
    etStringCharAdd( sqlquery, tableName );
    etStringCharAdd( sqlquery, "'" );


// Columns    
    etStringCharAdd( sqlquery, " ( " );

    etDBColumnIndexReset( dbTable, etDebugActual );
    tempState = __etDBColumnGetNextFull( dbTable, NULL, &columnName, etDebugActual );
    while( tempState == etID_YES ){

    // get Column Name
     tempState = etDBColumnNameGet( dbTable, columnName, etDebugActual );
     if( tempState == etID_STATE_NEW ){

     // column name
         etStringCharAdd( sqlquery, "'" );
         etStringCharAdd( sqlquery, columnName );
         etStringCharAdd( sqlquery, "' " );

     // column type
         tempState = etDBColumnTypeGet( dbTable, columnType, etDebugActual );
         if( tempState == etID_STATE_NODATA ){
          etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_WARNING, "Column Type is not set in etDB, we use the default one" );
          columnType = etDB_COL_TYPE_DEFAULT;
         }

         columnTypeName = etjDB_TYPE_SQLITE[columnType];
         etStringCharAdd( sqlquery, columnTypeName );
         
     // Primary key
         if( etDBColumnIsIdentifier( dbTable, columnName, etDebugActual ) == etID_YES ){
          etStringCharAdd( sqlquery, " PRIMARY KEY" );
         }
     }


        // next column
        tempState = __etDBColumnGetNextFull( dbTable, NULL, &columnName, etDebugActual );
        if( tempState == etID_YES  ){
            etStringCharAdd( sqlquery, ", " );
        }

    }

    etStringCharAdd( sqlquery, " );\0" );

    etDebugReturnOnError( etDebugActual );
    return etDBSQLiteQueryRun( etDBActual, etDebugActual );
    
}


etID_STATE          etDBSQLiteTableGet( etDB* etDBActual, etDBTable* dbTable, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, etDBActual );
    etDebugCheckNull( etDebugActual, dbTable );
    
// Vars
    etDBSQLite*         etDBSQLiteActual = (etDBSQLite*)etDBActual->data;
    etID_STATE          tempState = etID_NO;
    etString*           sqlquery = etDBSQLiteActual->query;
    const char*         tableName;
    const char*         columnName;
    int                 columnType;
    int                 sqliteDataCount;
    const char*         sqliteColumnType;
    //int              sqliteDataNotNull;
    int                 sqliteDataPK;

// Table name
    etDBTableNameGet( dbTable, tableName, etDebugActual );


// Build Query
    tempState = etID_YES;
    tempState |= etStringCharSet( sqlquery, "PRAGMA table_info( '\0", 20 );
    tempState |= etStringCharAdd( sqlquery, tableName );
    tempState |= etStringCharAdd( sqlquery, "' )\0" );
    if( tempState != etID_YES ) return etID_STATE_ERROR_INTERNAL;


// Run the query
    etDebugReturnOnError( etDebugActual );
    etDBSQLiteQueryRun( etDBActual, etDebugActual );
    etDebugReturnOnError( etDebugActual );


// Set that the table is inside the db
    sqliteDataCount = sqlite3_data_count( etDBSQLiteActual->dbSqliteResult );
    if( sqliteDataCount <= 0 ){
      etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_WARNING, "Table not exist in DB" );
      return etID_STATE_NODATA;
    }


// Set table-original name
    etDBTableNameSetFull( dbTable, tableName, NULL, etDebugActual );


    while( etDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){

     columnName = (const char*)sqlite3_column_text( etDBSQLiteActual->dbSqliteResult, 1 );
     sqliteColumnType = (const char*)sqlite3_column_text( etDBSQLiteActual->dbSqliteResult, 2 );
     //sqliteDataNotNull = sqlite3_column_int( etDBSQLiteActual->dbSqliteResult, 3 );
     sqliteDataPK = sqlite3_column_int( etDBSQLiteActual->dbSqliteResult, 5 );

    // Try to find the column in our etDB description
     if( etDBColumnSeek( dbTable, columnName, etDebugActual ) == etID_YES ){
         etDBColumnNameOriginalSet( dbTable, columnName, etDebugActual );
     } else {
         etDBColumnNameOriginalSet( dbTable, columnName, etDebugActual );
     }

    // Get the ColumnType
     columnType = etDBSQLiteColumnTypeGet( sqliteColumnType, etDebugActual );
     etDBColumnTypeOriginalSet( dbTable, columnType, etDebugActual );
     
    // Set primary key
     if( sqliteDataPK > 0 ){
         etDBColumnIdentifierSet( dbTable, etDebugActual );
     }


     etDBSQLiteActual->dbSQLiteState = sqlite3_step( etDBSQLiteActual->dbSqliteResult );
    }



    return etID_YES;
}


etID_STATE          etDBSQLiteDataAdd( etDB* etDBActual, etDBTable* dbTable, etDBValue* dbValues, etDebug* etDebugActual ){
// Check
// Check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, etDBActual );
    etDebugCheckNull( etDebugActual, dbTable );
    etDebugCheckNull( etDebugActual, dbTable->jsonColumns );
    etDebugCheckNull( etDebugActual, dbValues );



// Vars
    etDBSQLite*      etDBSQLiteActual = (etDBSQLite*)etDBActual->data;
    etID_STATE       tempState = etID_NO;
    etString*        sqlquery = etDBSQLiteActual->query;
    const char*      sqlQueryChar = NULL;
    etString*        sqlQueryValues = etDBSQLiteActual->queryValues;
    const char*      tableName;
    etID_STATE       hasColumn;
    const char*      columnName;
    const char*      columnValue;



// Allocate memory for query values
    etStringClean( sqlquery );
    etStringClean( sqlQueryValues );

// Basic query
    etStringCharSet( sqlquery, "INSERT INTO \0", 13 );
    etStringCharSet( sqlQueryValues, " VALUES ( \0", 11 );


// Get table-name
    etDBTableNameGet(dbTable, tableName, etDebugActual );
    etStringCharAdd( sqlquery, "'\0" );
    etStringCharAdd( sqlquery, tableName );
    etStringCharAdd( sqlquery, "'\0" );

// Columns
    hasColumn = etID_NO;
    etStringCharAdd( sqlquery, " ( \0" );

    etDBColumnIndexReset( dbTable, etDebugActual );
    while( __etDBColumnGetNextFull( dbTable, &columnName, &columnName, etDebugActual ) == etID_YES ){

    // get Column Name
     etDBColumnNameGet( dbTable, columnName, etDebugActual );
     if( columnName != NULL ){

     // has column an new value ?
         etDBValueGet( dbValues, columnName, columnValue );
         if( columnValue != NULL ){
          
         // comma
          if( hasColumn == etID_YES ){
              etStringCharAdd( sqlquery, ", \0" );
              etStringCharAdd( sqlQueryValues, ", \0" );
          }
          hasColumn = etID_YES;
          
         // column name
          etStringCharAdd( sqlquery, "'\0" );
          etStringCharAdd( sqlquery, columnName );
          etStringCharAdd( sqlquery, "' \0" );

         // column value
          etStringCharAdd( sqlQueryValues, "'\0" );
          etStringCharAdd( sqlQueryValues, columnValue );
          etStringCharAdd( sqlQueryValues, "' \0" );

         }

     }

    }

    etStringCharAdd( sqlquery, " )\0" );
    etStringCharAdd( sqlQueryValues, " )\0" );

    etStringCharGet( sqlQueryValues, sqlQueryChar );
    etStringCharAdd( sqlquery, sqlQueryChar );

    etStringCharAdd( sqlquery, ";\0" );

// Run the query
    etDebugReturnOnError( etDebugActual );
    etDBSQLiteQueryRun( etDBActual, etDebugActual );
    etDebugReturnOnError( etDebugActual );

    return etID_YES;
}


etID_STATE          etDBSQLiteDataChange( etDB *etDBActual, etDBTable *dbTable, etDBValue *dbValues, etDebug* etDebugActual ){
// Check
    etCheckNull( dbTable );
    etCheckNull( dbTable->jsonColumns );
    etCheckNull( dbValues );


// Vars
    etDBSQLite*     etDBSQLiteActual = (etDBSQLite*)etDBActual->data;
    etID_STATE      tempState = etID_NO;
    etString*       sqlquery = etDBSQLiteActual->query;
    const char*     tableName;
    etID_STATE      oneValue = etID_YES;
    const char*     columnName;
    const char*     columnValue;
    const char*     primaryKey = NULL;
    const char*     primaryKeyValue = NULL;

// Basic query
    etStringClean( sqlquery );
    etStringCharSet( sqlquery, "UPDATE \0", 13 );

// Get table-name
    etDBTableNameGet(dbTable, tableName, etDebugActual);
    etStringCharAdd( sqlquery, "'\0" );
    etStringCharAdd( sqlquery, tableName );
    etStringCharAdd( sqlquery, "'\0" );

// Set values
    etStringCharAdd( sqlquery, " SET \0" );

    etDBColumnIndexReset( dbTable, etDebugActual );
    while( __etDBColumnGetNextFull( dbTable, NULL, &columnName, etDebugActual ) == etID_YES ){

    // get Column Name
        tempState = etDBColumnNameGet( dbTable, columnName, etDebugActual );
        if( tempState != etID_YES ) continue;
        if( columnName == NULL ) continue;

    // has column an new value ?
        tempState = etDBValueGet( dbValues, columnName, columnValue );
        if( tempState != etID_YES ) continue;
        if( columnName == NULL ) continue;
          
    // comma
        if( oneValue == etID_NO ){
            etStringCharAdd( sqlquery, " , \0" );
        }
        oneValue = etID_NO;
          
    // column name
        etStringCharAdd( sqlquery, "\"\0" );
        etStringCharAdd( sqlquery, columnName );
        etStringCharAdd( sqlquery, "\" = \0" );

    // column value
        etStringCharAdd( sqlquery, "'\0" );
        etStringCharAdd( sqlquery, columnValue );
        etStringCharAdd( sqlquery, "'\0" );

    }


// WHERE primaryKey
    tempState = etDBColumnIdentifierGet( dbTable, primaryKey, etDebugActual );
    if( tempState != etID_YES || primaryKey == NULL ){ 
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Table '%s' dont have an primary key", tableName );
        etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_ERR, etDebugTempMessage );
        return etID_STATE_ERROR_INTERNAL;
    }

// primaryKey-value
    tempState = etDBValueGet( dbValues, primaryKey, primaryKeyValue );
    if( tempState != etID_YES ) return etID_STATE_ERROR_INTERNAL;
    if( primaryKeyValue == NULL ) return etID_STATE_ERROR_INTERNAL;

    etStringCharAdd( sqlquery, " WHERE \0" );
    etStringCharAdd( sqlquery, "\"\0" );
    etStringCharAdd( sqlquery, primaryKey );
    etStringCharAdd( sqlquery, "\" == '\0" );
    etStringCharAdd( sqlquery, primaryKeyValue );
    etStringCharAdd( sqlquery, "';\0" );

// Run the query
    etDebugReturnOnError( etDebugActual );
    etDBSQLiteQueryRun( etDBActual, etDebugActual );
    etDebugReturnOnError( etDebugActual );


    return etID_YES;
}


etID_STATE          etDBSQLiteDataRemove( etDB *etDBActual, etDBTable *dbTable, etDBValue *dbValues, etDebug* etDebugActual ){
    
    
// Vars
    etDBSQLite*     etDBSQLiteActual = (etDBSQLite*)etDBActual->data;
    etString*       sqlquery = etDBSQLiteActual->query;
    const char*     tableName;
    const char*     primaryKey = NULL;
    const char*     primaryKeyValue = NULL;

// Basic query
    etStringCharSet( sqlquery, "DELETE FROM \0", 13 );  
    
// Get table-name
    etDBTableNameGet(dbTable, tableName, etDebugActual);
    etStringCharAdd( sqlquery, "'" );
    etStringCharAdd( sqlquery, tableName );
    etStringCharAdd( sqlquery, "'" );


// WHERE primaryKey
    etDBColumnIdentifierGet( dbTable, primaryKey, etDebugActual );

// primaryKey-value
    etDBValueGet( dbValues, primaryKey, primaryKeyValue );

    etStringCharAdd( sqlquery, " WHERE " );
    etStringCharAdd( sqlquery, "\"" );
    etStringCharAdd( sqlquery, primaryKey );
    etStringCharAdd( sqlquery, "\" == '" );
    etStringCharAdd( sqlquery, primaryKeyValue );
    etStringCharAdd( sqlquery, "'" );


// Run the query
    etDebugReturnOnError( etDebugActual );
    etDBSQLiteQueryRun( etDBActual, etDebugActual );
    etDebugReturnOnError( etDebugActual );

    return etID_YES;
}


etID_STATE          etDBSQLiteDataGet( etDB* etDBActual, etDBTable* dbTable, etDBFilter* dbFilter, etDebug* etDebugActual ){
// Check
    etCheckNull( etDBActual );
    etCheckNull( dbTable );


// Vars
    etDBSQLite*         etDBSQLiteActual = (etDBSQLite*)etDBActual->data;
    etID_STATE          tempState = etID_NO;
    etString*           sqlQuery = etDBSQLiteActual->query;
    const char*         tableName;


// Basic query
    etStringCharSet( sqlQuery, "SELECT * FROM \0", 15 );

// Get table-name
    etDBTableNameGet(dbTable, tableName, etDebugActual );
    etStringCharAdd( sqlQuery, "'" );
    etStringCharAdd( sqlQuery, tableName );
    etStringCharAdd( sqlQuery, "'" );

// add where
    etDBSQLiteWhereAdd( etDBActual, dbFilter, sqlQuery, etDebugActual );

// Run the query
    etDebugReturnOnError( etDebugActual );
    etDBSQLiteQueryRun( etDBActual, etDebugActual );
    etDebugReturnOnError( etDebugActual );

    return etID_YES;
}


// ############################ API ############################
etID_STATE          etjDBSQLiteIsConnected( etDB* etDBActual, etDebug* etDebugActual ){
    // Check
    etCheckNull( etDBActual );
    etCheckNull( etDBActual->data );

// Vars
    etDBSQLite*     etDBSQLiteActual = (etDBSQLite*)etDBActual->data;

    if( etDBSQLiteActual->dbSqlite != NULL ) return etID_YES;
    
    return etID_FALSE;
}


etID_STATE          etjDBSQliteNextData( etDB* etDBActual, etDBValue* etDBValues, etDebug* etDebugActual ){
// Check
    etCheckNull( etDBActual );
    etCheckNull( etDBValues );


// Vars
    etDBSQLite*     etDBSQLiteActual = (etDBSQLite*)etDBActual->data;
    if( etDBSQLiteActual->dbSQLiteState != SQLITE_ROW ){
        etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, "No more Data" );
        return etID_STATE_NODATA;
    }

    int                 sqliteDataCount;
    int                 sqliteDataIndex;
    const char*         columnName;
    const char*         columnValue;



// Init the columnValues
    etDBValuesClean( etDBValues );

// next result    
    if( etDBSQLiteActual->sqliteResultIndex > 0 ){
     etDBSQLiteActual->dbSQLiteState = sqlite3_step( etDBSQLiteActual->dbSqliteResult );
     if( etDBSQLiteActual->dbSQLiteState != SQLITE_ROW ){
         return etID_NO;
     }     
    }

    

    if( etDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){
        sqliteDataCount = sqlite3_data_count( etDBSQLiteActual->dbSqliteResult );
        for( sqliteDataIndex = 0; sqliteDataIndex < sqliteDataCount; sqliteDataIndex++ ){
            columnName = sqlite3_column_name( etDBSQLiteActual->dbSqliteResult, sqliteDataIndex );
            columnValue = (const char*)sqlite3_column_text( etDBSQLiteActual->dbSqliteResult, sqliteDataIndex );
            etDBValueSet( etDBValues, columnName, columnValue );
        }

        etDBSQLiteActual->sqliteResultIndex++;
    }
    
#ifndef ET_DEBUG_OFF
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "Data readed #%i", etDBSQLiteActual->sqliteResultIndex );
    etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif


    return etID_YES;
}


etID_STATE          etDBSQLiteDump( etDB* etDBActual, etDebug* etDebugActual ){
// Check
    etCheckNull( etDBActual );



    return etID_YES;
}



/**
@todo Check size of driver-type
*/
etID_STATE          etDBSQLiteInit( etDB* etDBActual, etDebug* etDebugActual ){
    
// Vars
    etMemoryBlock*      etMemoryBlockActual = NULL;
    etDBSQLite*         etDBSQLiteActual = NULL;


// Allocate memory for sqlite
    etMemoryRequest( etMemoryBlockActual, sizeof(etDBSQLite) );
//    etMemoryBlockClean( etMemoryBlockActual );
    etMemoryBlockDataGet( etMemoryBlockActual, etDBSQLiteActual );

// Allocate memory for query-string
    etStringAllocLen( etDBSQLiteActual->query, 256 );
    etStringAllocLen( etDBSQLiteActual->queryValues, 256 );


// Save memory
    etDBActual->data = etDBSQLiteActual;

// Set functions
    etDBActual->functions.isConnected = etjDBSQLiteIsConnected;

    etDBActual->functions.tableListGet = etDBSQLiteTableListGet;
    
    etDBActual->functions.tableAdd = etDBSQLiteTableAdd;
    etDBActual->functions.tableChange = NULL;
    etDBActual->functions.tableRemove = NULL;
    etDBActual->functions.tableGet = etDBSQLiteTableGet;
    
    etDBActual->functions.dataAdd = etDBSQLiteDataAdd;
    etDBActual->functions.dataChange = etDBSQLiteDataChange;
    etDBActual->functions.dataRemove = etDBSQLiteDataRemove;
    etDBActual->functions.dataGet = etDBSQLiteDataGet;
    

    etDBActual->functions.nextResult = etjDBSQliteNextData;



// return
    return etID_YES;
}


etID_STATE          etDBSQLiteConnect( etDB* etDBActual, const char* path, etDebug* etDebugActual ){
// Check
    etCheckNull( etDBActual );
    
// Vars
    etDBSQLite     * etDBSQLiteActual = (etDBSQLite*)etDBActual->data;

#ifndef ET_DEBUG_OFF
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "Connecting to %s...", path );
    etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif
     
// Open an sqlite database
    if( sqlite3_open( path, &etDBSQLiteActual->dbSqlite ) != SQLITE_OK ){
     etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_ERR, sqlite3_errmsg(etDBSQLiteActual->dbSqlite) );
     sqlite3_close( etDBSQLiteActual->dbSqlite );
     etDBSQLiteActual->dbSqlite = NULL;
     return etID_STATE_ERROR_INTERNAL;
    }

    return etID_YES;
}


