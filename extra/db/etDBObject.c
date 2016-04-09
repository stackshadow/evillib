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

#include "core/etDebug.h"
#include "core/etObject.h"
#include "memory/etMemory.h"
#include "db/etDBObject.h"




/** @defgroup etDBObject
@short The DB-Object


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

// debug message    
    etDebugMessage( etID_LEVEL_DETAIL_DB, dbObject->dumpString );
    

    return etID_YES;
}


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





