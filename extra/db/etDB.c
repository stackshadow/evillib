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
    #include "evillib-extra_depends.h"

	#include "core/etDebug.h"
	#include "core/etObject.h"
	#include "memory/etMemoryBlock.h"
	#include "memory/etMemory.h"
	#include "string/etString.h"
	

    #include "etDBTable.h"
    #include "etDBValue.h"
    #include "etDBFilter.h"
	#include "etDB.h"
#endif

/** @defgroup etDB etDB - Database functions of evillib

The etDB-Functions provide an API to DB's without thinking about querys.
( of course for db's like sqlite, etDB create the querys for you )
The goal ist to provide an easy to use, independent and portable interface to your DB's
\n
The etDB contains seperate parts:
etDB:
 - Your Application use this functions to run DB-Actions and get basic infos about an current connection

etDBTable:
 - The etDBTable-object represent table(s) ( even if there are not in an db ) \n
If multiple tables are used inside one etDBTable-Object, you need to set the curren-table-pointer, \n
all functions use the current selected table inside the etDBTable-Object \n

etDBRelations:
 - describe the relations between two columns of two tables

etDBValue:
 - an single dataset


In 5 Minutes:
 - Prepare etDB, etDBTable and etDBValues
 - Select an Driver and init it
 - use etDBActionSet to prepare etDB for run
 - use etDBRun to execute the query
 - use etDBNextResult to return the next ( or first ) result from the db


*/


const char*        etDB_ACTION_NAMES[etDB_ACTION_COUNT] = {
    "none\0",                   // etDB_ACTION_NONE
    "get table list\0",         // etDB_ACTION_TABLELIST_GET
    "add table\0",              // etDB_ACTION_TABLE_ADD
    "change table\0",           // etDB_ACTION_TABLE_CHANGE
    "remove table\0",           // etDB_ACTION_TABLE_REMOVE
    "get table\0",              // etDB_ACTION_TABLE_GET
    "add column\0",             // etDB_ACTION_COLUMN_ADD
    "change column\0",          // etDB_ACTION_COLUMN_CHANGE
    "remove column\0",          // etDB_ACTION_COLUMN_REMOVE
    "add data\0",               // etDB_ACTION_DATA_ADD
    "change data\0",            // etDB_ACTION_DATA_CHANGE
    "remove data\0",            // etDB_ACTION_DATA_REMOVE    
    "get data\0",               // etDB_ACTION_DATA_GET
    "done\0"
};



/**
@ingroup etDB
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@fn etID_STATE etDBAlloc( etDB *etDBActual )
@~english
@brief Creates an new etDB-Object
@param[out] etDBActual - An pointer to an etDB-object
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*/
etID_STATE          __etDBAlloc( etDB** p_etDBActual, etDebug* etDebugActual ){
	//if( etErrorActual->state < etID_STATE_NOTHING ) return etDebugPrintState(etErrorActual);

// Vars
	etMemoryBlock 		*etMemoryBlockActual = NULL;
	etDB 				*etDBActual = NULL;

// Allocate
	etMemoryRequest( etMemoryBlockActual, sizeof(etDB) );
	etMemoryBlockDataGet( etMemoryBlockActual, etDBActual );

		
	*p_etDBActual = etDBActual;
	return etID_YES;
}

/**
@ingroup etDB
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@fn etID_STATE etDBFree( etDB *etDBActual )
@~english
@brief Release the memory of an etDB-Object and its connected driver !
@param[out] etDBActual - An pointer to an etDB-object
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*/
etID_STATE          __etDBFree( etDB** p_etDBActual, etDebug* etDebugActual ){

// Vars
	etDB 				*etDBActual = *p_etDBActual;
	etMemoryBlock 		*etMemoryBlockActual = NULL;

// free the underlying driver
    if( etDBActual->functions.free != NULL ){
        etDBActual->functions.free(etDBActual, etDebugActual);
    }

// Release our etMemory
	etMemoryBlockFromData( etDBActual, etMemoryBlockActual );
	etMemoryRelease( etMemoryBlockActual );
	
	return etID_YES;
}

