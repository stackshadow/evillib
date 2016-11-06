/*  Copyright (C) 2016 by Martin Langlotz alias stackshadow

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


#include "evillib-extra_depends.h"

#include "core/etDebug.c"
#include "memory/etMemory.c"
#include "memory/etMemoryBlock.c"

#include "memory/etList.c"

#include "core/etDebug.h"
#include "core/etObject.h"
#include "memory/etMemory.h"
#include "db/etDBObject.h"


/** @defgroup etDB Database support
@short The Database-Support

The evillib-database support is split into two major objects. \n
@par etDBObject
The etDBObject holds all informations about tables, columns, and values

@par etDBDriver
The etDBDriver save/load tables, columns and values to an database like sqlite

*/

/** @ingroup etDB
@defgroup etDBObject etDBObject - The core etDBObject
@short The DB-Object

*/

/** @ingroup etDBObject
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etDBObjectAlloc( etDBObject *dbObject )

@~english
@brief Create an empty etDBObject
@param[out] dbObject An pointer to an etDBObject-pointer
@return If the etDBObject was correctly created \n
*- @ref etID_YES
*- @ref etID_STATE_NOMEMORY
*/
etID_STATE          __etDBObjectAlloc( etDBObject **p_dbObject ){
    etDebugCheckNull( p_dbObject );
    
// vars
    etDBObject *tempDBObject = NULL;

// allocate memory
    etMemoryAlloc( tempDBObject, sizeof(etDBObject) );

// set
    tempDBObject->jsonRootObject = json_object();


// return
    *p_dbObject = tempDBObject;
    return etID_YES;
}

/** @ingroup etDBObject
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etDBObjectFree( etDBObject *dbObject )

@~english
@brief Release an etDBObject
@param[in,out] dbObject An pointer to an etDBObject-pointer
@return If the etDBObject was correctly released \n
*- @ref etID_YES
*- @ref etID_STATE_ERR_PARAMETER
*/
etID_STATE          __etDBObjectFree( etDBObject **p_dbObject ){
    etDebugCheckNull( p_dbObject );

// vars
    etDBObject *tempDBObject = *p_dbObject;

// free temporary string
    if( tempDBObject->dumpString != NULL ){
        free((void*)tempDBObject->dumpString);
        tempDBObject->dumpString = NULL;
    }

// release json stuff
    json_decref( tempDBObject->jsonRootObject );
    etMemoryRelease( tempDBObject );
    
// return
    *p_dbObject = NULL;
    return etID_YES;
}

/** @ingroup etDBObject
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Print out the object
@param[in] dbObject The pointer to an etDBObject
@return If the etDBObject was correctly dumped \n
*- @ref etID_YES
*- @ref etID_STATE_NOMEMORY
*/
etID_STATE          etDBObjectDump( etDBObject *dbObject ){
// vars
    etDebugCheckNull( dbObject );

// free dumping string
    if( dbObject->dumpString != NULL ){
        free((void*)dbObject->dumpString);
        dbObject->dumpString = NULL;
    }

// dump string
    dbObject->dumpString = json_dumps( dbObject->jsonRootObject, JSON_INDENT(4) );
    printf( dbObject->dumpString );
// debug message    
    //etDebugMessage( etID_LEVEL_DETAIL_DB, dbObject->dumpString );
    

    return etID_YES;
}

/** @ingroup etDBObject
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Reset the iteration process

If you iterate through values, columns or tables, this function
will set the iteration to the beginning

@param[in] dbObject The pointer to an etDBObject
@return If the etDBObject iterate from the beginning\n
*- @ref etID_YES
*- @ref etID_STATE_NOMEMORY
*/
etID_STATE          etDBObjectIterationReset( etDBObject *dbObject ){
// check
    etDebugCheckNull( dbObject );

// reset all indexess
    dbObject->jsonIteratorIndex = 0;
    dbObject->jsonObjectToIterate = NULL;
    dbObject->jsonIterator = NULL;

// return
    return etID_YES;
}

/** @ingroup etDBObject
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@internal
@~english
@brief Set the type

@param[in] jsonObject The pointer to an json-object
@param[in] type The name of the type
@return If the type was correctly set \n
*- @ref etID_YES
*- @ref etID_STATE_NOMEMORY
*- @ref etID_STATE_ERR_INTERR 
*/
etID_STATE          etDBObjectTypeSet( json_t *jsonObject, const char *type ){
// check
    etDebugCheckNull( jsonObject );
    etDebugCheckNull( type );

// set object text
    int returnCode = json_object_set_new( jsonObject, "type", json_string(type) );
    if( returnCode == 0 ) return etID_YES;


    return etID_STATE_ERR_INTERR;
}

/** @ingroup etDBObject
@short Check if an jsonObject is from type
@param[in] jsonObject The json-object
@param[in] type The type-name to check
@return
  * etID_YES
  * etID_NO
**/
etID_STATE          etDBObjectTypeCheck( json_t *jsonObject, const char *type ){
// check
    etDebugCheckNull( jsonObject );
    etDebugCheckNull( type );

// vars
    json_t      *jsonType = NULL;
    const char  *typeName = NULL;

// get type
    jsonType = json_object_get( jsonObject, "type" );
    if( jsonType == NULL ) return etID_NO;
    typeName = json_string_value( jsonType );

// compare
    if( strncmp( typeName, type, strlen(type) ) == 0 ){
        return etID_YES;
    }

// return
    return etID_NO;
}





