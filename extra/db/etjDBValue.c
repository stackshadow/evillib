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
	#include "etjDBValue.h"
#endif


etID_STATE				etjDBValueInit( etjDB *etjDBActual ){
// Check
	etCheckNull( etjDBActual );
	etCheckParameterSequence( etjDBActual->nodeTable );

	if( etjDBActual->nodeValuesNew == NULL ){
		etjDBActual->nodeValuesNew = json_object_get( etjDBActual->nodeTable, "valuesNew" );
	}

	if( etjDBActual->nodeValuesNew == NULL ){
		etjDBActual->nodeValuesNew = json_object();
		json_object_set_new( etjDBActual->nodeTable, "valuesNew", etjDBActual->nodeValuesNew );
	}


	if( etjDBActual->nodeValues == NULL ){
		etjDBActual->nodeValues = json_object_get( etjDBActual->nodeTable, "values" );
	}

	if( etjDBActual->nodeValues == NULL ){
		etjDBActual->nodeValues = json_object();
		json_object_set_new( etjDBActual->nodeTable, "values", etjDBActual->nodeValues );
	}

	return etID_YES;
}


etID_STATE				etjDBValueSet( etjDB *etjDBActual, const char *columnName, const char *value ){
// Check
	etCheckNull( etjDBActual );

// Init the value-struct
	etjDBValueInit( etjDBActual );
	
	json_t *jsonValue = json_object_get( etjDBActual->nodeValuesNew, columnName );
	if( jsonValue == NULL ){
		jsonValue = json_string( value );
		json_object_set( etjDBActual->nodeValuesNew, columnName, jsonValue );
	} else {
		json_string_set( jsonValue, value );
	}
	
	return etID_YES;
}


etID_STATE				etjDBValueSetValid( etjDB *etjDBActual, const char *columnName, const char *value ){
// Check
	etCheckNull( etjDBActual );

// Init the value-struct
	etjDBValueInit( etjDBActual );
	
	json_t *jsonValue = json_object_get( etjDBActual->nodeValues, columnName );
	if( jsonValue == NULL ){
		jsonValue = json_string( value );
		json_object_set( etjDBActual->nodeValues, columnName, jsonValue );
	} else {
		json_string_set( jsonValue, value );
	}
	
	return etID_YES;
}

/** @ingroup etjDBValue
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@fn etID_STATE etjDBValueGet( etjDB *etjDBActual, const char *columnName, const char *value )
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
etID_STATE				__etjDBValueGet( etjDB *etjDBActual, const char *columnName, const char **p_value ){
// Check
	etCheckNull( etjDBActual );

// Init the value-struct
	etjDBValueInit( etjDBActual );

// Vars
	etID_STATE		returnState = etID_STATE_ERROR_INTERNAL;
	json_t 		*jsonValue = NULL;

// Get the normal value
	jsonValue = json_object_get( etjDBActual->nodeValues, columnName );
	returnState = etID_YES;

// Get the "new"-value
	if( jsonValue == NULL ){
		jsonValue = json_object_get( etjDBActual->nodeValuesNew, columnName );
		returnState = etID_STATE_NEW;
	}

// ERROR
	if( jsonValue == NULL ){
		etDebugMessage( etID_LEVEL_ERR, "No value present" );
		*p_value = NULL;
		return etID_STATE_NODATA;
	}


	*p_value = json_string_value( jsonValue );
	return returnState;
}







