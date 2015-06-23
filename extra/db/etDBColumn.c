/* etDBTable - evillib json Table functions
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
	
	#include "etDB.h"
	#include "etDBTable.h"
	#include "etDBColumn.h"
#endif



// Column set
etID_STATE				etDBColumnInit( etDB *etDBActual ){
// Get columns from json-object
	if( etDBActual->nodeColumns == NULL ){
		etDBActual->nodeColumns = json_object_get( etDBActual->nodeTable, "columns" );
	}
	return etID_YES;
}


etID_STATE				etDBColumnAppend( etDB *etDBActual, const char *columnName ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeTable );

// Try to load columns from json
	if( etDBColumnInit( etDBActual ) != etID_YES ) return etID_STATE_ERROR_INTERNAL;

// If no columns exist
	if( etDBActual->nodeColumns == NULL ){
		etDBActual->nodeColumns = json_object();
		etDBActual->nodeColumn = NULL;
		etDBActual->nodeColumnsIterator = NULL;
		json_object_set_new( etDBActual->nodeTable, "columns", etDBActual->nodeColumns );
	}

// Vars
	int 				jsonReturnCode = 0;


// Create Column
	etDBActual->nodeColumn = json_object();
	jsonReturnCode |= json_object_set_new( etDBActual->nodeColumn, "nameNew", json_string(columnName) );

// Add column to columns
	jsonReturnCode |= json_object_set_new( etDBActual->nodeColumns, columnName, etDBActual->nodeColumn );
	
// Check return code
	if( jsonReturnCode != 0 ){
		etDebugMessage( etID_LEVEL_ERR, "jansson error" );
		return etID_STATE_ERROR_INTERNAL;
	}

#ifndef ET_DEBUG_OFF
	const char *tableName;
	etDBTableNameGet( etDBActual, tableName, tableName );
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Append column '%s' to table '%s'", columnName, tableName );
	etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif

// return
	return etID_YES;
}


etID_STATE				etDBColumnSeek( etDB *etDBActual, const char *columnName ){
// Check
	etCheckNull( etDBActual );
	etCheckNull( columnName );

// Check if column-struct is init
	if( etDBColumnInit( etDBActual ) != etID_YES ) return etID_STATE_ERROR_INTERNAL;

// vars
	json_t			*jsonColumn;

// try to find the column
	jsonColumn = json_object_get( etDBActual->nodeColumns, columnName );

// We found it
	if( jsonColumn != NULL ){
		#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "Column Found %s", columnName );
			etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
		#endif
		
	// return
		etDBActual->nodeColumn = jsonColumn;
		return etID_YES;
	}

// Didnt found the column
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Column not Found %s", columnName );
	etDebugMessage( etID_LEVEL_WARNING, etDebugTempMessage );

	return etID_STATE_NODATA;
}


etID_STATE				etDBColumnGetFirst( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeColumns );


// Set the iterator to the first column
	etDBActual->nodeColumnsIterator = json_object_iter( etDBActual->nodeColumns );
	if( etDBActual->nodeColumnsIterator == NULL ) return etID_STATE_NODATA;
	
	etDBActual->nodeColumn = json_object_iter_value( etDBActual->nodeColumnsIterator );

// return
	return etID_YES;
}


etID_STATE				etDBColumnGetNext( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeColumns );


// Set the iterator to the first column
	etDBActual->nodeColumnsIterator = json_object_iter_next( etDBActual->nodeColumns, etDBActual->nodeColumnsIterator );
	if( etDBActual->nodeColumnsIterator == NULL ) return etID_STATE_NODATA;
	
	etDBActual->nodeColumn = json_object_iter_value( etDBActual->nodeColumnsIterator );

// return
	return etID_YES;
}




etID_STATE				etDBColumnNameOriginalSet( etDB *etDBActual, const char *columnName ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeColumn );

// Vars
	int 				jsonReturnCode = 0;

// table-name
	jsonReturnCode |= json_object_set_new( etDBActual->nodeColumn, "name", json_string(columnName) );


// Check return code
	if( jsonReturnCode != 0 ){
		etDebugMessage( etID_LEVEL_ERR, "jansson error" );
		return etID_STATE_ERROR_INTERNAL;
	}

	return etID_YES;
}


etID_STATE				__etDBColumnNameGet( etDB *etDBActual, const char **p_columnName, const char **p_columnNameNew ){
	return __etDBStringGet( etDBActual->nodeColumn, "name", "nameNew", p_columnName, p_columnNameNew );
}


etID_STATE				etDBColumnTypeSet( etDB *etDBActual, etDBColumnType columnType ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeColumn );

#ifndef ET_DEBUG_OFF
	const char *columnName;
	etDBColumnNameGet( etDBActual, columnName, columnName );
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set type of column '%s' to '%i'", columnName, columnType );
	etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif

	json_object_set_new( etDBActual->nodeColumn, "typeNew", json_integer(columnType) );

	return etID_YES;
}


etID_STATE				etDBColumnTypeOriginalSet( etDB *etDBActual, etDBColumnType columnType ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeColumn );

#ifndef ET_DEBUG_OFF
	const char *columnName;
	etDBColumnNameGet( etDBActual, columnName, columnName );
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set real(in DB) type of column '%s' to '%i'", columnName, columnType );
	etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif

	json_object_set_new( etDBActual->nodeColumn, "type", json_integer(columnType) );

	return etID_YES;
}


etID_STATE				__etDBColumnTypeGet( etDB *etDBActual, etDBColumnType *p_columnType ){
	return __etDBIntegerGet( etDBActual->nodeColumn, "type", "typeNew", (int*)p_columnType );
}




etID_STATE				etDBColumnIdentifierSet( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeColumn );

// vars 
	int 			jsonReturnCode = 0;
	json_t*		jsonIdentifier = NULL;


// set that this is the identifier
	json_object_set_new( etDBActual->nodeColumn, "isIdentifier", json_integer(1) );


#ifndef ET_DEBUG_OFF
	const char *columnName;
	etDBColumnNameGet( etDBActual, columnName, columnName );
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set identifier of column '%s' to 1", columnName );
	etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif

	return etID_YES;
}


etID_STATE				etDBColumnIsIdentifier( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeColumn );

// vars 
	int 			jsonReturnCode = 0;
	json_t*		jsonIdentifier = NULL;
	int				isIdentifier = NULL;

// try to get the identifyer
	jsonIdentifier = json_object_get( etDBActual->nodeColumn, "isIdentifier" );
	if( jsonIdentifier == NULL ){
		return etID_NO;
	}
	
	isIdentifier = json_integer_value( jsonIdentifier );
	if( isIdentifier != 1 ){
		return etID_NO;
	}

	return etID_YES;
}

