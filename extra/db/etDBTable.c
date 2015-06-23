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





// Table Stuff

etID_STATE				etDBTableStructInit( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );

	etDBActual->nodeColumn = NULL;
	etDBActual->nodeColumns = NULL;
	etDBActual->nodeColumnsIterator = NULL; 
	
	etDBActual->nodeValuesNew = NULL;
	etDBActual->nodeValuesNewIterator = NULL;
	etDBActual->nodeValues = NULL;
	etDBActual->nodeValuesIterator = NULL;

	etDBActual->nodeValuesFilters = NULL;
	etDBActual->nodeValuesFilter = NULL;
	etDBActual->nodeValuesIterator = NULL;

	
	return etID_YES;
}


etID_STATE				etDBTableAppend( etDB *etDBActual, const char *tableName ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeTables );

// Vars
	int 				jsonReturnCode = 0;


// Create a new table
	etDBActual->nodeTable = json_object();
	
// Append table to tables
	jsonReturnCode |= json_object_set_new( etDBActual->nodeTables, tableName, etDBActual->nodeTable );

// Table update
	etDBTableStructInit( etDBActual );


// table-name
	jsonReturnCode |= json_object_set_new( etDBActual->nodeTable, "nameNew", json_string(tableName) );


// Check return code
	if( jsonReturnCode != 0 ){
		etDebugMessage( etID_LEVEL_ERR, "jansson error" );
		return etID_STATE_ERROR_INTERNAL;
	}

#ifndef ET_DEBUG_OFF
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Append table %s", tableName );
	etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif

// return
	return etID_YES;
}


etID_STATE				etDBTableSeek( etDB *etDBActual, const char *tableName ){
// Check
	etCheckNull( etDBActual );
	etCheckNull( tableName );
	etCheckParameterSequence( etDBActual->nodeTables );

// vars
	json_t			*table;

// Get table
	table = json_object_get( etDBActual->nodeTables, tableName );
	if( table == NULL ) return etID_STATE_NODATA;

// Get Data from Table
	etDBActual->nodeTable = table;
	etDBTableStructInit( etDBActual );

	return etID_YES;
}


etID_STATE				etDBTableGetFirst( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeTables );

	etDBActual->nodeColumnsIterator = NULL;

// Set the iterator to the first column
	etDBActual->nodeTablesIterator = json_object_iter( etDBActual->nodeTables );
	if( etDBActual->nodeTablesIterator == NULL ) return etID_STATE_NODATA;

// Get table from iter
	etDBActual->nodeTable = json_object_iter_value( etDBActual->nodeTablesIterator );

// Get Data from Table
	etDBTableStructInit( etDBActual );

// return
	return etID_YES;
}


etID_STATE				etDBTableGetNext( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeTables );


// Set the iterator to the first column
	etDBActual->nodeTablesIterator = json_object_iter_next( etDBActual->nodeTables, etDBActual->nodeTablesIterator );
	if( etDBActual->nodeTablesIterator == NULL ) return etID_STATE_NODATA;

// Get table from iter
	etDBActual->nodeTable = json_object_iter_value( etDBActual->nodeTablesIterator );

// Get Data from Table
	etDBTableStructInit( etDBActual );

// return
	return etID_YES;
}




etID_STATE				etDBTableNameOriginalSet( etDB *etDBActual, const char *tableName ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeTable );

// Vars
	int 				jsonReturnCode = 0;

// table-name
	jsonReturnCode |= json_object_set_new( etDBActual->nodeTable, "name", json_string(tableName) );


// Check return code
	if( jsonReturnCode != 0 ){
		etDebugMessage( etID_LEVEL_ERR, "jansson error" );
		return etID_STATE_ERROR_INTERNAL;
	}

	return etID_YES;
}


etID_STATE				__etDBTableNameGet( etDB *etDBActual, const char **p_tableName, const char **p_tableNameNew ){
	return __etDBStringGet( etDBActual->nodeTable, "name", "nameNew", p_tableName, p_tableNameNew );
}


etID_STATE				etDBTableVisibleNameSet( etDB *etDBActual, const char *visibleName ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeTable );


// Vars
	int 				jsonReturnCode = 0;


// Add table names
	jsonReturnCode |= json_object_set_new( etDBActual->nodeTable, "visibleName", json_string(visibleName) );

// Check return code
	if( jsonReturnCode != 0 ){
		etDebugMessage( etID_LEVEL_ERR, "jansson error" );
		return etID_STATE_ERROR_INTERNAL;
	}

// return
	return etID_YES;
}


etID_STATE				__etDBTableVisibleNameGet( etDB *etDBActual, const char **p_visibleName ){
	return __etDBStringGet( etDBActual->nodeTable, "visibleName", NULL, p_visibleName, p_visibleName );
}




etID_STATE				etDBTableActionSet( etDB *etDBActual, etjDBAction etjDBActionNew ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeTable );

// Add table names
	json_object_set_new( etDBActual->nodeTable, "action", json_integer(etjDBActionNew) );


#ifndef ET_DEBUG_OFF
	const char *tableName;
	etDBTableNameGet( etDBActual, tableName, tableName );
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set action for table %s to %i", tableName, etjDBActionNew );
	etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif

	return etID_YES;
}


etjDBAction			etDBTableActionGet( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeTable );

// Vars
	json_t				*jsonAction;
	etjDBAction			etjDBActionActual = etDB_ACTION_NONE;

// Get the json-value
	jsonAction = json_object_get( etDBActual->nodeTable, "action" );
	if( jsonAction != NULL ){
	// get the value itselfe
		etjDBActionActual = json_integer_value( jsonAction );
	}

	return etjDBActionActual;
}








