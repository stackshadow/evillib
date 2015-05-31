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
	#include "etjDBColumn.h"
#endif



// Column set
etID_STATE				etjDBColumnAppend( etjDB *etjDBActual, const char *columnName ){
// Check
	etCheckNull( etjDBActual );
	etCheckParameterSequence( etjDBActual->nodeTable );
	etCheckParameterSequence( etjDBActual->nodeColumns );

// Vars
	int 				jsonReturnCode = 0;


// Create Column
	etjDBActual->nodeColumn = json_object();
	jsonReturnCode |= json_object_set_new( etjDBActual->nodeColumn, "nameNew", json_string(columnName) );

// Add column to columns
	jsonReturnCode |= json_object_set_new( etjDBActual->nodeColumns, columnName, etjDBActual->nodeColumn );
	
// Check return code
	if( jsonReturnCode != 0 ){
		etDebugMessage( etID_LEVEL_ERR, "jansson error" );
		return etID_STATE_ERROR_INTERNAL;
	}

// return
	return etID_YES;
}


etID_STATE				etjDBColumnSeek( etjDB *etjDBActual, const char *columnName ){
// Check
	etCheckNull( etjDBActual );
	etCheckNull( columnName );
	etCheckParameterSequence( etjDBActual->nodeColumns );


// try to find the column
	etjDBActual->nodeColumn = json_object_get( etjDBActual->nodeColumns, columnName );

// We found it
	if( etjDBActual->nodeColumn != NULL ){
		#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "Column Found %s", columnName );
			etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
		#endif
		
		return etID_YES;
	}

// Didnt found the column
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Column not Found %s", columnName );
	etDebugMessage( etID_LEVEL_WARNING, etDebugTempMessage );

	return etID_STATE_NODATA;
}


etID_STATE				etjDBColumnGetFirst( etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBActual );
	etCheckParameterSequence( etjDBActual->nodeColumns );


// Set the iterator to the first column
	etjDBActual->nodeColumnsIterator = json_object_iter( etjDBActual->nodeColumns );
	if( etjDBActual->nodeColumnsIterator == NULL ) return etID_STATE_NODATA;
	
	etjDBActual->nodeColumn = json_object_iter_value( etjDBActual->nodeColumnsIterator );

// return
	return etID_YES;
}


etID_STATE				etjDBColumnGetNext( etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBActual );
	etCheckParameterSequence( etjDBActual->nodeColumns );


// Set the iterator to the first column
	etjDBActual->nodeColumnsIterator = json_object_iter_next( etjDBActual->nodeColumns, etjDBActual->nodeColumnsIterator );
	if( etjDBActual->nodeColumnsIterator == NULL ) return etID_STATE_NODATA;
	
	etjDBActual->nodeColumn = json_object_iter_value( etjDBActual->nodeColumnsIterator );

// return
	return etID_YES;
}


etID_STATE				etjDBColumnInDBSet( etjDB *etjDBActual ){
	return etjDBInDBSet( etjDBActual->nodeColumn, "name", "nameNew" );
}


etID_STATE				__etjDBColumnNameGet( etjDB *etjDBActual, const char **p_columnName ){
	return __etjDBStringGet( etjDBActual->nodeColumn, "name", "nameNew", p_columnName );
}


etID_STATE				etjDBColumnTypeSet( etjDB *etjDBActual, etjDBColumnType columnType ){
// Check
	etCheckNull( etjDBActual );
	etCheckParameterSequence( etjDBActual->nodeColumn );

	json_object_set_new( etjDBActual->nodeColumn, "typeNew", json_integer(columnType) );

	return etID_YES;
}


etID_STATE				__etjDBColumnTypeGet( etjDB *etjDBActual, etjDBColumnType *p_columnType ){
	return __etjDBIntegerGet( etjDBActual->nodeColumn, "type", "typeNew", p_columnType );
}


etID_STATE				etjDBColumnTypeInDBSet( etjDB *etjDBActual ){
	return etjDBInDBSet( etjDBActual->nodeColumn, "type", "typeNew" );
}