/**
@ingroup etDBRelation
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@internal
@~english
@brief Get an string from an json-object

This is an internal helper function.
The Function perform two steps:
 - get the json-string with the name keyNormal and return it to p_value.
 - get the json-string with the name keyNew and return it to p_valueNew.
So the basic idea is to provide two functionality:
1. get the "new" and the "normal" as seperate values.
2. If you provide the same pointer to p_value and p_valueNew the original-value ( if it exists ) has priority

@param[in] jsonObject
@param[in] keyNormal
@param[in] keyNew
@param[out] p_value
@param[out] p_valueNew
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*- @ref etID_STATE_NODATA
 
 */
etID_STATE          __etDBStringGet( json_t *jsonObject, 
                        const char *keyNormal, 
                        const char *keyNew, 
                        const char **p_value,
                        const char **p_valueNew, 
                        etDebug* etDebugActual ){

// Check
    etDebugReturnOnError( etDebugActual );
	etDebugCheckNull( etDebugActual, jsonObject );

// Vars
    json_t*         jsonValue = NULL;
    const char*     value = NULL;

// set to NULL
    if( p_value != NULL ) *p_value = NULL;
    if( p_valueNew != NULL ) *p_valueNew = NULL;

// reset state
    etDebugStateSet( etDebugActual, etID_STATE_NOTHING );

// try get "new"-value
	if( keyNew != NULL ){
        if( p_valueNew != NULL ){
            jsonValue = json_object_get( jsonObject, keyNew );
            if( jsonValue != NULL ){
                
            // get the string value from the json-object
                value = json_string_value( jsonValue );
                
            // return it as NEW
                *p_valueNew = value;
                
            // set state to NEW
                etDebugStateSet( etDebugActual, etID_STATE_NEW );
            }        
        }
    }


// try get "original"-value
    if( keyNormal != NULL ){
        if( p_value != NULL ){
            jsonValue = json_object_get( jsonObject, keyNormal );
            if( jsonValue != NULL ){
                
            // get the string value from the json-object
                value = json_string_value( jsonValue );
                
            // return it as original-value
                *p_value = value;
                
            // set state to aviable data
                etDebugStateSet( etDebugActual, etID_STATE_DATA_AVIABLE );
            }
        }
    }

// ERROR
    if( etDebugActual->state == etID_STATE_NOTHING ){
        etDebugStateSet( etDebugActual, etID_STATE_NODATA );
    }
    
    return etDebugActual->state;
}


etID_STATE          __etDBIntegerGet( json_t *jsonObject, 
                        const char *keyNormal, 
                        const char *keyNew, 
                        int *p_value,
                        int *p_valueNew, 
                        etDebug* etDebugActual ){

// Check
    etDebugReturnOnError( etDebugActual );
	etDebugCheckNull( etDebugActual, jsonObject );

// Vars
    json_t*         jsonValue = NULL;
    int             value = NULL;

// set to NULL
    if( p_value != NULL ) *p_value = etDB_COL_TYPE_DEFAULT;
    if( p_valueNew != NULL ) *p_valueNew = etDB_COL_TYPE_DEFAULT;

// reset state
    etDebugStateSet( etDebugActual, etID_STATE_NOTHING );

// try get "new"-value
	if( keyNew != NULL ){
        if( p_valueNew != NULL ){
            jsonValue = json_object_get( jsonObject, keyNew );
            if( jsonValue != NULL ){
                
            // get the string value from the json-object
                value = json_integer_value( jsonValue );
                
            // return it as NEW
                *p_valueNew = value;
                
            // set state to NEW
                etDebugStateSet( etDebugActual, etID_STATE_NEW );
            }        
        }
    }


// try get "original"-value
    if( keyNormal != NULL ){
        if( p_value != NULL ){
            jsonValue = json_object_get( jsonObject, keyNormal );
            if( jsonValue != NULL ){
                
            // get the string value from the json-object
                value = json_integer_value( jsonValue );
                
            // return it as original-value
                *p_value = value;
                
            // set state to aviable data
                etDebugStateSet( etDebugActual, etID_STATE_DATA_AVIABLE );
            }
        }
    }

// ERROR
    if( etDebugActual->state == etID_STATE_NOTHING ){
        etDebugStateSet( etDebugActual, etID_STATE_NODATA );
    }
    
    return etDebugActual->state;
}


etID_STATE          etDBInDBSet( json_t *jsonObject, const char *keyNormal, const char *keyNew, etDebug* etDebugActual ){
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
			etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_ERR, "json error !" );
			return etID_STATE_ERROR_INTERNAL;
		}
		
		#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "Set from %s to %s", keyNew, keyNormal );
			etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
		#endif
		


		return etID_YES;
	}


