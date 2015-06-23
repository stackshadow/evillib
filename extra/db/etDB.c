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
	
	#include "etDB.h"
#endif


etID_STATE				__etDBAlloc( etDB **p_etjDBActual ){
	
// Vars
	etMemoryBlock 		*etMemoryBlockActual = NULL;
	etDB 				*etDBActual = NULL;
	int 				jsonReturnCode = 0;

// Allocate
	etMemoryRequest( etMemoryBlockActual, sizeof(etDB) );
	etMemoryBlockDataGet( etMemoryBlockActual, etDBActual );

// Setup json
	etDBActual->nodeRoot = json_object();
	etDBActual->nodeTables = json_object();

	
// Add version info of specification
	jsonReturnCode |= json_object_set_new( etDBActual->nodeRoot, "ver", json_integer(1) );
	jsonReturnCode |= json_object_set_new( etDBActual->nodeRoot, "tables", etDBActual->nodeTables );

	if( jsonReturnCode != 0 ){
		etDebugMessage( etID_LEVEL_ERR, "json error !" );
		*p_etjDBActual = NULL;
		return etID_STATE_ERROR_INTERNAL;
	}
		
	*p_etjDBActual = etDBActual;
	return etID_YES;
}


etID_STATE				__etDBFree( etDB **p_etjDBActual ){

// Vars
	etDB 				*etDBActual = *p_etjDBActual;
	etMemoryBlock 		*etMemoryBlockActual = NULL;

// Free the tables
	if( etDBActual->nodeTables != NULL ){
		while( etDBActual->nodeTables->refcount > 0 ){
			json_decref( etDBActual->nodeTables );
		}
	}

// Free the tables
	if( etDBActual->nodeTable != NULL ){
		while( etDBActual->nodeTable->refcount > 0 ){
			json_decref( etDBActual->nodeTable );
		}
	}
	
// Free the columns
	if( etDBActual->nodeColumns != NULL ){
		while( etDBActual->nodeColumns->refcount > 0 ){
			json_decref( etDBActual->nodeColumns );
		}
	}

// Free root
	if( etDBActual->nodeRoot != NULL ){
		while( etDBActual->nodeRoot->refcount > 0 ){
			json_decref( etDBActual->nodeRoot );
		}
	}


// Release our etMemory
	etMemoryBlockFromData( etDBActual, etMemoryBlockActual );
	etMemoryRelease( etMemoryBlockActual );
	
	return etID_YES;
}


etID_STATE				__etDBStringGet( json_t *jsonObject, 
							const char *keyNormal, 
							const char *keyNew, 
							const char **p_value,
							const char **p_valueNew
						){

// Check
	etCheckNull( jsonObject );
	etCheckNull( p_value );
	etCheckNull( p_valueNew );

// Vars
	json_t			*jsonValue = NULL;
	const char 	*value = NULL;

// set all to NULL
	*p_value = NULL;
	*p_valueNew = NULL;


// try get "nameNew"
	if( keyNew != NULL ){
		jsonValue = json_object_get( jsonObject, keyNew );
		if( jsonValue != NULL ){
			value = json_string_value( jsonValue );
			if( value != NULL ) *p_valueNew = value;
		}
	}


// try get "name"
	if( keyNormal != NULL ){
		jsonValue = json_object_get( jsonObject, keyNormal );
		if( jsonValue != NULL ){
			value = json_string_value( jsonValue );
			if( value != NULL ) *p_value = value;
		}
	}

// ERROR
	if( *p_valueNew == NULL && *p_value == NULL ){
		etDebugMessage( etID_LEVEL_WARNING, "No value present" );
		*p_value = NULL;
		return etID_STATE_NODATA;
	}

// return
	return etID_YES;
}


etID_STATE				__etDBIntegerGet( json_t *jsonObject, const char *keyNormal, const char *keyNew, int *p_value ){
// Check
	etCheckNull( jsonObject );
	etCheckNull( p_value );

// Vars
	etID_STATE		returnState = etID_STATE_ERROR_INTERNAL;
	json_t			*jsonValue = NULL;

// try get "name"
	jsonValue = json_object_get( jsonObject, keyNormal );
	returnState = etID_YES;

// try get "nameNew"
	if( jsonValue == NULL ){
		jsonValue = json_object_get( jsonObject, keyNew );
		returnState = etID_STATE_NEW;
	}

// ERROR
	if( jsonValue == NULL ){
		etDebugMessage( etID_LEVEL_WARNING, "No value present" );
		*p_value = 0;
		return etID_STATE_NODATA;
	}

// return
	*p_value = json_integer_value(jsonValue);
	return returnState;
}


etID_STATE				etDBInDBSet( json_t *jsonObject, const char *keyNormal, const char *keyNew ){
// Check
	etCheckNull( jsonObject );

// Vars
	int 			jsonReturnCode = 0;
	json_t			*jsonValue = NULL;

// try to get the new value
	jsonValue = json_object_get( jsonObject, keyNew );
	if( jsonValue != NULL ){
		json_incref( jsonValue );
		jsonReturnCode |= json_object_del( jsonObject, keyNew );
		jsonReturnCode |= json_object_set_new( jsonObject, keyNormal, jsonValue );
		
		if( jsonReturnCode != 0 ){
			etDebugMessage( etID_LEVEL_ERR, "json error !" );
			return etID_STATE_ERROR_INTERNAL;
		}
		
		#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set from %s to %s", keyNew, keyNormal );
			etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
		#endif
		


		return etID_YES;
	}


// No name or nameNew are present, can not validate
	return etID_YES;
}


void					etDBDumpf( etDB *etDBActual ){
	
	char *dump = json_dumps( etDBActual->nodeRoot, JSON_INDENT(4) | JSON_PRESERVE_ORDER );
	etDebugMessage( etID_LEVEL_INFO, dump );
	free(dump);
	//fprintf( stdout, ) );
}







































