/* etjDB - evillib json DB representation
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

	#include "../db/etjDB.h"
	#include "etjDBDriver.h"
	#include "../db/etjDBTable.h"
	#include "../db/etjDBColumn.h"
	#include "../db/etjDBValue.h"
	#include "etjDBSQLite.h"
#endif


const char *etjDB_TYPE_SQLITE[etjDB_TYPE_SQLITE_MAX] = {
	"TEXT\0" ,
	"TEXT\0"
};
const char *etjDB_COMP_SQLITE[etjDB_COMP_SQLITE_MAX] = {
	"==\0",
	"!=\0"
};

etID_STATE			etjDBSQLiteRun( etjDBDriver *etjDBDriverActual, etjDB *etjDBActual );
etID_STATE			etjDBSQLiteDump( etjDBDriver *etjDBDriverActual, etjDB *etjDBActual );
etID_STATE			etjDBSQliteDataGet( etjDBSQLite *etjDBSQLiteActual, etjDB *etjDBActual );

/**
@todo Check size of driver-type
*/
etID_STATE			__etjDBSQLiteInit( etjDBDriver **p_etjDBDriverActual ){
	
// Vars
	etMemoryBlock 		*etMemoryBlockActual = NULL;
	etjDBSQLite 		*etjDBSQLiteActual = NULL;


// Allocate memory for sqlite
	etMemoryRequest( etMemoryBlockActual, sizeof(etjDBSQLite) );
	etMemoryBlockDataGet( etMemoryBlockActual, etjDBSQLiteActual );

// Allocate memory for query-string
	etStringAllocLen( etjDBSQLiteActual->query, 256 );

// Set functions
	etjDBSQLiteActual->driverClass.queryRun = etjDBSQLiteRun;
	etjDBSQLiteActual->driverClass.nextResult = etjDBSQliteDataGet;
	etjDBSQLiteActual->driverClass.dump = etjDBSQLiteDump;

// return
	*p_etjDBDriverActual = (etjDBDriver*)etjDBSQLiteActual;
	return etID_YES;
}


etID_STATE			etjDBSQLiteConnect( etjDBDriver *etjDBDriverActual, const char *path ){
// Check
	etCheckNull( etjDBDriverActual );
	
// Vars
	etjDBSQLite 		*etjDBSQLiteActual = (etjDBSQLite*)etjDBDriverActual;

#ifndef ET_DEBUG_OFF
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Connecting to %s...", path );
	etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif
		
// Open an sqlite database
	if( sqlite3_open( path, &etjDBSQLiteActual->dbSqlite ) != SQLITE_OK ){
		etDebugMessage( etID_LEVEL_ERR, sqlite3_errmsg(etjDBSQLiteActual->dbSqlite) );
		return etID_STATE_ERROR_INTERNAL;
	}

	return etID_YES;
}




// Internal functions
etID_STATE			etjDBSQLitePrintState( etjDBSQLite *etjDBSQLiteActual ){
	
	if( etjDBSQLiteActual->dbSQLiteState == SQLITE_OK ){
		etDebugMessage( etID_LEVEL_DETAIL_DB, "Everything is OK with SQLITE" );
		return etID_YES;
	}
	if( etjDBSQLiteActual->dbSQLiteState == SQLITE_DONE ){
		etDebugMessage( etID_LEVEL_DETAIL_DB, "SQLITE is done with query" );
		return etID_YES;
	}
	
	if( etjDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){
		etDebugMessage( etID_LEVEL_DETAIL_DB, "SQLITE has data for you" );
		return etID_YES;
	}

	etDebugMessage( etID_LEVEL_ERR, sqlite3_errmsg( etjDBSQLiteActual->dbSqlite ) );
	return etID_LEVEL_ERR;
}


void				etjDBSQLiteDumpResult( etjDBSQLite *etjDBSQLiteActual ){
	
// Vars

	int			dataIndex = 0;
	int			dataCount = sqlite3_data_count( etjDBSQLiteActual->dbSqliteResult );
	
	if( etjDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){
		
		while( etjDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){

			etDebugMessage( etID_LEVEL_DETAIL_DB, "[ROW]" );
			
			for( dataIndex = 0; dataIndex < dataCount; dataIndex++ ){
				const unsigned char *columnTest = sqlite3_column_text( etjDBSQLiteActual->dbSqliteResult, dataIndex );
				etDebugMessage( etID_LEVEL_DETAIL_DB, (const char*)columnTest );
			}
			
			etjDBSQLiteActual->dbSQLiteState = sqlite3_step( etjDBSQLiteActual->dbSqliteResult );

		}

	}


}


