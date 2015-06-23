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

	#include "core/etDebug.h"
	#include "core/etObject.h"
	#include "memory/etMemoryBlock.h"
	#include "memory/etMemory.h"
	#include "string/etString.h"
	#include "string/etStringChar.h"

	#include "db/etDB.h"
	#include "etDBDriver.h"
	#include "db/etDBTable.h"
	#include "db/etDBColumn.h"
	#include "db/etDBValue.h"
	#include "db/etDBFilter.h"
	#include "etDBSQLite.h"
#endif


const char *etjDB_TYPE_SQLITE[etjDB_TYPE_SQLITE_MAX] = {
	"TEXT\0",
	"TEXT\0",
	"INTEGER\0"
};
const char *etjDB_COMP_SQLITE[etjDB_COMP_SQLITE_MAX] = {
	"==\0",
	"!=\0"
};

etID_STATE			etDBSQLiteRun( etDBDriver *etjDBDriverActual, etDB *etDBActual );
etID_STATE			etDBSQLiteDump( etDBDriver *etjDBDriverActual, etDB *etDBActual );
etID_STATE			etjDBSQliteDataGet( etDBDriver *etjDBDriverActual, etDB *etDBActual );

/**
@todo Check size of driver-type
*/
etID_STATE			__etDBSQLiteAlloc( etDBDriver **p_etjDBDriverActual ){
	
// Vars
	etMemoryBlock 		*etMemoryBlockActual = NULL;
	etDBSQLite 		*etDBSQLiteActual = NULL;


// Allocate memory for sqlite
	etMemoryRequest( etMemoryBlockActual, sizeof(etDBSQLite) );
	etMemoryBlockDataGet( etMemoryBlockActual, etDBSQLiteActual );

// Allocate memory for query-string
	etStringAllocLen( etDBSQLiteActual->query, 256 );

// Set functions
	etDBSQLiteActual->driverClass.queryRun = etDBSQLiteRun;
	etDBSQLiteActual->driverClass.nextResult = etjDBSQliteDataGet;
	etDBSQLiteActual->driverClass.dump = etDBSQLiteDump;

// return
	*p_etjDBDriverActual = (etDBDriver*)etDBSQLiteActual;
	return etID_YES;
}


etID_STATE			etDBSQLiteConnect( etDBDriver *etjDBDriverActual, const char *path ){
// Check
	etCheckNull( etjDBDriverActual );
	
// Vars
	etDBSQLite 		*etDBSQLiteActual = (etDBSQLite*)etjDBDriverActual;

#ifndef ET_DEBUG_OFF
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Connecting to %s...", path );
	etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif
		
// Open an sqlite database
	if( sqlite3_open( path, &etDBSQLiteActual->dbSqlite ) != SQLITE_OK ){
		etDebugMessage( etID_LEVEL_ERR, sqlite3_errmsg(etDBSQLiteActual->dbSqlite) );
		return etID_STATE_ERROR_INTERNAL;
	}

	return etID_YES;
}




// Internal functions
etID_STATE			etDBSQLitePrintState( etDBSQLite *etDBSQLiteActual ){
	
	if( etDBSQLiteActual->dbSQLiteState == SQLITE_OK ){
		etDebugMessage( etID_LEVEL_DETAIL_DB, "Everything is OK with SQLITE" );
		return etID_YES;
	}
	if( etDBSQLiteActual->dbSQLiteState == SQLITE_DONE ){
		etDebugMessage( etID_LEVEL_DETAIL_DB, "SQLITE is done with query" );
		return etID_YES;
	}
	
	if( etDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){
		etDebugMessage( etID_LEVEL_DETAIL_DB, "SQLITE has data for you" );
		return etID_YES;
	}

	etDebugMessage( etID_LEVEL_ERR, sqlite3_errmsg( etDBSQLiteActual->dbSqlite ) );
	return etID_LEVEL_ERR;
}


void				etDBSQLiteDumpResult( etDBSQLite *etDBSQLiteActual ){
	
// Vars

	int			dataIndex = 0;
	int			dataCount = sqlite3_data_count( etDBSQLiteActual->dbSqliteResult );
	
	if( etDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){
		
		while( etDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){

			etDebugMessage( etID_LEVEL_DETAIL_DB, "[ROW]" );
			
			for( dataIndex = 0; dataIndex < dataCount; dataIndex++ ){
				const unsigned char *columnTest = sqlite3_column_text( etDBSQLiteActual->dbSqliteResult, dataIndex );
				etDebugMessage( etID_LEVEL_DETAIL_DB, (const char*)columnTest );
			}
			
			etDBSQLiteActual->dbSQLiteState = sqlite3_step( etDBSQLiteActual->dbSqliteResult );

		}

	}


}


