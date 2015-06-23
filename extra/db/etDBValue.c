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
	#include "etDBValue.h"
#endif


etID_STATE				etDBValueInit( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeTable );

// get the "new"-values
	if( etDBActual->nodeValuesNew == NULL ){
		etDBActual->nodeValuesNew = json_object_get( etDBActual->nodeTable, "valuesNew" );
	}
// the "new"-values are not present, create the object
	if( etDBActual->nodeValuesNew == NULL ){
		etDBActual->nodeValuesNew = json_object();
		json_object_set_new( etDBActual->nodeTable, "valuesNew", etDBActual->nodeValuesNew );
	}


// get the "original"-values
	if( etDBActual->nodeValues == NULL ){
		etDBActual->nodeValues = json_object_get( etDBActual->nodeTable, "values" );
	}
// the "original"-values are not present, create the object
	if( etDBActual->nodeValues == NULL ){
		etDBActual->nodeValues = json_object();
		json_object_set_new( etDBActual->nodeTable, "values", etDBActual->nodeValues );
	}

	return etID_YES;
}


etID_STATE				etDBValuesClean( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeTable );

// Clean the "new"-values
	if( etDBActual->nodeValuesNew != NULL ){
		json_object_clear( etDBActual->nodeValuesNew );
	}

	return etID_YES;
}


etID_STATE				etDBValuesNewClean( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeTable );

// Clean the "new"-values
	if( etDBActual->nodeValuesNew != NULL ){
		json_object_clear( etDBActual->nodeValuesNew );
		
		/*
		void *iterator = json_object_iter( etDBActual->nodeValuesNew );
		json_object_clear( etDBActual->nodeValuesNew );
		json_t *jsonValue = NULL;
		
		while( iterator != NULL){
			jsonValue = json_object_iter_value( iterator );
			json_decref(jsonValue);
			
			iterator = json_object_iter_next( etDBActual->nodeValuesNew, iterator );
		}
		
		//json_decref( etDBActual->nodeValuesNew );
		etDBActual->nodeValuesNew = NULL;
		
		 */
	}

	return etID_YES;
}


etID_STATE				etDBValueSet( etDB *etDBActual, const char *columnName, const char *value ){
// Check
	etCheckNull( etDBActual );
	etCheckNull( columnName );

// Init the value-struct
	etDBValueInit( etDBActual );
	etCheckParameterSequence( etDBActual->nodeValuesNew );

// Check if this value is already present
	json_t *jsonValue = json_object_get( etDBActual->nodeValuesNew, columnName );

// Value not present
	if( jsonValue == NULL ){
		jsonValue = json_string( value );
		json_object_set_new( etDBActual->nodeValuesNew, columnName, jsonValue );
	} 
// Value present
	else {
		json_string_set( jsonValue, value );
	}
	
	return etID_YES;
}

/** @ingroup etDBValue
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

This is an internal function for set the values which exists inside an DB.

@warning Before use this function call etDBValueInit() !

*/
etID_STATE				etDBValueOriginalSet( etDB *etDBActual, const char *columnName, const char *value ){
// Check
	etCheckNull( etDBActual );
	etCheckNull( columnName );
	etCheckParameterSequence( etDBActual->nodeValues );

// Init the value-struct
	etDBValueInit( etDBActual );

// Check if this value is already present
	json_t *jsonValue = json_object_get( etDBActual->nodeValues, columnName );

// Value not present
	if( jsonValue == NULL ){
		jsonValue = json_string( value );
		json_object_set( etDBActual->nodeValues, columnName, jsonValue );
	} 
// Value present
	else {
		json_string_set( jsonValue, value );
	}
	
	return etID_YES;
}







/** @ingroup etDBValue
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@fn etID_STATE etDBValueGet( etDB *etDBActual, const char *columnName, const char *value )
@brief Get an value from a column

This function get an "new" or an "actual" value of a column. At first it will be looked if an "actual" value,
which means that this value comes from the DB, exist. If an "actual" value doesnt exist, you get the "new" value,
which means that this value was set from the userspace.


@param[out] value The pointer to an char array
@return
*- @ref etID_YES
*- @ref etID_STATE_NEW
*- @ref etID_STATE_NODATA
*/ 
etID_STATE				__etDBValueGet( etDB *etDBActual, 
							const char *columnName, 
							const char **p_value, 
							const char **p_valueNew
						){
// Check
	etCheckNull( etDBActual );
	etCheckParameterSequence( etDBActual->nodeValues );
	
// Vars
	etID_STATE		returnState = etID_STATE_ERROR_INTERNAL;
	json_t 		*jsonValue = NULL;
	const char		*value = NULL;

// set all to NULL
	*p_value = NULL;
	*p_valueNew = NULL;


// Get the "new"-value
	if( jsonValue == NULL ){
		jsonValue = json_object_get( etDBActual->nodeValues, columnName );
		
		if( jsonValue != NULL ){
			value = json_string_value( jsonValue );
			if( value != NULL ) *p_value = value;
		}
	}

// Get the "normal"-value
	if( jsonValue == NULL ){
		jsonValue = json_object_get( etDBActual->nodeValuesNew, columnName );
		
		if( jsonValue != NULL ){
			value = json_string_value( jsonValue );
			if( value != NULL ) *p_valueNew = value;
		}
	}


// ERROR
	if( *p_value == NULL && *p_valueNew == NULL ){
		etDebugMessage( etID_LEVEL_ERR, "No value present" );
		*p_value = NULL;
		return etID_STATE_NODATA;
	}


	return returnState;
}


etID_STATE				etDBValueGetReset( etDB *etDBActual ){
// Check
	etCheckNull( etDBActual );


// Init the value-struct
	etDBValueInit( etDBActual );

}


