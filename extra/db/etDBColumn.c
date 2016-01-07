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
	#include "evillib-extra_depends.h"

	#include "core/etDebug.h"
	#include "core/etObject.h"
	#include "memory/etMemoryBlock.h"
	#include "memory/etMemory.h"
	#include "string/etString.h"
	

	#include "etDBTable.h"
	#include "etDBColumn.h"
#endif



etID_STATE              etDBColumnAppend( etDBTable *dbTable, const char *columnName, etDebug* etDebugActual ){
    return etDBColumnNameSetFull( dbTable, NULL, columnName, etDebugActual );
}


etID_STATE              etDBColumnSeek( etDBTable *dbTable, const char *columnName, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, dbTable );
    etDebugCheckMember( etDebugActual, dbTable->jsonColumns );
    etDebugCheckNull( etDebugActual, columnName );


// Vars
    json_t*         jsonTableColumns = dbTable->jsonColumns;
    json_t*         jsonTableColumn = NULL;


// try to find the column
	jsonTableColumn = json_object_get( jsonTableColumns, columnName );

// We found it
	if( jsonTableColumn != NULL ){
		#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "Column Found %s", columnName );
			etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
		#endif
		
	// return
		dbTable->jsonColumn = jsonTableColumn;
		return etID_YES;
	}

// Didnt found the column
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Column not Found %s", columnName );
	etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );

	return etID_STATE_NODATA;
}


etID_STATE              etDBColumnIndexReset( etDBTable *dbTable, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
	etDebugCheckNull( etDebugActual, dbTable );
    etDebugCheckMember( etDebugActual, dbTable->jsonColumns );

// Vars
    json_t*         jsonTableColumns = dbTable->jsonColumns;


// Set the iterator to the first column
    dbTable->jsonColumn = NULL;
	dbTable->jsonColumnIterator = json_object_iter( jsonTableColumns );
	if( dbTable->jsonColumnIterator == NULL ) return etID_STATE_NODATA;

// return
	return etID_YES;
}


etID_STATE              __etDBColumnGetNextFull( etDBTable *dbTable, const char **p_columnName, const char **p_columnNameNew, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, dbTable );
    etDebugCheckMember( etDebugActual, dbTable->jsonColumns );

// Vars
    json_t*         jsonTableColumns = dbTable->jsonColumns;

// check if we are not at the end
    if( dbTable->jsonColumnIterator == NULL ) return etID_STATE_NODATA;
    
// get the column from the actual iterator
    dbTable->jsonColumn = json_object_iter_value( dbTable->jsonColumnIterator );

// read the values from the column
    __etDBStringGet( dbTable->jsonColumn, "name", "nameNew", p_columnName, p_columnNameNew, etDebugActual );

// go to the next column
	dbTable->jsonColumnIterator = json_object_iter_next( jsonTableColumns, dbTable->jsonColumnIterator );

// return
	return etID_YES;
}


etID_STATE              etDBColumnNameSetFull( etDBTable *dbTable, const char *columnName, const char *columnNameNew, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
	etDebugCheckNull( etDebugActual, dbTable );
	etDebugCheckMember( etDebugActual, dbTable->jsonColumns );

// Vars
    int             jsonReturnCode = 0;
    json_t*         jsonTempObject = NULL;

// search if the column already exist
    if( columnName != NULL ){
        jsonTempObject = json_object_get( dbTable->jsonColumns, columnName );
    }

// if no column is present, create one
    if( jsonTempObject == NULL ){
    // create a new json-object for column
        jsonTempObject = json_object();
        if( jsonTempObject == NULL ){
            return etDebugStateSet( etDebugActual, etID_STATE_ERROR_INTERNAL);
        }

    // column-name
        if( columnName != NULL ){
            jsonReturnCode |= json_object_set_new( jsonTempObject, "name", json_string(columnName) );
        }

    // get the name of the object
        const char *tempChar = NULL;
        if( columnNameNew != NULL ) tempChar = columnNameNew;
        if( columnName != NULL ) tempChar = columnName;
        

    // append the json-column object
        jsonReturnCode |= json_object_set_new( dbTable->jsonColumns, tempChar, jsonTempObject );
        
        #ifndef ET_DEBUG_OFF
            const char *tableName;
            etDBTableNameGet( dbTable, tableName, etDebugActual );
            snprintf( etDebugTempMessage, etDebugTempMessageLen, "Append column '%s' to table '%s'", tempChar, tableName );
            etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
        #endif
    }

// new name if needed
	if( columnNameNew != NULL ){
		jsonReturnCode |= json_object_set_new( jsonTempObject, "nameNew", json_string(columnNameNew) );
	}

// Check return code
	if( jsonReturnCode != 0 ){
		etDebugMessage( etID_LEVEL_ERR, "jansson error" );
		return etID_STATE_ERROR_INTERNAL;
	}
    
// set the actual column
    dbTable->jsonColumn = jsonTempObject;

	return etID_YES;
}