etDBColumnType		etDBSQLiteColumnTypeGet( const char *SQLiteColumnType ){
	
	int columnTypeIndex;
	int columnTypeMax = etjDB_TYPE_SQLITE_MAX;
	
// We start with 1, because 0 is the default column type
	for( columnTypeIndex = 1; columnTypeIndex < columnTypeMax; columnTypeIndex++ ){
		
		if( strncmp( etjDB_TYPE_SQLITE[columnTypeIndex], SQLiteColumnType, 50) == 0 ){
			#ifndef ET_DEBUG_OFF
				snprintf( etDebugTempMessage, etDebugTempMessageLen, "Map SQLITE-Type '%s' to columnType '%i'", SQLiteColumnType, columnTypeIndex );
				etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
			#endif
			return columnTypeIndex;
		}
		
	}

// Warning if default column is used
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Columntype '%s' not found, use default", SQLiteColumnType );
	etDebugMessage( etID_LEVEL_WARNING, etDebugTempMessage );

	
	return etDB_COL_TYPE_DEFAULT;
}



// Build-Query-Functions
etID_STATE			etDBSQLiteQueryTableAdd( etDBSQLite *etDBSQLiteActual, etDB *etDBActual ){
// Check
	etCheckNull( etDBSQLiteActual );
	etCheckNull( etDBActual );
	etCheckNull( etDBActual->nodeTable );

// Load the column-pointer
	etDBColumnInit( etDBActual );
	etCheckNull( etDBActual->nodeColumns );

	
// Vars
	etID_STATE			tempState = etID_NO;

	etString			*query = etDBSQLiteActual->query;
	etDBColumnType		columnType;

	const char			*tableName;
	const char			*columnName;
	const char			*columnTypeName;


// Basic query
	etStringCharSet( query, "CREATE TABLE IF NOT EXISTS ", 27 );

// Table name
	tempState = etDBTableNameGet(etDBActual, tableName, tableName);
	if( tempState != etID_YES ) return etID_STATE_ERROR_INTERNAL;
	etStringCharAdd( query, "'" );
	etStringCharAdd( query, tableName );
	etStringCharAdd( query, "'" );


// Columns	
	etStringCharAdd( query, " ( " );

	tempState = etDBColumnGetFirst( etDBActual );
	while( tempState == etID_YES ){

	// get Column Name
		tempState = etDBColumnNameGet( etDBActual, columnName, columnName );
		if( tempState == etID_YES || tempState == etID_STATE_NEW ){

		// column name
			etStringCharAdd( query, "'" );
			etStringCharAdd( query, columnName );
			etStringCharAdd( query, "' " );

		// column type
			tempState = etDBColumnTypeGet( etDBActual, columnType );
			if( tempState == etID_STATE_NODATA ){
				etDebugMessage( etID_LEVEL_WARNING, "Column Type is not set in etDB, we use the default one" );
				columnType = etDB_COL_TYPE_DEFAULT;
			}

			columnTypeName = etjDB_TYPE_SQLITE[columnType];
			etStringCharAdd( query, columnTypeName );
			
		// Primary key
			if( etDBColumnIsIdentifier(etDBActual) == etID_YES ){
				etStringCharAdd( query, " PRIMARY KEY" );
			}
		}


	// next column
		tempState = etDBColumnGetNext( etDBActual );
		if( tempState == etID_YES  ){
			etStringCharAdd( query, ", " );
		}

	}

	etStringCharAdd( query, " );\0" );



	return etID_YES;
}


etID_STATE			etjDBSQliteQueryTableGet( etDBSQLite *etDBSQLiteActual, etDB *etDBActual ){
// Check
	etCheckNull( etDBSQLiteActual );
	etCheckNull( etDBActual );
	etCheckNull( etDBActual->nodeTable );


// Vars
	etID_STATE		returnState = etID_YES;
	etString		*query = etDBSQLiteActual->query;
	const char		*tableName;

// Table name
	returnState = etDBTableNameGet(etDBActual, tableName, tableName);
	if( returnState != etID_YES ) return etID_STATE_ERROR_INTERNAL;

// Build Query
	returnState = etID_YES;
	returnState |= etStringCharSet( query, "PRAGMA table_info( '\0", 20 );
	returnState |= etStringCharAdd( query, tableName );
	returnState |= etStringCharAdd( query, "' )\0" );
	
	if( returnState != etID_YES ){
		return etDebugState( etID_STATE_ERROR_INTERNAL );
	}

	return etID_YES;
}


