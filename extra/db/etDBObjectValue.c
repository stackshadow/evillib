/*  Copyright (C) 2014-2016 by Martin Langlotz alias stackshadow

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

#include "evillib_depends.h"
#include "evillib-extra_depends.h"


#include "core/etDebug.h"
#include "core/etObject.h"
#include "memory/etMemory.h"
#include "db/etDBObject.h"
#include "db/etDBObjectTable.h"
#include "db/etDBObjectTableColumn.h"
#include "db/etDBObjectValue.h"




etID_STATE          etDBObjectValueClean( etDBObject *dbObject ){
// check
    etDebugCheckNull( dbObject );

// vars
    json_object_set_new( dbObject->jsonRootObject, "values", json_object() );

    return etID_YES;
}


etID_STATE          etDBObjectValueSet( etDBObject *dbObject, const char *columnName, const char *value ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( columnName );
    etDebugCheckNull( value );

// vars
    json_t          *jsonColumnValues = NULL;
    json_t          *jsonColumnValue = NULL;

// get the object which hold all the values
    jsonColumnValues = json_object_get( dbObject->jsonRootObject, "values" );
    if( jsonColumnValues == NULL ){
        jsonColumnValues = json_object();
        json_object_set_new( dbObject->jsonRootObject, "values", jsonColumnValues );
    }

// save value
    jsonColumnValue = json_object();
    if( json_object_set_new( jsonColumnValue, "inDB", json_integer(0) ) != 0 ) return etID_STATE_ERR;
    if( json_object_set_new( jsonColumnValue, "value", json_string(value) ) != 0 ) return etID_STATE_ERR;
    if( json_object_set_new( jsonColumnValues, columnName, jsonColumnValue ) != 0 ) return etID_STATE_ERR;




    return etID_YES;
}


etID_STATE          __etDBObjectValueNext( etDBObject *dbObject, const char **p_columnName, const char **p_value ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( p_columnName );
    etDebugCheckNull( p_value );



// start iteration if needed
    if( dbObject->jsonIterator == NULL ){

    // vars
        json_t          *jsonColumnValues = NULL;

    // get the object which hold all the values
        jsonColumnValues = json_object_get( dbObject->jsonRootObject, "values" );
        if( jsonColumnValues == NULL ){
            jsonColumnValues = json_object();
            json_object_set_new( dbObject->jsonRootObject, "values", jsonColumnValues );
        }

    // start iteration
        dbObject->jsonObjectToIterate = jsonColumnValues;
        dbObject->jsonIterator = json_object_iter( dbObject->jsonObjectToIterate );

    } else {
    // iterate
        dbObject->jsonIterator = json_object_iter_next( dbObject->jsonObjectToIterate, dbObject->jsonIterator );
    }

// if there is data
    if( dbObject->jsonIterator != NULL){
        json_t          *jsonColumnValue = NULL;

        jsonColumnValue = json_object_iter_value(dbObject->jsonIterator);

    // column name
        *p_columnName = json_object_iter_key( dbObject->jsonIterator );

    // column value
        *p_value = json_string_value( json_object_get(jsonColumnValue,"value") );
        return etID_YES;
    }

    return etID_STATE_NODATA;
}