etID_STATE              __etDBColumnNameGetFull( etDBTable *dbTable, const char **p_columnName, const char **p_columnNameNew, etDebug* etDebugActual ){
	return __etDBStringGet( dbTable->jsonColumn, "name", "nameNew", p_columnName, p_columnNameNew, etDebugActual );
}




etID_STATE              etDBColumnTypeSetFull( etDBTable *dbTable, etDBColumnType columnType, etDBColumnType columnTypeNew, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
	etDebugCheckNull( etDebugActual, dbTable );
	etDebugCheckMember( etDebugActual, dbTable->jsonColumn );

// Vars
    int             jsonReturnCode = 0;
    json_t*         jsonTableColumn = dbTable->jsonColumn;


#ifndef ET_DEBUG_OFF
	const char *columnName;
	etDBColumnNameGet( dbTable, columnName, etDebugActual );
    
    if( columnType >= 0 ){    
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set type of column '%s' to '%i'", columnName, columnType );
        etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
    }

    if( columnTypeNew >= 0 ){    
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set new type of column '%s' to '%i'", columnName, columnTypeNew );
        etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
    }
	
#endif

    if( columnType >= 0 ){
        jsonReturnCode |= json_object_set_new( jsonTableColumn, "type", json_integer(columnType) );
    }

    if( columnTypeNew >= 0 ){
        jsonReturnCode |= json_object_set_new( jsonTableColumn, "typeNew", json_integer(columnTypeNew) );
    }

// Check return code
	if( jsonReturnCode != 0 ){
		etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_ERR, "jansson error" );
		return etID_STATE_ERROR_INTERNAL;
	}

// return
	return etID_YES;
}



etID_STATE              __etDBColumnTypeGetFull( etDBTable *dbTable, etDBColumnType *p_columnType, etDBColumnType *p_columnTypeNew, etDebug* etDebugActual ){
	return __etDBIntegerGet( dbTable->jsonColumn, "type", "typeNew", (int*)p_columnType, (int*)p_columnTypeNew, etDebugActual );
}




etID_STATE              etDBColumnIdentifierSet( etDBTable *dbTable, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
	etDebugCheckNull( etDebugActual, dbTable );
	etDebugCheckMember( etDebugActual, dbTable->jsonTable );
	etDebugCheckMember( etDebugActual, dbTable->jsonColumn );

// vars 
    int             jsonReturnCode = 0;
    json_t*         jsonTable = dbTable->jsonTable;
    json_t*         jsonIdentifier = NULL;
    const char*     columnName;

// set that this is the identifier
    etDBColumnNameGet( dbTable, columnName, etDebugActual );
    etDebugReturnOnError( etDebugActual );

    jsonIdentifier = json_string( columnName );
    if( jsonIdentifier == NULL ) return etDebugState( etID_STATE_ERROR_INTERNAL );
    jsonReturnCode |= json_object_set_new( jsonTable, "identColumn", jsonIdentifier );

// Check return code
    if( jsonReturnCode != 0 ){
        etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_ERR, "jansson error" );
        return etID_STATE_ERROR_INTERNAL;
    }

#ifndef ET_DEBUG_OFF
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set identifier of column '%s' to 1", columnName );
	etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif

	return etID_YES;
}


etID_STATE              __etDBColumnIdentifierGet( etDBTable *dbTable, const char **p_identifierColumn, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
	etDebugCheckNull( etDebugActual, dbTable );
	etDebugCheckMember( etDebugActual, dbTable->jsonTable );

// vars 
    json_t*         jsonTable = dbTable->jsonTable;
    json_t*         jsonIdentifier = NULL;

// Get values
	jsonIdentifier = json_object_get( jsonTable, "identColumn" );
	if( jsonIdentifier != NULL ){
		*p_identifierColumn = json_string_value(jsonIdentifier);
		return etID_YES;
	}


	return etID_STATE_NODATA;
}


etID_STATE              etDBColumnIsIdentifier( etDBTable *dbTable, const char *column, etDebug* etDebugActual ){
// check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, dbTable );
	etDebugCheckNull( etDebugActual, column );

// vars
    const char          *columnIdent = NULL;
    
// get the ident column
    etDBColumnIdentifierGet( dbTable, columnIdent, etDebugActual);
    etDebugReturnOnError( etDebugActual );

    if( strncmp(columnIdent,column,strlen(column)) == 0 ) return etID_YES;
    
    return etID_NO;
}