etID_STATE			etjDBSQliteQueryGetAllTableNames( etDBSQLite *etDBSQLiteActual, etDB *etDBActual ){
// Check
	etCheckNull( etDBSQLiteActual );
	etCheckNull( etDBActual );
	etCheckNull( etDBActual->nodeTable );


// Vars
	etID_STATE		returnState = etID_YES;
	etString		*query = etDBSQLiteActual->query;


// Build Query
	returnState = etID_YES;
	returnState |= etStringCharSet( query, "SELECT name FROM sqlite_master WHERE type = 'table'\0", 52 );
	
	if( returnState != etID_YES ){
		return etDebugState( etID_STATE_ERROR_INTERNAL );
	}

	return etID_YES;
}


etID_STATE			etDBSQLiteQueryDataAdd( etDBSQLite *etDBSQLiteActual, etDB *etDBActual ){
// Check
	etCheckNull( etDBSQLiteActual );
	etCheckNull( etDBActual );
	etCheckNull( etDBActual->nodeTable );

// Load the column-pointer
	etDBColumnInit( etDBActual );
	etCheckNull( etDBActual->nodeColumns );

// Vars
	etID_STATE			tempState = etID_YES;
	etID_STATE			hasColumn = etID_NO;
	etString			*query = etDBSQLiteActual->query;
	etString			*queryValues = NULL;
	const char			*tempChar = NULL;
	const char			*tableName;
	const char			*columnName;
	const char			*columnValue;
	const char			*columnValueNew;

// Allocate memory for query values
	etStringAllocLen( queryValues, 128 );

// Basic query
	etStringCharSet( query, "INSERT INTO \0", 13 );
	etStringCharSet( queryValues, " VALUES ( \0", 11 );
	 

// Get table-name
	tempState = etDBTableNameGet(etDBActual, tableName,tableName);
	if( tableName == NULL ) return etID_STATE_ERROR_INTERNAL;
	etStringCharAdd( query, "'" );
	etStringCharAdd( query, tableName );
	etStringCharAdd( query, "'" );



// Columns
	hasColumn = etID_NO;
	etStringCharAdd( query, " ( " );

	tempState = etDBColumnGetFirst( etDBActual );
	while( tempState == etID_YES ){

	// get Column Name
		tempState = etDBColumnNameGet( etDBActual, columnName, columnName );
		if( columnName != NULL ){

		// has column an new value ?
			etDBValueGet( etDBActual, columnName, columnValue, columnValueNew );
			if( columnValueNew != NULL ){
				
			// comma
				if( hasColumn == etID_YES ){
					etStringCharAdd( query, ", " );
					etStringCharAdd( queryValues, ", " );
				}
				hasColumn = etID_YES;
				
			// column name
				etStringCharAdd( query, "'" );
				etStringCharAdd( query, columnName );
				etStringCharAdd( query, "' " );

			// column name
				etStringCharAdd( queryValues, "'" );
				etStringCharAdd( queryValues, columnValueNew );
				etStringCharAdd( queryValues, "' " );

			}

		}

	// next column
		tempState = etDBColumnGetNext( etDBActual );
	}

	etStringCharAdd( query, " ) \0" );
	etStringCharAdd( queryValues, " ) \0" );
	
	etStringCharGet( queryValues, tempChar );
	etStringCharAdd( query, tempChar );
	


	return etID_YES;
}


