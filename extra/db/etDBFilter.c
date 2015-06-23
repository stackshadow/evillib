/* etjDBCondition - contidion for columns
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
	#include "etDBFilter.h"
#endif




etID_STATE				etDBFilterInit( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeTable );

	
// Filter already exist ?
	if( etDBActual->nodeValuesFilters == NULL ){
		etDBActual->nodeValuesFilters = json_object_get( etDBActual->nodeTable, "filter" );
	}
// Filter dont exist, create it
	if( etDBActual->nodeValuesFilters == NULL ){
		etDBActual->nodeValuesFilters = json_array();
		json_object_set_new( etDBActual->nodeTable, "filter", etDBActual->nodeValuesFilters );
	}

	return etID_YES;
}

/** @ingroup etDBFilter
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@fn etID_STATE etDBFilterDump( etDB *etDBActual, const char* jsonChar )
@brief Dump the filter as an json-string

This function create a json-dump to save it. You need to free the returned jsonChar ( if its not null )

@param[in] etDBActual The pointer to an etDB-Object
@param[out] jsonChar The pointer to an const char which will be replaced by an char-pointer 
@warning You must free this char, to prevent memory leak
@return
*- @ref etID_STATE_PARAMETER_MISSUSE
*- @ref etID_STATE_NODATA
*- @ref etID_YES
*/
etID_STATE				__etDBFilterDump( etDB *etDBActual, const char** p_jsonChar ){
// Check
	if( etDBActual == NULL ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "etDBActual is null" );
		etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
		
		*p_jsonChar = NULL;
		return etID_STATE_PARAMETER_MISSUSE;
	}
	if( etDBActual->nodeValuesFilters == NULL ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "No filter is present" );
		etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
		
		*p_jsonChar = NULL;
		return etID_STATE_NODATA;
	}

	*p_jsonChar = json_dumps( etDBActual->nodeValuesFilters, JSON_INDENT(4) | JSON_PRESERVE_ORDER );

	return etID_YES;
}


etID_STATE				etDBFilterInject( etDB *etDBActual, const char *jsonChar ){
// Check
	if( etDBActual == NULL ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "etDBActual is null" );
		etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
		return etID_STATE_PARAMETER_MISSUSE;
	}
	if( etDBActual->nodeValuesFilters == NULL ){
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "No filter is present" );
		etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
		return etID_STATE_NODATA;
	}


// Vars
	json_error_t 		jsonError;
	json_t				*jsonFilter;
	int 				arrayLen;
	
	jsonFilter = json_loads( jsonChar, JSON_INDENT(4) | JSON_PRESERVE_ORDER, &jsonError );
	arrayLen = json_array_append_new( etDBActual->nodeValuesFilters, jsonFilter );
	if( arrayLen > 0 ) return etID_STATE_INTERR;
	
	return etID_YES;
}


etID_STATE				etDBFilterClean( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	
	json_object_del( etDBActual->nodeTable, "filter" );
//	json_decref( etDBActual->nodeValuesFilters );
	etDBActual->nodeValuesFilters = NULL;
	etDBActual->nodeValuesFilter = NULL;

	etDBFilterInit( etDBActual );
/*	
	if( etDBActual->nodeValuesFilters != NULL ){
		json_array_clear( etDBActual->nodeValuesFilters );
	}
*/
	return etID_YES;
}


etID_STATE				etDBFilterAppend( 
							etDB *etDBActual, 
							etID_TYPE linkMode,
							int group, 
							const char *column, 
							etDBFilterMode compareMode, 
							const char *value 
						){
// Check
	etCheckNull( etDBActual );
	if( group <= 0 ){
		etDebugMessage( etID_LEVEL_ERR, "Group must be > 0 !" );
		return etID_STATE_PARAMETER_MISSUSE;
	}

// Vars
	json_t		*filter = json_object();

// Init the filter structure
	etDBFilterInit( etDBActual );
	etCheckParameterSequence( etDBActual->nodeValuesFilters );

// the column
	json_object_set_new( filter, "link", json_integer(linkMode) );
	json_object_set_new( filter, "group", json_integer(group) );
	json_object_set_new( filter, "column", json_string(column) );
	json_object_set_new( filter, "mode", json_integer(compareMode) );
	json_object_set_new( filter, "value", json_string(value) );
	
	json_array_append_new(etDBActual->nodeValuesFilters, filter );

	return etID_YES;
}


int						etDBFilterCount( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	if( etDBActual->nodeValuesFilters == NULL ) return 0;

	return json_array_size( etDBActual->nodeValuesFilters );
}


etID_STATE				etDBFilterGet( 
							etDB *etDBActual, 
							int index,
							etID_TYPE *p_linkMode,
							int *p_group, 
							const char **p_column, 
							etDBFilterMode *p_compareMode, 
							const char **p_value 
						){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeValuesFilters );
	if( index < 0 ){
		etDebugMessage( etID_LEVEL_ERR, "Index must be >= 0 !" );
		return etID_STATE_PARAMETER_MISSUSE;
	}


// Vars
	json_t			*jsonValue;

// Get filter from json-array
	etDBActual->nodeValuesFilter = json_array_get( etDBActual->nodeValuesFilters, index );
	if( etDBActual->nodeValuesFilter == NULL ) return etID_STATE_NODATA;

// Get vars
	jsonValue = json_object_get( etDBActual->nodeValuesFilter, "link" );
	*p_linkMode = json_integer_value( jsonValue );

	jsonValue = json_object_get( etDBActual->nodeValuesFilter, "group" );
	*p_group = json_integer_value( jsonValue );

	jsonValue = json_object_get( etDBActual->nodeValuesFilter, "column" );
	*p_column = json_string_value( jsonValue );

	jsonValue = json_object_get( etDBActual->nodeValuesFilter, "mode" );
	*p_compareMode = json_integer_value( jsonValue );

	jsonValue = json_object_get( etDBActual->nodeValuesFilter, "value" );
	*p_value = json_string_value( jsonValue );

	return etID_YES;
}





	