etjDBColumnType	etjDBSQLiteMapColumnType( const char *SQLiteColumnType ){
	
	int columnTypeIndex;
	int columnTypeMax = etjDB_TYPE_SQLITE_MAX;
	
// We start with 1, because 0 is the default column type
	for( columnTypeIndex = 1; columnTypeIndex < columnTypeMax; columnTypeIndex++ ){
		
		if( strncmp( etjDB_TYPE_SQLITE[columnTypeIndex], SQLiteColumnType, 50) == 0 ){
			return columnTypeIndex;
		}
		
	}
	
	return etjDB_COL_TYPE_DEFAULT;
}



// Build-Query-Functions
etID_STATE			etjDBSQLiteQueryTableAdd( etjDBSQLite *etjDBSQLiteActual, etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBSQLiteActual );
	etCheckNull( etjDBActual );
	etCheckNull( etjDBActual->nodeTable );
	etCheckNull( etjDBActual->nodeColumns );

	
// Vars
	etID_STATE			tempState = etID_NO;

	etString			*query = etjDBSQLiteActual->query;
	etjDBColumnType		columnType;

	const char			*tableName;
	const char			*columnName;
	const char			*columnTypeName;


// Basic query
	etStringCharSet( query, "CREATE TABLE IF NOT EXISTS ", 27 );

// Table name
	tempState = etjDBTableNameGet(etjDBActual, tableName);
	if( tempState != etID_YES && tempState != etID_STATE_NEW ) return etID_STATE_ERROR_INTERNAL;
	etStringCharAdd( query, "'" );
	etStringCharAdd( query, tableName );
	etStringCharAdd( query, "'" );

// Columns	
	etStringCharAdd( query, "( " );

	tempState = etjDBColumnGetFirst( etjDBActual );
	while( tempState == etID_YES ){

	// get Column Name
		tempState = etjDBColumnNameGet( etjDBActual, columnName );
		if( tempState == etID_YES || tempState == etID_STATE_NEW ){

		// column name
			etStringCharAdd( query, "'" );
			etStringCharAdd( query, columnName );
			etStringCharAdd( query, "' " );

		// column type
			tempState = etjDBColumnTypeGet( etjDBActual, columnType );
			if( tempState == etID_STATE_NODATA ){
				etDebugMessage( etID_LEVEL_WARNING, "Column Type is not set in etjDB, we use the default one" );
				columnType = etjDB_COL_TYPE_DEFAULT;
			}
				
			columnTypeName = etjDB_TYPE_SQLITE[columnType];
			etStringCharAdd( query, columnTypeName );
			
		}


	// next column
		tempState = etjDBColumnGetNext( etjDBActual );
		if( tempState == etID_YES  ){
			etStringCharAdd( query, ", " );
		}

	}

	etStringCharAdd( query, " );\0" );


	return etID_YES;
}


etID_STATE			etjDBSQliteQueryTableGet( etjDBSQLite *etjDBSQLiteActual, etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBSQLiteActual );
	etCheckNull( etjDBActual );
	etCheckNull( etjDBActual->nodeTable );


// Vars
	etID_STATE		returnState = etID_YES;
	etString		*query = etjDBSQLiteActual->query;
	const char		*tableName;

// Table name
	returnState = etjDBTableNameGet(etjDBActual, tableName);
	if( returnState != etID_STATE_NEW && returnState != etID_YES ) return etID_STATE_ERROR_INTERNAL;

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


etID_STATE			etjDBSQliteQueryDataAdd( etjDBSQLite *etjDBSQLiteActual, etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBSQLiteActual );
	etCheckNull( etjDBActual );
	etCheckNull( etjDBActual->nodeTable );

// Vars
	etID_STATE			tempState = etID_YES;
	etID_STATE			hasColumn = etID_NO;
	etString			*query = etjDBSQLiteActual->query;
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
	tempState = etjDBTableNameGet(etjDBActual, tableName);
	if( tableName == NULL ) return etID_STATE_ERROR_INTERNAL;
	etStringCharAdd( query, "'" );
	etStringCharAdd( query, tableName );
	etStringCharAdd( query, "'" );