etID_STATE			etjDBSQliteQueryDataGet( etDBSQLite *etDBSQLiteActual, etDB *etDBActual ){
// Check
	etCheckNull( etDBSQLiteActual );
	etCheckNull( etDBActual );
	etCheckNull( etDBActual->nodeTable );

// Vars
	etString			*query = etDBSQLiteActual->query;
	const char			*tableName;


// Basic query
	etStringCharSet( query, "SELECT * FROM \0", 15 );

// Get table-name
	etDBTableNameGet(etDBActual, tableName, tableName);
	if( tableName == NULL ) return etID_STATE_ERROR_INTERNAL;
	etStringCharAdd( query, "'" );
	etStringCharAdd( query, tableName );
	etStringCharAdd( query, "'" );

// Where
	int filterCount = 0;
	int filterIndex = 0;

	etID_TYPE groupMode;
	int filterGroupOld = 0;
	int filterGroup = 0;
	const char *filterColumn;
	etDBFilterMode compareMode;
	const char *filterValue;
	
	filterCount = etDBFilterCount( etDBActual );
	
	if( filterCount <= 0 ){
		return etID_YES;
	}


	etStringCharAdd( query, " WHERE " );
	
	for( filterIndex = 0; filterIndex < filterCount; filterIndex++ ){
		

		
		etDBFilterGet( etDBActual, filterIndex, &groupMode, &filterGroup, &filterColumn, &compareMode, &filterValue );
		
	// If the filter is in the same group
		if( filterGroupOld == filterGroup ){
			switch( groupMode ){
				case etID_TYPE_AND:
					etStringCharAdd( query, " AND " );
					break;
				
				default:
					etStringCharAdd( query, " AND " );
			}
		}

	
		if( filterGroupOld != filterGroup ){
			if( filterGroupOld != 0 ){
				etStringCharAdd( query, " )" );
				
				switch( groupMode ){
					case etID_TYPE_AND:
						etStringCharAdd( query, " AND " );
						break;
					
					default:
						etStringCharAdd( query, " AND " );
				}

			}
			etStringCharAdd( query, " ( " );
			filterGroupOld = filterGroup;
		}
		
		etStringCharAdd( query, " \"" );
		etStringCharAdd( query, filterColumn );
		etStringCharAdd( query, "\" " );
	
		switch( compareMode ){
			case etDB_FILTER_EQUAL:
				etStringCharAdd( query, "==" );
				break;
				
			case etDB_FILTER_ISNULL:
				etStringCharAdd( query, "IS NULL" );
				break;
			
			default:
				etStringCharAdd( query, "==" );
		}
		
		if( compareMode != etDB_FILTER_ISNULL 
			&& compareMode != etDB_FILTER_NOTNULL 
		){
			etStringCharAdd( query, " '" );
			etStringCharAdd( query, filterValue );
			etStringCharAdd( query, "' " );
		}
		
	}
	
	etStringCharAdd( query, " )" );


	return etID_YES;
}


etID_STATE			etjDBSQliteQueryWhereAdd( etDBSQLite *etDBSQLiteActual, etDB *etDBActual ){
// Check
	etCheckNull( etDBSQLiteActual );
	etCheckNull( etDBActual );
	etCheckNull( etDBActual->nodeTable );

// Vars
	etString				*query = etDBSQLiteActual->query;	
	int						filterCount;
	int						filterIndex;
	int 					filterGroup;
	const char 			*filterColumn;
	etDBFilterMode 		filterCompareMode;
	const char 			*filterValue;
	
	filterCount = etDBFilterCount(etDBActual);
	
}



// Additional Actions after query was run
etID_STATE			etDBSQLiteTableRead( etDBSQLite *etDBSQLiteActual, etDB *etDBActual ){
// Check
	etCheckNull( etDBSQLiteActual );
	etCheckNull( etDBActual );
	etCheckNull( etDBActual->nodeTable );
	if( etDBSQLiteActual->dbSQLiteState != SQLITE_ROW ){
		etDebugMessage( etID_LEVEL_WARNING, "No Data, which means the query is broken or the table don't exist" );
		return etID_STATE_NODATA;
	}

// Vars
	int					dataCount = sqlite3_data_count( etDBSQLiteActual->dbSqliteResult );
	const char			*tableName;
	const char			*columnName;
	const char			*SQLiteColumnType;
	int					sqliteDataNotNull = 0;
	int					sqliteDataPK = 0;
	
	etDBColumnType		columnType;

// Set that the table is inside the db
	if( dataCount > 0 ){
		etDBTableNameGet( etDBActual, tableName, tableName );
		etDBTableNameOriginalSet( etDBActual, tableName );
	}


// Init the column struct
	etDBColumnInit( etDBActual );


	while( etDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){

		columnName = (const char*)sqlite3_column_text( etDBSQLiteActual->dbSqliteResult, 1 );
		SQLiteColumnType = (const char*)sqlite3_column_text( etDBSQLiteActual->dbSqliteResult, 2 );
		sqliteDataNotNull = sqlite3_column_int( etDBSQLiteActual->dbSqliteResult, 3 );
		sqliteDataPK = sqlite3_column_int( etDBSQLiteActual->dbSqliteResult, 5 );

	// Try to find the column in our etDB description
		if( etDBColumnSeek( etDBActual, columnName ) == etID_YES ){
			etDBColumnNameOriginalSet( etDBActual, columnName );
		} else {
			etDBColumnAppend( etDBActual, columnName );
			etDBColumnNameOriginalSet( etDBActual, columnName );
		}

	// Get the ColumnType
		columnType = etDBSQLiteColumnTypeGet( SQLiteColumnType );
		etDBColumnTypeOriginalSet( etDBActual, columnType );
		
	// Set primary key
		if( sqliteDataPK > 0 ){
			etDBColumnIdentifierSet( etDBActual );
		}


		etDBSQLiteActual->dbSQLiteState = sqlite3_step( etDBSQLiteActual->dbSqliteResult );
	}



	return etID_YES;
}


