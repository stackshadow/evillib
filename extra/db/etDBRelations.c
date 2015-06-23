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
	#include "etDBRelations.h"
#endif



json_t*				etDBRelationExtract( etDB *etDBActual ){
// Check
	if( etDBActual == NULL ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "etDBActual is null" );
		etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
		return NULL;
	}
	if( etDBActual->nodeRoot == NULL ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "etDBActual is null" );
		etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
		return NULL;
	}


// Vars
	//int 				jsonReturnCode = 0;
	json_t				*jsonRelations;


	jsonRelations = json_object_get( etDBActual->nodeRoot, "relations" );
	if( jsonRelations != NULL ){
		json_object_del( etDBActual->nodeRoot, "relations" );
		return jsonRelations;
	}


	return NULL;
}


etID_STATE				etDBRelationInject( etDB *etDBActual, json_t *relation ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeRoot );

	json_object_set( etDBActual->nodeRoot, "relations", relation );
	
	return etID_YES;
}


etID_STATE				etDBRelationAppend( 
							etDB *etDBActual, 
							const char *tableA, const char *columnA,
							const char *tableB, const char *columnB
						){


// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeRoot );

// Vars
	//int 				jsonReturnCode = 0;
	json_t				*jsonRelations;
	json_t				*jsonRelation;

// try to get the relation-array
	jsonRelations = json_object_get( etDBActual->nodeRoot, "relations" );
	if( jsonRelations == NULL ){
		jsonRelations = json_array();
		json_object_set_new( etDBActual->nodeRoot, "relations", jsonRelations );
	}

	jsonRelation = json_object();
	json_object_set_new( jsonRelation, "tableA", json_string(tableA) );
	json_object_set_new( jsonRelation, "columnA", json_string(columnA) );
	json_object_set_new( jsonRelation, "tableB", json_string(tableB) );
	json_object_set_new( jsonRelation, "columnB", json_string(columnB) );

	json_array_append_new( jsonRelations, jsonRelation );
	

#ifndef ET_DEBUG_OFF
	const char *tableName;
	etDBTableNameGet( etDBActual, tableName, tableName );
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Append relation to table %s", tableName );
	etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif

// return
	return etID_YES;
}


int						etDBRelationCount( etDB *etDBActual ){
// Check
	if( etDBActual == NULL){
		etDebugState( etID_STATE_PARAMETER_MISSUSE );
		return -1;
	}
	if( etDBActual->nodeRoot == NULL){
		etDebugState( etID_STATE_PARAMETER_MISSUSE );
		return -1;
	}

// Vars
	//int 				jsonReturnCode = 0;
	json_t				*jsonRelations;

// try to get the relation-array
	jsonRelations = json_object_get( etDBActual->nodeRoot, "relations" );
	if( jsonRelations == NULL ){
		return -1;
	}	
	
	return json_array_size(jsonRelations);
}


etID_STATE				etDBRelationFind( 
							etDB *etDBActual, int index,
							const char *table, const char *column,
							const char **p_tableB, const char **p_columnB
						){

// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeRoot );

// Vars
	//int 				jsonReturnCode = 0;
	json_t				*jsonRelations;
	json_t				*jsonRelation;
	json_t				*jsonString;
	const char			*jsonChar;
	int					jsonRelationsIndex;
	int					jsonRelationsCount;
	
// try to get the relation-array
	jsonRelations = json_object_get( etDBActual->nodeRoot, "relations" );
	if( jsonRelations == NULL ){
		return etID_STATE_NODATA;
	}

// Go to every element
	jsonRelationsCount = json_array_size(jsonRelations);
	for( jsonRelationsIndex = 0; jsonRelationsIndex < jsonRelationsCount; jsonRelationsIndex++ ){

		jsonRelation = json_array_get( jsonRelations, jsonRelationsIndex );
		if( jsonRelation == NULL ) return etID_STATE_NODATA;
		
	// Table a ?
		jsonString = json_object_get( jsonRelation, "tableA" );
		jsonChar = json_string_value( jsonString );
		if( strncmp( jsonChar, table, strlen(table) ) == 0 ){
			
		// column a ?
			jsonString = json_object_get( jsonRelation, "columnA" );
			jsonChar = json_string_value( jsonString );
			if( strncmp( jsonChar, column, strlen(column) ) == 0 ){

			// return b
				jsonString = json_object_get( jsonRelation, "tableB" );
				if( jsonString != NULL ) *p_tableB = json_string_value( jsonString );
				else *p_tableB = NULL;
				
				jsonString = json_object_get( jsonRelation, "columnB" );
				if( jsonString != NULL ) *p_columnB = json_string_value( jsonString );
				else *p_tableB = NULL;

				return etID_YES;	
				
			}
		}
		
	// Table b ?
		jsonString = json_object_get( jsonRelation, "tableB" );
		jsonChar = json_string_value( jsonString );
		if( strncmp( jsonChar, table, strlen(table) ) == 0 ){
		
		// column b ?
			jsonString = json_object_get( jsonRelation, "columnB" );
			jsonChar = json_string_value( jsonString );
			if( strncmp( jsonChar, column, strlen(column) ) == 0 ){
				
				
			// return a
				jsonString = json_object_get( jsonRelation, "tableA" );
				if( jsonString != NULL ) *p_tableB = json_string_value( jsonString );
				else *p_tableB = NULL;
				
				jsonString = json_object_get( jsonRelation, "columnA" );
				if( jsonString != NULL ) *p_columnB = json_string_value( jsonString );
				else *p_tableB = NULL;
				
				return etID_YES;
				
			}
			

		}
		

	}


	return etID_NO;
}