// Columns
	hasColumn = etID_NO;
	etStringCharAdd( query, " ( " );

	tempState = etjDBColumnGetFirst( etjDBActual );
	while( tempState == etID_YES ){

	// get Column Name
		tempState = etjDBColumnNameGet( etjDBActual, columnName );
		if( columnName != NULL ){

		// has column an new value ?
			etjDBValueGet( etjDBActual, columnName, columnValue );
			if( columnValue != NULL ){
				
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
				etStringCharAdd( queryValues, columnValue );
				etStringCharAdd( queryValues, "' " );

			}

		}

	// next column
		tempState = etjDBColumnGetNext( etjDBActual );
	}

	etStringCharAdd( query, " ) \0" );
	etStringCharAdd( queryValues, " ) \0" );
	
	etStringCharGet( queryValues, tempChar );
	etStringCharAdd( query, tempChar );
	


	return etID_YES;
}


etID_STATE			etjDBSQliteQueryDataGet( etjDBSQLite *etjDBSQLiteActual, etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBSQLiteActual );
	etCheckNull( etjDBActual );
	etCheckNull( etjDBActual->nodeTable );

// Vars
	etID_STATE			tempState = etID_YES;
	etID_STATE			hasColumn = etID_NO;
	etString			*query = etjDBSQLiteActual->query;
	const char			*tableName;
	const char			*columnName;
	const char			*columnValue;
	const char			*columnValueNew;

// Basic query
	etStringCharSet( query, "SELECT * FROM \0", 15 );

// Get table-name
	tempState = etjDBTableNameGet(etjDBActual, tableName);
	if( tempState != etID_YES && tempState != etID_STATE_NEW ) return etID_STATE_ERROR_INTERNAL;
	etStringCharAdd( query, "'" );
	etStringCharAdd( query, tableName );
	etStringCharAdd( query, "';" );


	return etID_YES;
}




// Additional Actions after query was run
etID_STATE			etjDBSQLiteTableValidate( etjDBSQLite *etjDBSQLiteActual, etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBSQLiteActual );
	etCheckNull( etjDBActual );
	etCheckNull( etjDBActual->nodeTable );
	if( etjDBSQLiteActual->dbSQLiteState != SQLITE_ROW ){
		etDebugMessage( etID_LEVEL_WARNING, "No Data, which means the query is broken or the table don't exist" );
		return etID_STATE_NODATA;
	}

// Vars
	int					dataCount = sqlite3_data_count( etjDBSQLiteActual->dbSqliteResult );
	const char			*columnName;
	const char			*SQLiteColumnType;
	etjDBColumnType	columnType;

// Set that the table is inside the db
	if( dataCount > 0 ){
		etjDBTableInDBSet( etjDBActual );
	}


	while( etjDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){

		columnName = (const char*)sqlite3_column_text( etjDBSQLiteActual->dbSqliteResult, 1 );
		SQLiteColumnType = (const char*)sqlite3_column_text( etjDBSQLiteActual->dbSqliteResult, 2 );

	// Try to find the column in our etjDB description
		if( etjDBColumnSeek( etjDBActual, columnName ) == etID_YES ){
			etjDBColumnInDBSet( etjDBActual );
		} else {
			etjDBColumnAppend( etjDBActual, columnName );
			etjDBColumnInDBSet( etjDBActual );
		}

	// Get the ColumnType
		columnType = etjDBSQLiteMapColumnType( SQLiteColumnType );
		etjDBColumnTypeSet( etjDBActual, columnType );
		etjDBColumnTypeInDBSet( etjDBActual );


		etjDBSQLiteActual->dbSQLiteState = sqlite3_step( etjDBSQLiteActual->dbSqliteResult );
	}



	return etID_YES;
}


etID_STATE			etjDBSQliteDataGet( etjDBSQLite *etjDBSQLiteActual, etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBSQLiteActual );
	etCheckNull( etjDBActual );
	etCheckNull( etjDBActual->nodeTable );
	if( etjDBSQLiteActual->dbSQLiteState != SQLITE_ROW ){
		etDebugMessage( etID_LEVEL_WARNING, "No Data" );
		return etID_STATE_NODATA;
	}

// Vars
	int					dataCount = sqlite3_data_count( etjDBSQLiteActual->dbSqliteResult );
	int					dataIndex;
	const char			*columnName;
	const char			*columnValue;
	
	if( etjDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){

		for( dataIndex = 0; dataIndex < dataCount; dataIndex++ ){
			columnName = sqlite3_column_name( etjDBSQLiteActual->dbSqliteResult, dataIndex );
			columnValue = (const char*)sqlite3_column_text( etjDBSQLiteActual->dbSqliteResult, dataIndex );
			etjDBValueSetValid( etjDBActual, columnName, columnValue );
		}
	}			

// next result	
	etjDBSQLiteActual->dbSQLiteState = sqlite3_step( etjDBSQLiteActual->dbSqliteResult );
	if( etjDBSQLiteActual->dbSQLiteState == SQLITE_ROW ){
		return etID_YES;
	}

	return etID_NO;
}