etID_STATE			etDBSQLiteTablesGet( etDBSQLite *etDBSQLiteActual, etDB *etDBActual ){
// Check
	etCheckNull( etDBSQLiteActual );
	etCheckNull( etDBActual );
	etCheckNull( etDBActual->nodeTable );
	if( etDBSQLiteActual->dbSQLiteState != SQLITE_ROW ){
		etDebugMessage( etID_LEVEL_WARNING, "No Data, which means the query is broken or the table don't exist" );
		return etID_STATE_NODATA;
	}

// Vars
	const char			*tableName;

	while( etDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){

		tableName = (const char*)sqlite3_column_text( etDBSQLiteActual->dbSqliteResult, 0 );

	// Try to find the column in our etDB description
		if( etDBTableSeek( etDBActual, tableName ) == etID_YES ){
			etDBTableNameOriginalSet( etDBActual, tableName );
		} else {
			etDBTableAppend( etDBActual, tableName );
			etDBTableNameOriginalSet( etDBActual, tableName );
		}

		etDBSQLiteActual->dbSQLiteState = sqlite3_step( etDBSQLiteActual->dbSqliteResult );
	}


	return etID_YES;
}


etID_STATE			etjDBSQliteDataGet( etDBDriver *etjDBDriverActual, etDB *etDBActual ){
// Check
	etCheckNull( etjDBDriverActual );
	etCheckNull( etDBActual );
	etCheckNull( etDBActual->nodeTable );


// Vars
	etDBSQLite 		*etDBSQLiteActual = (etDBSQLite*)etjDBDriverActual;
	if( etDBSQLiteActual->dbSQLiteState != SQLITE_ROW ){
		etDebugMessage( etID_LEVEL_DETAIL_DB, "No more Data" );
		return etID_STATE_NODATA;
	}

	int					dataCount = sqlite3_data_count( etDBSQLiteActual->dbSqliteResult );
	int					dataIndex;
	const char			*columnName;
	const char			*columnValue;

// Init the columnValues
	etDBValueInit( etDBActual );
	etDBValuesClean( etDBActual );

// next result	
	if( etDBSQLiteActual->sqliteResultIndex > 0 ){
		etDBSQLiteActual->dbSQLiteState = sqlite3_step( etDBSQLiteActual->dbSqliteResult );
		if( etDBSQLiteActual->dbSQLiteState != SQLITE_ROW ){
			return etID_NO;
		}		
	}

	if( etDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){

		for( dataIndex = 0; dataIndex < dataCount; dataIndex++ ){
			columnName = sqlite3_column_name( etDBSQLiteActual->dbSqliteResult, dataIndex );
			columnValue = (const char*)sqlite3_column_text( etDBSQLiteActual->dbSqliteResult, dataIndex );
			etDBValueOriginalSet( etDBActual, columnName, columnValue );
		}
		
		etDBSQLiteActual->sqliteResultIndex++;
	}
	
#ifndef ET_DEBUG_OFF
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Data readed #%i", etDBSQLiteActual->sqliteResultIndex );
	etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif


	return etID_YES;
}


// API

