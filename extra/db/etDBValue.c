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
	
    #include "etDBValue.h"
#endif


etID_STATE          __etDBValueAlloc( etDBValue **p_etDBValues ){
	etCheckNull( p_etDBValues );

// vars
    etMemoryBlock*      etMemoryBlockActual = NULL;
    etDBValue*          dbValues;

// Allocate
    etMemoryRequest( etMemoryBlockActual, sizeof(etDBValue) );
    etMemoryBlockDataGet( etMemoryBlockActual, dbValues );
    

// create the json-object
    dbValues->values = json_object();

// Set the iterator to NULL
    dbValues->iterator = NULL;

	*p_etDBValues = dbValues;
	return etID_YES;
}


etID_STATE          etDBValuesClean( etDBValue *etDBValues ){
// Check
	etCheckNull( etDBValues );


// Clean the "new"-values
	json_object_clear( etDBValues->values );

	return etID_YES;
}


etID_STATE          etDBValueSet( etDBValue *etDBValues, const char *columnName, const char *value ){
// Check
	etCheckNull( etDBValues );
	etCheckNull( columnName );

// vars
    json_t*     jsonValues = etDBValues->values;


// Check if this value is already present
	json_t*     jsonValue = json_object_get( jsonValues, columnName );

// Value not present
	if( jsonValue == NULL ){
		jsonValue = json_string( value );
		json_object_set_new( jsonValues, columnName, jsonValue );
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
etID_STATE          __etDBValueGet( etDBValue *etDBValues, const char *columnName, const char **p_value ){
// Check
    etCheckNull( etDBValues );
    etCheckNull( columnName );
    etCheckNull( p_value );

// Vars
    json_t*         jsonValues = etDBValues->values;
    json_t*         jsonValue = NULL;
    const char*     stringValue = NULL;

// set all to NULL
    *p_value = NULL;


// Get the value
    jsonValue = json_object_get( jsonValues, columnName );
    if( jsonValue == NULL ) return etID_STATE_NODATA;

// get the string-value
    stringValue = json_string_value( jsonValue );
    if( stringValue == NULL ) return etID_STATE_NODATA;

// return
    *p_value = stringValue;
    return etID_YES;
}


etID_STATE          __etDBValueGetNext( etDBValue *etDBValues, const char **p_columnName, const char **p_value ){
// Check
    etCheckNull( etDBValues );
    etCheckNull( p_columnName );
    etCheckNull( p_value );

// Vars
    json_t*         jsonValues = etDBValues->values;
    json_t*         jsonValue = NULL;

// set all to NULL
    *p_columnName = NULL;
    *p_value = NULL;

// start from beginning if iterator is null
    if( etDBValues->iterator != NULL ){
        etDBValues->iterator = json_object_iter_next( jsonValues, etDBValues->iterator );
    } else {
        etDBValues->iterator = json_object_iter( jsonValues );
    }



// If iterator is NULL no more data is present
    if( etDBValues->iterator == NULL ){
        return etID_STATE_NODATA;
    }

// return column name
    *p_columnName = json_object_iter_key( etDBValues->iterator );
    
// return columnvalue
    jsonValue = json_object_iter_value( etDBValues->iterator );
    if( jsonValue != NULL ){
        *p_value = json_string_value( jsonValue );
    }

    return etID_YES;
}



etID_STATE          __etDBValueFree( etDBValue **p_etDBValues ){
    
// Parameter check
	etCheckNull( p_etDBValues );


// Vars
    etMemoryBlock*      etMemoryBlockNew = NULL;
    etDBValue*          etDBValues = *p_etDBValues;


// free the json-stuff
    if( etDBValues->values != NULL ){
        json_decref( etDBValues->values );
    }


// Get the Block from the data and release the data
    etMemoryBlockFromData( (void*)etDBValues, etMemoryBlockNew );
    etMemoryBlockRelease( etMemoryBlockNew );


// Return
    *p_etDBValues = NULL;
    return etID_YES;

}