// API

etID_STATE			etjDBSQLiteRun( etjDBDriver *etjDBDriverActual, etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBDriverActual );
	etCheckNull( etjDBActual );


// Vars
	etjDBSQLite		*etjDBSQLiteActual = (etjDBSQLite*)etjDBDriverActual;
	etString			*query = etjDBSQLiteActual->query;
	const char 		*queryChar;
	etjDBAction		etjDBActionActual = etjDB_ACTION_NONE;
	etID_STATE			tempState;

	tempState = etjDBTableGetFirst(etjDBActual);
// Go to all tables
	while( tempState == etID_YES ){

	// Get the action
		etjDBActionActual = etjDBTableActionGet( etjDBActual );

	// Clean the query
		etStringClean( etjDBSQLiteActual->query );
		
	// Build the query
		switch( etjDBActionActual ){

			case etjDB_ACTION_TABLE_ADD:
				if( etjDBSQLiteQueryTableAdd( etjDBSQLiteActual, etjDBActual ) != etID_YES ){
					return etID_STATE_ERROR_INTERNAL;
				}
				break;
		
			case etjDB_ACTION_TABLE_GET:
				if( etjDBSQliteQueryTableGet( etjDBSQLiteActual, etjDBActual ) != etID_YES ){
					return etID_STATE_ERROR_INTERNAL;
				}
				break;
			
			case etjDB_ACTION_DATA_ADD:
				if( etjDBSQliteQueryDataAdd( etjDBSQLiteActual, etjDBActual ) != etID_YES ){
					return etID_STATE_ERROR_INTERNAL;
				}
				break;
				
			case etjDB_ACTION_DATA_GET:
				if( etjDBSQliteQueryDataGet( etjDBSQLiteActual, etjDBActual ) != etID_YES ){
					return etID_STATE_ERROR_INTERNAL;
				}
				break;

			case etjDB_ACTION_DONE:
				etDebugMessage( etID_LEVEL_WARNING, "Already Done with this table, do nothing" );
				goto nextTable;

			case etjDB_ACTION_NONE:
				etDebugMessage( etID_LEVEL_WARNING, "No action is set, do nothing" );
				goto nextTable;

			default:
				etDebugMessage( etID_LEVEL_WARNING, "Action is not supported." );
				goto nextTable;
		}

	// Get the query
		etStringCharGet( query, queryChar );
		etDebugMessage( etID_LEVEL_DETAIL_DB, queryChar );

	// run callback
		if( etjDBDriverActual->queryPreRun != NULL ){
			if( etjDBDriverActual->queryPreRun( etjDBDriverActual, query ) != etID_YES ){
				return etID_STATE_ERROR_INTERNAL;
			}
		}

	// Clear the last statement
		sqlite3_finalize( etjDBSQLiteActual->dbSqliteResult );

	// Run the query
		sqlite3_prepare( 
			etjDBSQLiteActual->dbSqlite, 
			queryChar, query->lengthActual+1, 
			&etjDBSQLiteActual->dbSqliteResult, 
			NULL 
		);

		etjDBSQLiteActual->dbSQLiteState = sqlite3_step( etjDBSQLiteActual->dbSqliteResult );
		if( etjDBSQLitePrintState( etjDBSQLiteActual ) != etID_YES ) return etID_STATE_ERROR_INTERNAL;
		
	// do something special
		switch( etjDBActionActual ){

			case etjDB_ACTION_TABLE_GET:
				if( etjDBSQLiteTableValidate( etjDBSQLiteActual, etjDBActual ) != etID_YES ){
					return etID_STATE_ERROR_INTERNAL;
				}
				goto nextTable;
				break;


			case etjDB_ACTION_DATA_GET:
				if( etjDBSQliteDataGet( etjDBSQLiteActual, etjDBActual ) != etID_YES ){
					return etID_STATE_ERROR_INTERNAL;
				}
				break;
				

			default:
				break;
		}
		
	// We are finished with this action, so set it
		etjDBTableActionSet( etjDBActual, etjDB_ACTION_DONE );

		nextTable:
		tempState = etjDBTableGetNext( etjDBActual );
	}
	
	return etID_YES;
}


etID_STATE			etjDBSQLiteDump( etjDBDriver *etjDBDriverActual, etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBDriverActual );
	etCheckNull( etjDBActual );

// Vars
	etjDBSQLite		*etjDBSQLiteActual = (etjDBSQLite*)etjDBDriverActual;

	etjDBSQLiteDumpResult( etjDBSQLiteActual );

	return etID_YES;
}




