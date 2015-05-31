/* etjDBTable - evillib json Table functions
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
	
	#include "etjDB.h"
	#include "etjDBTable.h"
	#include "etjDBColumn.h"
#endif





// Table Stuff
etID_STATE				etjDBTableAppend( etjDB *etjDBActual, const char *tableName ){
// Check
	etCheckNull( etjDBActual );
	etCheckParameterSequence( etjDBActual->nodeTables );

// Vars
	int 				jsonReturnCode = 0;


// Create a new table
	etjDBActual->nodeTable = json_object();
	
	etjDBActual->nodeColumn = NULL;
	etjDBActual->nodeColumns = json_object();
	etjDBActual->nodeColumnsIterator = NULL;
	
	etjDBActual->nodeValuesNew = NULL;
	etjDBActual->nodeValues = NULL;

// Append table to tables
	jsonReturnCode |= json_object_set_new( etjDBActual->nodeTables, tableName, etjDBActual->nodeTable );

// table-name
	jsonReturnCode |= json_object_set_new( etjDBActual->nodeTable, "nameNew", json_string(tableName) );

// columns
	json_object_set( etjDBActual->nodeTable, "columns", etjDBActual->nodeColumns );

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


etID_STATE				etjDBTableStructInit( etjDB *etjDBActual ){
	
	
// Get Data from Table
	etjDBActual->nodeColumns = json_object_get( etjDBActual->nodeTable, "columns" );
	if( etjDBActual->nodeColumns == NULL ){
		etjDBActual->nodeColumns = json_object();
		json_object_set( etjDBActual->nodeTable, "columns", etjDBActual->nodeColumns );
	}
	
	return etID_YES;
}


etID_STATE				etjDBTableInDBSet( etjDB *etjDBActual ){
	return etjDBInDBSet( etjDBActual->nodeTable, "name", "nameNew" );
}


etID_STATE				__etjDBTableNameGet( etjDB *etjDBActual, const char **p_tableName ){
	return __etjDBStringGet( etjDBActual->nodeTable, "name", "nameNew", p_tableName );
}


etID_STATE				etjDBTableVisibleNameSet( etjDB *etjDBActual, const char *visibleName ){
// Check
	etCheckNull( etjDBActual );
	etCheckParameterSequence( etjDBActual->nodeTable );


// Vars
	int 				jsonReturnCode = 0;


// Add table names
	jsonReturnCode |= json_object_set_new( etjDBActual->nodeTable, "visibleName", json_string(visibleName) );

// Check return code
	if( jsonReturnCode != 0 ){
		etDebugMessage( etID_LEVEL_ERR, "jansson error" );
		return etID_STATE_ERROR_INTERNAL;
	}

// return
	return etID_YES;
}


etID_STATE				__etjDBTableVisibleNameGet( etjDB *etjDBActual, const char **p_visibleName ){
	return __etjDBStringGet( etjDBActual->nodeTable, "visibleName", NULL, p_visibleName );
}


etID_STATE				etjDBTableActionSet( etjDB *etjDBActual, etjDBAction etjDBActionNew ){
// Check
	etCheckNull( etjDBActual );
	etCheckParameterSequence( etjDBActual->nodeTable );

// Add table names
	json_object_set_new( etjDBActual->nodeTable, "action", json_integer(etjDBActionNew) );


#ifndef ET_DEBUG_OFF
	const char *tableName;
	etjDBTableNameGet( etjDBActual, tableName );
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set action for table %s to %i", tableName, etjDBActionNew );
	etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif

	return etID_YES;
}


etjDBAction			etjDBTableActionGet( etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBActual );
	etCheckParameterSequence( etjDBActual->nodeTable );

// Vars
	json_t				*jsonAction;
	etjDBAction			etjDBActionActual = etjDB_ACTION_NONE;

// Get the json-value
	jsonAction = json_object_get( etjDBActual->nodeTable, "action" );
	if( jsonAction != NULL ){
	// get the value itselfe
		etjDBActionActual = json_integer_value( jsonAction );
	}

	return etjDBActionActual;
}


etID_STATE				etjDBTableSeek( etjDB *etjDBActual, const char *tableName ){
// Check
	etCheckNull( etjDBActual );
	etCheckNull( tableName );
	etCheckParameterSequence( etjDBActual->nodeTables );

// Get table
	etjDBActual->nodeTable = json_object_get( etjDBActual->nodeTables, tableName );
	if( etjDBActual->nodeTable == NULL ) return etID_STATE_NODATA;

// Get Data from Table
	etjDBTableStructInit( etjDBActual );

	return etID_YES;
}


etID_STATE				etjDBTableGetFirst( etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBActual );
	etCheckParameterSequence( etjDBActual->nodeTables );

	etjDBActual->nodeColumnsIterator = NULL;

// Set the iterator to the first column
	etjDBActual->nodeTablesIterator = json_object_iter( etjDBActual->nodeTables );
	if( etjDBActual->nodeTablesIterator == NULL ) return etID_STATE_NODATA;

// Get table from iter
	etjDBActual->nodeTable = json_object_iter_value( etjDBActual->nodeTablesIterator );

// Get Data from Table
	etjDBTableStructInit( etjDBActual );

// return
	return etID_YES;
}


etID_STATE				etjDBTableGetNext( etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBActual );
	etCheckParameterSequence( etjDBActual->nodeTables );


// Set the iterator to the first column
	etjDBActual->nodeTablesIterator = json_object_iter_next( etjDBActual->nodeTables, etjDBActual->nodeTablesIterator );
	if( etjDBActual->nodeTablesIterator == NULL ) return etID_STATE_NODATA;

// Get table from iter
	etjDBActual->nodeTable = json_object_iter_value( etjDBActual->nodeTablesIterator );

// Get Data from Table
	etjDBTableStructInit( etjDBActual );

// return
	return etID_YES;
}










