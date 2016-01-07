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
#endif





// Table Stuff

etID_STATE                  __etDBTableAlloc( etDBTable **p_etDBTable, etDebug* etDebugActual ){
	etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, p_etDBTable );

// vars
    etMemoryBlock*      etMemoryBlockActual = NULL;
    etDBTable*          dbTable;
    
// Allocate
	etMemoryRequest( etMemoryBlockActual, sizeof(etDBTable) );
	etMemoryBlockDataGet( etMemoryBlockActual, dbTable );

// Allocate an empty json-object for the table
    dbTable->jsonTables = json_object();
    dbTable->jsonTable = NULL;
    dbTable->jsonColumns = NULL;
    dbTable->jsonColumn = NULL;
    dbTable->jsonColumnIterator = NULL;

// return
    *p_etDBTable = dbTable;
	return etID_YES;
}


etID_STATE                  __etDBTableFree( etDBTable **p_etDBTable, etDebug* etDebugActual ){
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, p_etDBTable );

// vars
    etDBTable*          dbTable = *p_etDBTable;

    json_decref( dbTable->jsonTables );
    
    return etID_YES;
}


etID_STATE                  __etDBTableDumps( etDBTable* dbTable, const char** p_jsonChar, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
	etDebugCheckNull( etDebugActual, dbTable );
	etDebugCheckMember( etDebugActual, dbTable->jsonTables );

    *p_jsonChar = json_dumps( dbTable->jsonTables, JSON_INDENT(4) | JSON_PRESERVE_ORDER );
	return etID_YES;
}


etID_STATE                  etDBTableAppend( etDBTable *dbTable, const char *tableName, etDebug* etDebugActual ){
    return etDBTableNameSetFull( dbTable, NULL, tableName, etDebugActual );
}


etID_STATE                  etDBTableSeek( etDBTable *dbTable, const char *tableName, etDebug* etDebugActual ){
// Check
    etCheckNull( dbTable );
    etCheckNull( dbTable->jsonTables );
    etCheckNull( tableName );

// Vars
    json_t*         jsonTables = dbTable->jsonTables;
    json_t*         jsonTable = NULL;
    json_t*         jsonTableColumns = NULL;
    
// get the table from tables
    jsonTable = json_object_get( jsonTables, tableName );
    if( jsonTable == NULL ) return etID_STATE_NODATA;
    
// get columns
    jsonTableColumns = json_object_get( jsonTable, "columns" );


// Set the internal pointer to the table
    dbTable->jsonTable = jsonTable;
    dbTable->jsonColumns = jsonTableColumns;
    dbTable->jsonColumn = NULL;
    dbTable->jsonColumnIterator = NULL;
    
    return etID_YES;
}


etID_STATE                  etDBTableDumpf( etDBTable *dbTable, etDebug* etDebugActual ){
// check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, dbTable );
    etDebugCheckNull( etDebugActual, dbTable->jsonTable );
    
	char *dump = json_dumps( dbTable->jsonTable, JSON_INDENT(4) | JSON_PRESERVE_ORDER );
	etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_INFO, dump );
	free(dump);
	//fprintf( stdout, ) );
    
    return etID_YES;
}



etID_STATE                  etDBTableNameSetFull( etDBTable *etDBTableActual, const char *tableName, const char *tableNameNew, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, etDBTableActual );
    etDebugCheckNull( etDebugActual, etDBTableActual->jsonTables );

// Vars
    int             jsonReturnCode = 0;
    json_t*         jsonTable = NULL;
    json_t*         jsonTableColumns = NULL;

// search if the table already exist
    if( tableName != NULL ){
        jsonTable = json_object_get( etDBTableActual->jsonTables, tableName );
    }

// if no table is present, create one
    if( jsonTable == NULL ){
        
    // get the name of the object
        const char *tempChar = NULL;
        if( tableNameNew != NULL ) tempChar = tableNameNew;
        if( tableName != NULL ) tempChar = tableName;
        
    // Create a new table
        jsonTable = json_object();
        jsonReturnCode |= json_object_set_new( etDBTableActual->jsonTables, tempChar, jsonTable );
        
    // create the column-object inside the table
        jsonTableColumns = json_object();
        jsonReturnCode |= json_object_set_new( jsonTable, "columns", jsonTableColumns );

    // Set the internal pointer to the new created table
        etDBTableActual->jsonTable = jsonTable;
        etDBTableActual->jsonColumns = jsonTableColumns;
        etDBTableActual->jsonColumn = NULL;
        etDBTableActual->jsonColumnIterator = NULL;

    // set the name of the table
        if( tableName != NULL ){
            jsonReturnCode |= json_object_set_new( jsonTable, "name", json_string(tableName) );
            #ifndef ET_DEBUG_OFF
                snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set table-name to '%s' ", tableName );
                etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
            #endif
        }

    }


// set the name
    if( tableNameNew != NULL ){
		jsonReturnCode |= json_object_set_new( jsonTable, "nameNew", json_string(tableNameNew) );
		#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set new table-name to '%s' ", tableNameNew );
			etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
		#endif
	}


// Check return code
	if( jsonReturnCode != 0 ){
		etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_ERR, "jansson error" );
		return etDebugStateSet( etDebugActual, etID_STATE_ERROR_INTERNAL);
	}

	return etDebugStateSet( etDebugActual, etID_YES);
}



etID_STATE                  __etDBTableNameGetFull( etDBTable *etDBTableActual, const char **p_tableName, const char **p_tableNameNew, etDebug* etDebugActual ){
    return __etDBStringGet( etDBTableActual->jsonTable, "name", "nameNew", p_tableName, p_tableNameNew, etDebugActual );
}


etID_STATE                  etDBTableVisibleNameSet( etDBTable *etDBTableActual, const char *visibleName, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
	etDebugCheckNull( etDebugActual, etDBTableActual );
	etDebugCheckNull( etDebugActual, etDBTableActual->jsonTable );

// Vars
    int             jsonReturnCode = 0;
    json_t*         jsonTable = etDBTableActual->jsonTable;



// Add table names
	jsonReturnCode |= json_object_set_new( jsonTable, "visibleName", json_string(visibleName) );

// Check return code
	if( jsonReturnCode != 0 ){
		etDebugMessage( etID_LEVEL_ERR, "jansson error" );
		return etDebugStateSet( etDebugActual, etID_STATE_ERROR_INTERNAL);
	}

// return
	return etDebugStateSet( etDebugActual, etID_YES);
}


etID_STATE                  __etDBTableVisibleNameGet( etDBTable *etDBTableActual, const char **p_visibleName, etDebug* etDebugActual ){
// Check
	etDebugCheckNull( etDebugActual, etDBTableActual );

	return __etDBStringGet( etDBTableActual->jsonTable, "visibleName", NULL, p_visibleName, NULL, etDebugActual );
}



