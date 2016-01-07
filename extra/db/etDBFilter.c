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
	#include "evillib-extra_depends.h"

	#include "core/etDebug.h"
	#include "core/etObject.h"
	#include "memory/etMemoryBlock.h"
	#include "memory/etMemory.h"
	#include "string/etString.h"
	
	#include "etDBFilter.h"
#endif




etID_STATE          __etDBFilterAlloc( etDBFilter **p_dbFilter, etDebug* etDebugActual ){
// check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, p_dbFilter );

// vars
    json_t*     jsonFilter = json_object();
    json_t*     jsonFilterArray = json_array();
    json_t*     jsonIndex = json_integer(0);

    json_object_set_new( jsonFilter, "filter", jsonFilterArray );
    json_object_set_new( jsonFilter, "__index__", jsonIndex );

	*p_dbFilter = jsonFilter;
	return etID_YES;
}

/// @todo write free function
etID_STATE          __etDBFilterFree( etDBFilter **p_dbFilter, etDebug* etDebugActual ){
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
etID_STATE          __etDBFilterDump( etDBFilter *dbFilter, const char** p_jsonChar, etDebug* etDebugActual ){
// check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, dbFilter );
    etDebugCheckNull( etDebugActual, p_jsonChar );

	*p_jsonChar = json_dumps( dbFilter, JSON_INDENT(4) | JSON_PRESERVE_ORDER );

	return etID_YES;
}


etID_STATE          etDBFilterClean( etDBFilter *dbFilter, etDebug* etDebugActual ){
// check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, dbFilter );

// vars
    json_t*     jsonFilter = dbFilter;
    json_t*     jsonFilterArray = json_array();

    json_object_del( jsonFilter, "filter" );
    json_object_set_new( jsonFilter, "filter", jsonFilterArray );

    return etID_YES;
}


etID_STATE          etDBFilterAppend( 
                        etDBFilter *dbFilter, 
                        etID_TYPE linkMode,
                        int group, 
                        const char *column, 
                        etDBFilterMode compareMode, 
                        const char *value, 
                        etDebug* etDebugActual ){
// check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, dbFilter );
	if( group <= 0 ){
		etDebugMessage( etID_LEVEL_ERR, "Group must be > 0 !" );
		return etID_STATE_PARAMETER_MISSUSE;
	}

// vars
    json_t*     jsonFilter = dbFilter;
    json_t*     jsonFilterArray = json_object_get( jsonFilter, "filter" );
    json_t*     jsonFilterElement = json_object();


    etDebugCheckNull( etDebugActual, jsonFilterArray );
    etDebugCheckNull( etDebugActual, jsonFilterElement );

// the column
    json_object_set_new( jsonFilterElement, "link", json_integer(linkMode) );
    json_object_set_new( jsonFilterElement, "group", json_integer(group) );
    json_object_set_new( jsonFilterElement, "column", json_string(column) );
    json_object_set_new( jsonFilterElement, "mode", json_integer(compareMode) );
    json_object_set_new( jsonFilterElement, "value", json_string(value) );

	json_array_append_new( jsonFilterArray, jsonFilterElement );

	return etID_YES;
}


etID_STATE          etDBFilterInject( etDBFilter *dbFilter, const char *jsonChar, etDebug* etDebugActual ){
// check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, dbFilter );
    etDebugCheckNull( etDebugActual, jsonChar );

// Vars
    json_error_t        jsonError;
    json_t*             jsonFilters = dbFilter;
    json_t*             jsonFilterArray = json_object_get( jsonFilters, "filter" );
    json_t*             jsonFilterElement;


    jsonFilterElement = json_loads( jsonChar, JSON_INDENT(4) | JSON_PRESERVE_ORDER, &jsonError );
    if( !jsonFilterElement ){
        etDebugMessage( etID_LEVEL_ERR, jsonError.text );
        return etID_STATE_ERROR_INTERNAL;
    }
    json_array_append_new( jsonFilterArray, jsonFilterElement );

    return etID_YES;
}


int                 etDBFilterCount( etDBFilter *dbFilter, etDebug* etDebugActual ){
// check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, dbFilter );

// vars
    json_t*     jsonFilter = dbFilter;
    json_t*     jsonFilterArray = json_object_get( jsonFilter, "filter" );

	if( jsonFilterArray == NULL ) return 0;

	return json_array_size( jsonFilterArray );
}


etID_STATE          etDBFilterGet( 
                        etDBFilter *dbFilter, 
                        int index,
                        etID_TYPE *p_linkMode,
                        int *p_group, 
                        const char **p_column, 
                        etDBFilterMode *p_compareMode, 
                        const char **p_value, 
                        etDebug* etDebugActual ){
// check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, dbFilter );
	if( index < 0 ){
		etDebugMessage( etID_LEVEL_ERR, "Index must be >= 0 !" );
		return etID_STATE_PARAMETER_MISSUSE;
	}


// vars
    json_t*     jsonFilter = dbFilter;
    json_t*     jsonFilterArray = json_object_get( jsonFilter, "filter" );
    json_t*     jsonFilterElement = NULL;
    json_t*     jsonValue = NULL;

// Get filter from json-array
	jsonFilterElement = json_array_get( jsonFilterArray, index );
	if( jsonFilterElement == NULL ) return etID_STATE_NODATA;

// Get vars
	jsonValue = json_object_get( jsonFilterElement, "link" );
	*p_linkMode = json_integer_value( jsonValue );

	jsonValue = json_object_get( jsonFilterElement, "group" );
	*p_group = json_integer_value( jsonValue );

	jsonValue = json_object_get( jsonFilterElement, "column" );
	*p_column = json_string_value( jsonValue );

	jsonValue = json_object_get( jsonFilterElement, "mode" );
	*p_compareMode = json_integer_value( jsonValue );

	jsonValue = json_object_get( jsonFilterElement, "value" );
	*p_value = json_string_value( jsonValue );

	return etID_YES;
}





	