etID_STATE			etDBSQLiteRun( etDBDriver *etjDBDriverActual, etDB *etDBActual ){
// Check
	etCheckNull( etjDBDriverActual );
	etCheckNull( etDBActual );


// Vars
	etDBSQLite			*etDBSQLiteActual = (etDBSQLite*)etjDBDriverActual;
	etString			*query = etDBSQLiteActual->query;
	const char 		*queryChar;
	etjDBAction		etjDBActionActual = etDB_ACTION_NONE;
	etID_STATE			tempState;



// Get the action
	etjDBActionActual = etDBTableActionGet( etDBActual );

// Clean the query
	etStringClean( etDBSQLiteActual->query );
	
// Build the query
	switch( etjDBActionActual ){

		case etDB_ACTION_TABLE_ADD:
			if( etDBSQLiteQueryTableAdd( etDBSQLiteActual, etDBActual ) != etID_YES ){
				return etID_STATE_ERROR_INTERNAL;
			}
			break;
	
		case etDB_ACTION_TABLE_GET:
			if( etjDBSQliteQueryTableGet( etDBSQLiteActual, etDBActual ) != etID_YES ){
				return etID_STATE_ERROR_INTERNAL;
			}
			break;
			
		case etDB_ACTION_TABLE_GET_ALL:
			if( etjDBSQliteQueryGetAllTableNames( etDBSQLiteActual, etDBActual ) != etID_YES ){
				return etID_STATE_ERROR_INTERNAL;
			}
			break;
		
		case etDB_ACTION_DATA_ADD:
			if( etDBSQLiteQueryDataAdd( etDBSQLiteActual, etDBActual ) != etID_YES ){
				return etID_STATE_ERROR_INTERNAL;
			}
			break;
			
		case etDB_ACTION_DATA_GET:
			if( etjDBSQliteQueryDataGet( etDBSQLiteActual, etDBActual ) != etID_YES ){
				return etID_STATE_ERROR_INTERNAL;
			}
			break;

		case etDB_ACTION_DONE:
			etDebugMessage( etID_LEVEL_WARNING, "Already Done with this table, do nothing" );
			return etID_STATE_NODATA;

		case etDB_ACTION_NONE:
			//etDBColumnNameGet( etDBActual, columnName );
			//snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set type of column '%s' to '%i'", columnName, columnType );
			//etDebugMessage( etID_LEVEL_WARNING, etDebugTempMessage );
			return etID_STATE_NODATA;

		default:
			etDebugMessage( etID_LEVEL_WARNING, "Action is not supported." );
			return etID_STATE_NODATA;
	}

// Get the query
	etStringCharGet( query, queryChar );
	etDebugMessage( etID_LEVEL_DETAIL_DB, queryChar );

// run callback
	if( etjDBDriverActual->queryPreRun != NULL ){
		if( etjDBDriverActual->queryPreRun( etDBActual, query ) != etID_YES ){
			return etID_STATE_ERROR_INTERNAL;
		}
	}

// Clear the last statement
	sqlite3_finalize( etDBSQLiteActual->dbSqliteResult );

// Run the query
	sqlite3_prepare( 
		etDBSQLiteActual->dbSqlite, 
		queryChar, query->lengthActual+1, 
		&etDBSQLiteActual->dbSqliteResult, 
		NULL 
	);

	etDBSQLiteActual->dbSQLiteState = sqlite3_step( etDBSQLiteActual->dbSqliteResult );
	if( etDBSQLitePrintState( etDBSQLiteActual ) != etID_YES ) return etID_STATE_ERROR_INTERNAL;
	
// do something special
	switch( etjDBActionActual ){

		case etDB_ACTION_TABLE_GET:
			if( etDBSQLiteTableRead( etDBSQLiteActual, etDBActual ) != etID_YES ){
				return etID_STATE_ERROR_INTERNAL;
			}
			break;
			
		
		case etDB_ACTION_TABLE_GET_ALL:
			if( etDBSQLiteTablesGet( etDBSQLiteActual, etDBActual ) != etID_YES ){
				return etID_STATE_ERROR_INTERNAL;
			}
			break;

		case etDB_ACTION_DATA_ADD:
			// clean the "new"-value field
			etDBValuesNewClean( etDBActual );
			break;
			
		case etDB_ACTION_DATA_GET:
			etDBSQLiteActual->sqliteResultIndex = 0;
			break;

		default:
			break;
	}
	
// We are finished with this action, so set it
	etDBTableActionSet( etDBActual, etDB_ACTION_DONE );


	return etID_YES;
}


etID_STATE			etDBSQLiteDump( etDBDriver *etjDBDriverActual, etDB *etDBActual ){
// Check
	etCheckNull( etjDBDriverActual );
	etCheckNull( etDBActual );

// Vars
	etDBSQLite		*etDBSQLiteActual = (etDBSQLite*)etjDBDriverActual;

	etDBSQLiteDumpResult( etDBSQLiteActual );

	return etID_YES;
}