// No name or nameNew are present, can not validate
	return etID_YES;
}

/** @ingroup etDB
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english
@brief Prepare the etDB-Object to execute an action
@param[in] etDBActual The pointer to an etDB-Object
@param[in] etDBActionNew The actoion you would like to execute @see etDB_ACTION
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*/
etID_STATE          etDBActionSet( etDB *etDBActual, etDB_ACTION etDBActionNew, etDebug* etDebugActual ){
// Check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, etDBActual);


	etDBActual->action = etDBActionNew;

#ifndef ET_DEBUG_OFF
    snprintf( etDebugTempMessage, etDebugTempMessageLen, 
        "Set action to '%s'(%i)", etDB_ACTION_NAMES[etDBActual->action], etDBActual->action );
    etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
#endif

	return etID_YES;
}


etDB_ACTION         etDBActionGet( etDB *etDBActual, etDebug* etDebugActual ){
// Check
	etCheckNull( etDBActual );
	return etDBActual->action;
}


etID_STATE          etDBIsConnected( etDB *etDBActual, etDebug* etDebugActual ){
	if( etDBActual->functions.isConnected == NULL ) return etID_NO;

	return etDBActual->functions.isConnected( etDBActual, etDebugActual );
}


etID_STATE          etDBRun( etDB *etDBActual, etDBTable *dbTable, etDBFilter *dbFilter, etDBValue *dbValues, etDebug* etDebugActual ){
// Check
	etCheckNull( etDBActual );
	if( etDBActual->action == etDB_ACTION_NONE ) return etID_NO;
	if( etDBActual->action == etDB_ACTION_DONE ) return etID_NO;
	if( etDBActual->action == etDB_ACTION_COUNT ) return etID_NO;

    snprintf( etDebugTempMessage, etDebugTempMessageLen, 
        "Try to run action '%s'(%i)", etDB_ACTION_NAMES[etDBActual->action], etDBActual->action );
    etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
    
switch( etDBActual->action ){

    case etDB_ACTION_TABLELIST_GET:
        if( etDBActual->functions.tableListGet != NULL ){
            return etDBActual->functions.tableListGet( etDBActual, etDebugActual );
        }
        

    case etDB_ACTION_TABLE_ADD:
        if( etDBActual->functions.tableAdd != NULL ){
            return etDBActual->functions.tableAdd ( etDBActual, dbTable, etDebugActual );
        }
    
    case etDB_ACTION_TABLE_CHANGE:
    
    case etDB_ACTION_TABLE_REMOVE:

    case etDB_ACTION_TABLE_GET:
        if( etDBActual->functions.tableGet != NULL ){
            return etDBActual->functions.tableGet( etDBActual, dbTable, etDebugActual );
        }
    
    case etDB_ACTION_DATA_ADD:
        if( etDBActual->functions.dataAdd != NULL ){
            return etDBActual->functions.dataAdd( etDBActual, dbTable, dbValues, etDebugActual );
        }

    case etDB_ACTION_DATA_CHANGE:
        if( etDBActual->functions.dataChange != NULL ){
            return etDBActual->functions.dataChange( etDBActual, dbTable, dbValues, etDebugActual );
        }
        
    case etDB_ACTION_DATA_REMOVE:
        if( etDBActual->functions.dataRemove != NULL ){
            return etDBActual->functions.dataRemove( etDBActual, dbTable, dbValues, etDebugActual );
        }

    case etDB_ACTION_DATA_GET:
        if( etDBActual->functions.dataGet != NULL ){
            return etDBActual->functions.dataGet( etDBActual, dbTable, dbFilter, etDebugActual );
        }
        
    default:
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Action '%s'(%i) not supported by driver", etDB_ACTION_NAMES[etDBActual->action], etDBActual->action );
        etDebugPrintCustomMessage( etDebugActual, etID_LEVEL_DETAIL_DB, etDebugTempMessage );
        break;    
}

	return etID_STATE_NODATA;
}


etID_STATE 			etDBNextResult( etDB *etDBActual, etDBValue *nodeValues, etDebug* etDebugActual ){
	if( etDBActual->functions.nextResult == NULL ) return etID_NO;

	return etDBActual->functions.nextResult( etDBActual, nodeValues, etDebugActual );
}



































