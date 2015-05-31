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
	
	#include "etjDB.h"
#endif


etID_STATE				__etjDBAlloc( etjDB **p_etjDBActual ){
	
// Vars
	etMemoryBlock 		*etMemoryBlockActual = NULL;
	etjDB 				*etjDBActual = NULL;

// Allocate
	etMemoryRequest( etMemoryBlockActual, sizeof(etjDB) );
	etMemoryBlockDataGet( etMemoryBlockActual, etjDBActual );

// Setup json
	etjDBActual->nodeRoot = json_object();
	etjDBActual->nodeTables = json_object();

	
// Add version info of specification
	json_object_set_new( etjDBActual->nodeRoot, "ver", json_integer(1) );
	json_object_set_new( etjDBActual->nodeRoot, "tables", etjDBActual->nodeTables );


	*p_etjDBActual = etjDBActual;
	return etID_YES;
}


etID_STATE				__etjDBFree( etjDB **p_etjDBActual ){

// Vars
	etjDB 				*etjDBActual = *p_etjDBActual;
	etMemoryBlock 		*etMemoryBlockActual = NULL;

// Free the json-stuff
	json_decref( etjDBActual->nodeTables );

// Release our etMemory
	etMemoryBlockFromData( etjDBActual, etMemoryBlockActual );
	etMemoryRelease( etMemoryBlockActual );
	
	return etID_YES;
}


etID_STATE				__etjDBStringGet( json_t *jsonObject, const char *keyNormal, const char *keyNew, const char **p_value ){
// Check
	etCheckNull( jsonObject );
	etCheckNull( p_value );

// Vars
	etID_STATE		returnState = etID_STATE_ERROR_INTERNAL;
	json_t			*jsonValue = NULL;
	const char 	*value = NULL;

// try get "name"
	jsonValue = json_object_get( jsonObject, keyNormal );
	returnState = etID_YES;

// try get "nameNew"
	if( jsonValue == NULL && keyNew != NULL ){
		jsonValue = json_object_get( jsonObject, keyNew );
		returnState = etID_STATE_NEW;
	}

// ERROR
	if( jsonValue == NULL ){
		etDebugMessage( etID_LEVEL_ERR, "No value present" );
		*p_value = NULL;
		return etID_STATE_NODATA;
	}

// return
	value = json_string_value(jsonValue);
	*p_value = value;
	return returnState;
}


etID_STATE				__etjDBIntegerGet( json_t *jsonObject, const char *keyNormal, const char *keyNew, etjDBColumnType *p_value ){
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


etID_STATE				etjDBInDBSet( json_t *jsonObject, const char *keyNormal, const char *keyNew ){
// Check
	etCheckNull( jsonObject );

// Vars
	int 			jsonReturnCode = 0;
	json_t			*jsonValue = NULL;

// try to get the new value
	jsonValue = json_object_get( jsonObject, keyNew );
	if( jsonValue != NULL ){
		json_incref( jsonValue );
		json_object_del( jsonObject, keyNew );
		json_object_set_new( jsonObject, keyNormal, jsonValue );

		#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set from %s to %s", keyNew, keyNormal );
			etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
		#endif

		return etID_YES;
	}


// No name or nameNew are present, can not validate
	return etID_YES;
}


void					etjDBDumpf( etjDB *etjDBActual ){
	
	char *dump = json_dumps( etjDBActual->nodeRoot, JSON_INDENT(4) | JSON_PRESERVE_ORDER );
	etDebugMessage( etID_LEVEL_INFO, dump );
	free(dump);
	//fprintf( stdout, ) );
}







































